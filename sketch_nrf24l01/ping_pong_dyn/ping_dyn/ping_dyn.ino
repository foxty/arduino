/*
  Getting Started example sketch for nRF24L01+ radios
  This is a very basic example of how to send data from one node to another
  Updated: Dec 2014 by TMRh20
*/
#include <SPI.h>
#include "RF24.h"
#include <LiquidCrystal_I2C.h>
#include <printf.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);
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
  Serial.println(F("RF24/examples/GettingStarted - ping"));
  lcd.init(); // initialize the lcd
  lcd.noBacklight(); //Close the backlight

  radio.begin();
  // enable dynamic payloads
  radio.enableDynamicPayloads();
  // optionally, increase the delay between retries & # of retries
  radio.setRetries(5, 15);

  // Open a writing and reading pipe on each radio, with opposite addresses
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);
  radio.printDetails();

  lcd.print("1Node C");
  lcd.print(radio.getChannel());
  delay(2000);
}
void loop() {
  /****************** Ping Out Role ***************************/
  lcd.clear();

  radio.stopListening();
  // The payload will always be the same, what will change is how much of it we send.
  static char send_payload[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ789012345";
  // Take the time, and send it.  This will block until complete
  lcd.print("S..");
  lcd.print(next_payload_size);
  lcd.print("B..");
  Serial.print(F("Now sending length "));
  Serial.println(next_payload_size);
  bool re = radio.write( send_payload, next_payload_size );
  lcd.print(re ? "S" : "F");
  // Now, continue listening
  lcd.setCursor(0, 1);
  lcd.print("R..");
  radio.startListening();
  // Wait here until we get a response, or timeout
  unsigned long started_waiting_at = millis();
  bool timeout = false;
  while ( ! radio.available() && ! timeout )
    if (millis() - started_waiting_at > 500 )
      timeout = true;
  // Describe the results
  if ( timeout ) {
    Serial.println(F("Failed, response timed out."));
    lcd.print("F");
  } else {
    // Grab the response, compare, and send to debugging spew
    uint8_t len = radio.getDynamicPayloadSize();

    // If a corrupt dynamic payload is received, it will be flushed
    if (!len) {
      lcd.print("C");
      return;
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
  }

  // Update size for next time.
  next_payload_size += payload_size_increments_by;
  if ( next_payload_size > max_payload_size )
    next_payload_size = min_payload_size;
  // Try again 1s later
  delay(1000);
} // Loop
