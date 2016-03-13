/*
  Getting Started example sketch for nRF24L01+ radios
  This is a very basic example of how to send data from one node to another
  Updated: Dec 2014 by TMRh20
*/
#include <SPI.h>
#include "RF24.h"
#include <LiquidCrystal_I2C.h>
#include <printf.h>
/****************** User Config ***************************/
/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7, 8);
/**********************************************************/
byte addresses[][6] = {"1Node", "2Node"};
LiquidCrystal_I2C lcd(0x3F, 16, 2);

void setup() {
  printf_begin();
  Serial.begin(115200);
  Serial.println(F("RF24/examples/GettingStarted - pong"));
  lcd.init(); // initialize the lcd
  lcd.noBacklight(); //Close the backlight

  radio.begin();
  // Set the PA Level low to prevent power supply related issues since this is a
  // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);

  // Open a writing and reading pipe on each radio, with opposite addresses
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1, addresses[1]);
  radio.setAutoAck(false);
  lcd.print("2Node C:");
  lcd.print(radio.getChannel());
  delay(1000);
  radio.printDetails();
  // Start the radio listening for data
}
void loop() {
  lcd.clear();

  /****************** Pong Back Role ***************************/
  radio.startListening();
  unsigned long got_time;
  lcd.print("R...");
  if ( radio.available()) {
    // Variable for the received timestamp
    while (radio.available()) {                                   // While there is data ready
      radio.read( &got_time, sizeof(unsigned long) );             // Get the payload
    }
    lcd.print(got_time);     

    lcd.setCursor(0, 1);
    lcd.print("S...");
    delay(1000); //Delay 100ms wait receiver to start listening.
    radio.stopListening();                                        // First, stop listening so we can talk
    if (!radio.write( &got_time, sizeof(unsigned long))) {
      Serial.println("Send back failed!");
      lcd.print("Fail");
    } else {
      lcd.print("Done");
    }
    Serial.print(F("Sent response "));
    Serial.println(got_time);
  }
  delay(1000);
} // Loop
