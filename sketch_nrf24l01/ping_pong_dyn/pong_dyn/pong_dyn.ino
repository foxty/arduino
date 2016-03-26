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
const int min_payload_size = 4;
const int max_payload_size = 32;
const int payload_size_increments_by = 1;
int next_payload_size = min_payload_size;
char receive_payload[max_payload_size + 1]; // +1 to allow room for a terminating NULL char

void setup() {
  printf_begin();
  Serial.begin(115200);
  Serial.println(F("RF24/examples/GettingStarted - pong"));
  lcd.init(); // initialize the lcd
  lcd.noBacklight(); //Close the backlight

  radio.begin();
  // enable dynamic payloads
  radio.enableDynamicPayloads();
  // optionally, increase the delay between retries & # of retries
  radio.setRetries(5, 15);

  // Open a writing and reading pipe on each radio, with opposite addresses
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1, addresses[1]);
  radio.startListening();
  radio.printDetails();

  lcd.print("2Node C");
  lcd.print(radio.getChannel());
  delay(2000);
  // Start the radio listening for data

}
void loop() {
  /****************** Pong Back Role ***************************/

  // if there is data ready
  while ( radio.available() ) {
    lcd.clear();
    lcd.print("R..");
    // Fetch the payload, and see if this was the last one.
    uint8_t len = radio.getDynamicPayloadSize();

    // If a corrupt dynamic payload is received, it will be flushed
    if (!len) {
      lcd.print("C");
      continue;
    }

    radio.read( receive_payload, len );
    // Put a zero at the end for easy printing
    receive_payload[len] = 0;
    // Spew it
    lcd.print(len);
    lcd.print("B");
    Serial.print(F("Got response size="));
    Serial.print(len);
    Serial.print(F(" value="));
    Serial.println(receive_payload);
    // First, stop listening so we can talk
    radio.stopListening();
    // Send the final one back.
    lcd.setCursor(0, 1);
    lcd.print("S..");
    lcd.print(len);
    lcd.print("B..");
    bool re = radio.write( receive_payload, len );
    lcd.print(re ? "S": "F");
    Serial.println(F("Sent response."));
    // Now, resume listening so we catch the next packets.
    radio.startListening();
  }
} // Loop
