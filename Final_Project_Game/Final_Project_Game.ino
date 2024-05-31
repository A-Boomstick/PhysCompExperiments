//  Import the library we need for manipulating the leds on the led strip
#include<Adafruit_NeoPixel.h>
#include<LiquidCrystal_I2C.h>
//  Define a constant which describes the number of leds on the led strip
#define NUMPIXELS 144
//  Define a constant which describes which pin on the arduino should be used to transfer data to the led strip
#define LED_STRIP_PIN 5

//  Define a constant which describes how bright the leds on the led strip should be
#define BRIGHTNESS 15


//  Define the led strip as a variable. Pass in the constants required by the strip to work as arguments.
//  The last argument describes the format the library will use to transfer data to the led strip, which might differ between strips. They claim that the strip should receive its data at 800 khz, and that the strip is expecting the data to be in GRBW format (our strip has a white leds on it, meaning that it uses the GRBW format)
Adafruit_NeoPixel strip(NUMPIXELS, LED_STRIP_PIN, NEO_GRBW + NEO_KHZ800);

// Setting the LCD address
LiquidCrystal_I2C lcd(0x27,20,4);

//  Define a constant which describes which pin the button used for confirming your selected led is used for.
#define FIRE_BUTTON 7

//  Defining variables for numbers
float fruit_position_f = 99;
int fruit_size = 20;
int level = 19;
int ship_position = 1;
int ship_speed = 1;
int high_score = 0;
int current_score = 0;
int dodge = 5;
float fruit_speed = 0;

void drawline(int position, int size, uint32_t color){ // Function to draw a line of pixels with variable size, saves repeating the code
  for (int i = 0; i<size; i++){
    strip.setPixelColor(position + i, color);
  }
}

int trail_positions[] = {
  0,0,0,0
};
void trail(int position, int direction, int colortype){ // Tracks last few of ship's positions and fills with light that has less intensity
  int power = 128;
  for(int i=0; i<4; i++){
    strip.setPixelColor(trail_positions[i], strip.Color(power, 0,0));
    power /= 2;
  }
  strip.setPixelColor(trail_positions[3], 0);
  for(int i=2; i>=0; i--){
    trail_positions[i+1] = trail_positions[i];
  }
  trail_positions[0] = position;
}


void setup() {
  // put your setup code here, to run once:

  lcd.init();
  lcd.backlight();

  Serial.begin(9600);

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS);

  lcd.setCursor(3, 0);
  lcd.print("Game Start!");

  pinMode(FIRE_BUTTON, INPUT_PULLUP); //  Tell the arduino that we are going to be reading this pin's value, and that the arduino should connect this pin to its built-in resistor. This means that we don't have to add in an extra resistor between the button and the arduino, as the arduino can do this for us.

  tone(9, 293, 50);
  delay(100);
  tone(9, 349, 50);
  delay(100);
  tone(9, 440, 50);
  delay(100);

}

void loop() {  
  int fruit_position = (int)fruit_position_f;
  if (ship_position == NUMPIXELS){ // Causes ship to bounce and change direction when hitting edge
    ship_speed = -1;
    tone(9, 200);
    delay(50);
    noTone(9);
    delay(50);
  }
  else if(ship_position <= 0){
    ship_speed = 1;
    tone(9, 200);
    delay(50);
    noTone(9);
    delay(50);
  }

  if (fruit_position + fruit_size == NUMPIXELS){ // Same as above but for fruit
    fruit_speed *= -1;
  }
  else if (fruit_position == 0){
    fruit_speed *= -1;
  }


  
  ship_position += ship_speed; // This is what makes the ship and fruit move
  fruit_position_f += fruit_speed;


  uint32_t fruit_color = strip.Color(0, 255, 0);  //  Sets up the colour of the fruit as green. The colour is stored as a 32-bit unsigned integer (uint32_t)

  
  if (level > 10 && ship_position<fruit_position && ship_position>fruit_position-2 && dodge > 0){ // Dodge code for later levels
    drawline(fruit_position, fruit_size, 0);
    drawline(fruit_position-dodge, fruit_size, fruit_color);
    fruit_position_f -= (float)dodge;
    dodge = 0;
  }
  else if(level > 10 && ship_position>fruit_position+fruit_size && ship_position<fruit_position+fruit_size+2 && dodge > 0){ // Dodge in different direction
    drawline(fruit_position, fruit_size, 0);
    drawline(fruit_position+dodge, fruit_size, fruit_color);
    fruit_position_f += (float)dodge;
    dodge = 0;
  }
  else{
    drawline(fruit_position-10, 10, 0);
    drawline(fruit_position+fruit_size, 10,0);
    drawline(fruit_position, fruit_size, fruit_color);
  }

  Serial.print(fruit_position);
  Serial.print(' ');
  Serial.print(fruit_size);
  Serial.print(' ');
  Serial.println(level); // Printing stuff for debugging
  strip.show(); //  When you set the colours of the leds on the strip they won't change on their own. The strip.show() function forces the leds to update their colour to the ones previously set.

  trail(ship_position, 0, 0);

  uint32_t ship_color = strip.Color(255, 0, 0);
  strip.setPixelColor(ship_position, ship_color);

  strip.show();
  if(!digitalRead(FIRE_BUTTON)){ // Only triggers when button is hit
    if(ship_position>=fruit_position && ship_position < fruit_position+fruit_size){
      int center = fruit_position + fruit_size/2; // These two lines calculate score gain based on accuracy
      int ship_accuracy = abs(ship_position - center); 
      drawline(0, NUMPIXELS, strip.Color(0,255,0));
      level++;
      ship_position = 1;
      current_score += fruit_size/2-ship_accuracy+level;
      if (high_score < current_score){ // Sets high-score
        high_score = current_score;
      }
      lcd.clear();
      lcd.print("Level Complete!");
      lcd.setCursor(3, 1);
      lcd.print("+");
      lcd.print(fruit_size/2-ship_accuracy+level);
      lcd.print("00 points");
      lcd.setCursor(3, 0);
      tone(9, 440, 50);
      delay(100);
      tone(9, 466, 50);
      delay(100);
      tone(9, 440, 50);
    }else{
      drawline(0, NUMPIXELS, strip.Color(255,0,0));
      level = 1;
      ship_position = 1;
      lcd.clear();
      lcd.print("Level Failed");
      tone(9, 223, 50);
      delay(100);
      tone(9, 185, 50);
      delay(100);
      tone(9, 146, 50);
      current_score = 0;
      fruit_speed = 0;
      
    }
    dodge = random(0, 10);
    strip.show(); //  Update the leds on the strip

    delay(2500);  //  We should wait a second before turning the leds on the strip off.
    lcd.clear();
    lcd.print("Score:");
    lcd.print(current_score);
    lcd.print("00");
    lcd.setCursor(0, 1);
    lcd.print("HighScore:");
    lcd.print(high_score);
    lcd.print("00");
    drawline(0, NUMPIXELS, 0);

    strip.show(); //  Update the leds on the strip

    if (level < 11){
      fruit_size = 20 - random(level, level+3); // Randomly generates a number to subtract, making it harder over time
    }
    else if (level > 10){ // Ensures no soft-locking or impossible levels
      fruit_size = random(4, 7);
    }

    if (level >= 20){
      fruit_speed = (float)random(-(level-15),level-15) * 0.05;
    }
    
    fruit_position_f = random(10,NUMPIXELS-fruit_size); //  Randomly choose a new position for the fruit
    delay(750);

    if(level == 1){ // Special tune for level 1
      tone(9, 293, 50);
      delay(100);
      tone(9, 349, 50);
      delay(100);
      tone(9, 440, 50);
      delay(100);
    }
    else if(level == 11){ // Special tune for when tricks come into play
      tone(9, 466, 50);
      delay(100);
      tone(9, 440, 50);
      delay(100);
      tone(9, 349, 50);
      delay(100);
    }

  }

}