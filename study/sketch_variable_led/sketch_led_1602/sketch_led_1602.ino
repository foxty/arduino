#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);

void setup() {
  // put your setup code here, to run once:
  lcd.init(); // initialize the lcd
  lcd.noBacklight(); //Close the backlight
  lcd.print("    Octopus"); // Print a message to the LCD.

  Serial.begin(9600);
  Serial.println("octopus");
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("aaaa");
}
