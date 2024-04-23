#define CLASSES   5
#define CLASS1    3   // class1 stars, mosfet                    
#define CLASS2    5   // class2 stars, W/O mosfet (only one led)
#define CLASS3    6   // class3 stars, mosfet
#define CLASS4    9   // class4 stars, mosfet
#define CLASS5   10   // class5 stars, mosfet
#define LAMP     11   // led mesh, mosf;ol;et

#define CONTROL  A0   // potentiometer
#define SWITCH    4   // button to switch mode, active=LOW

#define MODE_OFF         0
#define MODE_STAR        1
#define MODE_TWINKLE     2
#define MODE_LAMP        3
#define _MODE_END        4

#define TWINKLE   5

#define MAXVALUE 224
#define MINVALUE 32

int pins[]       = {CLASS1, CLASS2, CLASS3, CLASS4, CLASS5};
int brightness[] = {192,  224,  96, 64, 32};
int off[]        = {0, 0, 0, 0, 0};
int current[]    = {0, 0, 0, 0, 0};

int mode = MODE_OFF;

bool prevStatus = false;

void setup() {
  for (int i=0; i<CLASSES; ++i) {
    pinMode(pins[i], OUTPUT);
    analogWrite(pins[i], 0);
  }
  pinMode(LAMP, OUTPUT);
  pinMode(CONTROL, INPUT_PULLUP);
  pinMode(SWITCH,  INPUT_PULLUP);

  Serial.begin(9600);
}

void setLeds(int* leds) {
  for (int i=0; i<CLASSES; ++i) analogWrite(pins[i], leds[i]);
}

float getFactor() {
  int ifactor(analogRead(CONTROL));
  float factor(ifactor / 800.0);
  if (factor>1) factor=1;
  if (factor<0.3) factor=0.3  return factor;
}

void loop() {
  switch (mode) {
  
    case MODE_OFF: {
      setLeds(off);
      analogWrite(LAMP, 0);
      break;
    }
  
    case MODE_STAR:
    case MODE_TWINKLE: {
      float factor(getFactor());
      for (int i=0; i<CLASSES; ++i) current[i] = (float)brightness[i] * factor;
  
      if (MODE_TWINKLE == mode) {
        for (int i=0; i<CLASSES; ++i) {
          int value(current[i] * (95 + random(0, 2*TWINKLE)) / 100.0);
          if (value<TWINKLE) value=TWINKLE;
          if (value>MAXVALUE) value=MAXVALUE;
          current[i]=value;
        }
      }
  
      setLeds(current);
      analogWrite(LAMP, 0);
      break;
    }
  
    case MODE_LAMP: {
      setLeds(off);
      analogWrite(LAMP, analogRead(CONTROL)/4);
      break;
    }
  }

  bool button(digitalRead(SWITCH) == LOW);

  if (prevStatus && !button)
  {
    ++mode;
    if (mode >= _MODE_END) mode = MODE_OFF;
  }

  prevStatus = button;

  delay(25);
}
