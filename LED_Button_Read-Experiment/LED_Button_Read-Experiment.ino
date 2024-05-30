#define BTN_PRESS 2


void setup() {

  pinMode(3, OUTPUT);
  
  pinMode(BTN_PRESS, INPUT_PULLUP);
}

void loop() {
  
  if(!digitalRead(BTN_PRESS)){
    digitalWrite(3,HIGH);
  }
  digitalWrite(3, LOW);
}