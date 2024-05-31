//  Import the library we need for manipulating the leds on the led strip
#include<Adafruit_NeoPixel.h>

//  Define a constant which describes the number of leds on the led strip
#define NUMPIXELS 144
//  Define a constant which describes which pin on the arduino should be used to transfer data to the led strip
#define LED_STRIP_PIN 5

//  Define a constant which describes how bright the leds on the led strip should be
#define BRIGHTNESS 15


//  Define the led strip as a variable. Pass in the constants required by the strip to work as arguments.
//  The last argument describes the format the library will use to transfer data to the led strip, which might differ between strips. They claim that the strip should receive its data at 800 khz, and that the strip is expecting the data to be in GRBW format (our strip has a white leds on it, meaning that it uses the GRBW format)
Adafruit_NeoPixel strip(NUMPIXELS, LED_STRIP_PIN, NEO_GRBW + NEO_KHZ800);

//  Define a constant which describes which pin the arduino should read to get the potentiometer's value
#define POTENTIOMETER A2

//  Define a constant which describes which pin the button used for confirming your selected led is used for.
#define FIRE_BUTTON 7

//  Define a variable which stores the position of the green led
int fruit_position = 99;

void setup() {
  // put your setup code here, to run once:

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS);

  pinMode(POTENTIOMETER, INPUT);  //  Tell the arduino that we are going to be reading the value of the pin the potentiometer is connected to.
  pinMode(FIRE_BUTTON, INPUT_PULLUP); //  Tell the arduino that we are going to be reading this pin's value, and that the arduino should connect this pin to its built-in resistor. This means that we don't have to add in an extra resistor between the button and the arduino, as the arduino can do this for us.
}

void loop() {
  //  Read the value of the potentiometer
  int potentiometer_value = analogRead(POTENTIOMETER);

  //  Convert the value we are reading from the potentiometer to determine which led on the strip should be lit up.
  //  On an arduino uno board the int type describes an signed 16-bit integer (an integer between -32768 and 32767), but we might need a larger precision for this, so an uint32_t is used to force the arduino to use a larger integer.
  //  uint32_t always describes a 32-bit unsigned integer (an integer between 0 and 4294967295).
  uint32_t ship_position = map(potentiometer_value, 0,1024, 0,NUMPIXELS);  

  uint32_t fruit_color = strip.Color(0, 255, 0);  //  Sets up the colour of the fruit as green. The colour is stored as a 32-bit unsigned integer (uint32_t)

  strip.setPixelColor(fruit_position, fruit_color);
  delay(1);
  strip.show(); //  When you set the colours of the leds on the strip they won't change on their own. The strip.show() function forces the leds to update their colour to the ones previously set.

  uint32_t ship_color = strip.Color(255, 0, 0);
  strip.setPixelColor(ship_position, ship_color);

  strip.show();
  if(!digitalRead(FIRE_BUTTON)){
    if(ship_position==fruit_position){
      for(int i=0; i<NUMPIXELS; i++) {
        strip.setPixelColor(i, strip.Color(0,255,0));  //  Iterates over every pixel in the strip and sets them all to green. This is done to indicate that the user clicked the fruit
      }
    }else{
      for(int i=0; i<NUMPIXELS; i++) {
        strip.setPixelColor(i, strip.Color(255,0,0));  //  Iterates over every pixel in the strip and sets them all to red. This is done to indicate that the user didn't click the fruit
      }
    }

    strip.show(); //  Update the leds on the strip

    delay(1000);  //  We should wait a second before turning the leds on the strip off.

    for(int i=0; i<NUMPIXELS; i++){
      strip.setPixelColor(i, 0); //  Turn all the leds on the strip off. This can be done by setting the colour of each led to either black (0,0,0) or just 0.
    }

    strip.show(); //  Update the leds on the strip

    fruit_position = random(0,NUMPIXELS); //  Randomly choose a new position for the fruit
    delay(750);
  }
  delay(1);
  strip.setPixelColor(ship_position, 0);  //  turn off the led responsible for the ship so moving the ship doesn't create persisting pixels.
}
