void setup() {
  // put your setup code here, to run once:
  pinMode(5, OUTPUT); // Sets the pin that will take the output
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(5,HIGH); // This makes the LED glow brightly
  delay(1000);
  digitalWrite(5,LOW); // Makes the LED stop glowing
  delay(1000);
}
