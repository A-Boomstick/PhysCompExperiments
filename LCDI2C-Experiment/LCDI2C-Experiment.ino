#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

void setup()
{
  lcd.init(); // initialize the LCD
  lcd.backlight(); // Activates the backlight
  lcd.clear();
}

void loop()
{
  lcd.setCursor(0, 0);         // Sets the point where characters will start being displayed   (0, 0)
  lcd.print("If you see this");        // Prints message from previous cursor set
  lcd.setCursor(2, 1);         // Sets the cursor point to a different location
  lcd.print("The code works!!!"); // Prints message again at cursor set location
}