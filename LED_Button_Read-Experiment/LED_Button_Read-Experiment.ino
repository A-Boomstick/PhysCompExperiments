#define BTN_PRESS 2


void setup() {

  pinMode(3, OUTPUT); // Sets pin to output mode
  
  pinMode(BTN_PRESS, INPUT_PULLUP); // Sets BTN_PRESS's pin to take input using built-in resistor
}

void loop() {
  
  if(!digitalRead(BTN_PRESS)){ // Pullup makes the circuit work a little differently so this activates when the button is pressed
    digitalWrite(3,HIGH);
  }
  digitalWrite(3, LOW);
}