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
struct dataStruct {
  unsigned long _micros;
  float value;
} myData;

LiquidCrystal_I2C lcd(0x3F, 16, 2);

void setup() {
  printf_begin();
  Serial.begin(115200);
  Serial.println(F("RF24/examples/GettingStarted - ping"));
  lcd.init(); // initialize the lcd
  lcd.noBacklight(); //Close the backlight

  radio.begin();
  // Set the PA Level low to prevent power supply related issues since this is a
  // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);

  // Open a writing and reading pipe on each radio, with opposite addresses
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);
  lcd.print("1Node C");
  lcd.print(radio.getChannel());
  Serial.print("Workign on channel:");
  Serial.println(radio.getChannel());
  myData.value = 1.22;
  delay(1000);
  radio.printDetails();
  lcd.clear();
}
void loop() {
  /****************** Ping Out Role ***************************/
  radio.stopListening();                                    // First, stop listening so we can talk.
  Serial.println(F("Now sending"));
  lcd.setCursor(0, 0);
  lcd.print("S...");
  myData._micros = micros();
  if (!radio.write( &myData, sizeof(myData) )) {
    Serial.println(F("failed"));
    lcd.print("Fail");
  } else {
    lcd.print(myData.value);
  }

  radio.startListening();                                    // Now, continue listening
  unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
  boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not

  lcd.setCursor(0 , 1);
  lcd.print("R...");
  while ( ! radio.available() ) {                            // While nothing is received
    if (micros() - started_waiting_at > 200000 ) {           // If waited longer than 200ms, indicate timeout and exit while loop
      timeout = true;
      break;
    }
  }

  if ( timeout ) {                                            // Describe the results
    Serial.println(F("Failed, response timed out."));
    lcd.print("Fail");
  } else {
    // Grab the response, compare, and send to debugging spew
    radio.read( &myData, sizeof(myData) );
    unsigned long time = micros();
    lcd.print(myData.value);

    // Spew it
    Serial.print(F("Sent "));
    Serial.print(time);
    Serial.print(F(", Got response "));
    Serial.print(myData._micros);
    Serial.print(F(", Round-trip delay "));
    Serial.print(time - myData._micros);
    Serial.print(F(" microseconds Value "));
    Serial.println(myData.value);
  }
  // Try again 1s later
  delay(5000);
} // Loop
