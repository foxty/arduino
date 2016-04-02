/*
  Getting Started example sketch for nRF24L01+ radios
  This is a very basic example of how to send data from one node to another
  Updated: Dec 2014 by TMRh20
*/
#include <SPI.h>
#include "RF24.h"
#include <LiquidCrystal_I2C.h>
#include <printf.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
RF24 radio(7, 8);

/**********************************************************/
byte addresses[][6] = {"1Node", "2Node"};
struct dataStruct {
  unsigned long _micros;
  float value;
} myData;

void setup() {
  printf_begin();
  Serial.begin(115200);
  Serial.println(F("RF24/examples/GettingStarted - pong"));
  lcd.init(); // initialize the lcd
  lcd.noBacklight(); //Close the backlight

  radio.begin();
  // Open a writing and reading pipe on each radio, with opposite addresses
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1, addresses[1]);
  lcd.print("2Node C");
  lcd.print(radio.getChannel());
  delay(2000);
  radio.printDetails();
  lcd.clear();
  // Start the radio listening for data
  radio.startListening();

}
void loop() {
  /****************** Pong Back Role ***************************/
  if ( radio.available()) {
    lcd.setCursor(0, 0);
    lcd.print("R...");
    while (radio.available()) {                          // While there is data ready
      radio.read( &myData, sizeof(myData) ); // Get the payload
      lcd.print(myData.value);
    }

    radio.stopListening();                               // First, stop listening so we can talk
    myData.value += 0.01;       // Increment the float value
    lcd.setCursor(0, 1);
    lcd.print("S...");
    bool re = radio.write( &myData, sizeof(myData) );              // Send the final one back.
    if (re) {
      lcd.print(myData.value);
    } else {
      lcd.print("Fail");
    }
    radio.startListening();                              // Now, resume listening so we catch the next packets.
    Serial.print(F("Sent response "));
    Serial.print(myData._micros);
    Serial.print(F(" : "));
    Serial.println(myData.value);
  }
} // Loop
