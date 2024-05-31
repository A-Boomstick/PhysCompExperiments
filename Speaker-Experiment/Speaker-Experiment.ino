


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  tone(9, 110); // Makes the device connected to this pin attempt to play sound of a specific frequency
  delay(1000);
  noTone(9); // Ceases the output from the earlier tone function in this specific pin
  delay(2000);
}
