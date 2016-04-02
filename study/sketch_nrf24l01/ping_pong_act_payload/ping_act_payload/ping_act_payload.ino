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
/****************** User Config ***************************/
/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7, 8);
/**********************************************************/
const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };              // Radio pipe addresses for the 2 nodes to communicate.
typedef enum { role_ping_out = 1, role_pong_back } role_e;                 // The various roles supported by this sketch
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};  // The debug-friendly names of those roles
role_e role = role_ping_out;
byte counter = 1;

void setup() {
  printf_begin();
  Serial.begin(115200);
  Serial.println(F("RF24/examples/GettingStarted - ping"));
  lcd.init(); // initialize the lcd
  lcd.noBacklight(); //Close the backlight

  radio.begin();
  radio.setAutoAck(true);                    // Ensure autoACK is enabled
  radio.enableAckPayload();               // Allow optional ack payloads
  radio.setRetries(0, 15);                // Smallest time between retries, max no. of retries
  radio.setPayloadSize(1);                // Here we are sending 1-byte payloads to test the call-response speed
  if ( role == role_ping_out ) {
    Serial.println(F("Ping"));
    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1, pipes[1]);
  }
  else {
    Serial.println(F("Pong"));
    radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1, pipes[0]);
    radio.startListening();
  }
  radio.startListening();                 // Start listening
  radio.printDetails();

  lcd.print("Ping C:");
  lcd.print(radio.getChannel());
  Serial.print("Workign on channel:");
  Serial.println(radio.getChannel());
  delay(1000);
  lcd.clear();
}
void loop() {
  /****************** Ping Out Role ***************************/
  radio.stopListening();                                  // First, stop listening so we can talk.
  lcd.clear();
  printf("Now sending %d as payload. ", counter);
  lcd.print("S..");
  lcd.print(counter);
  lcd.print("..");
  byte gotByte;
  unsigned long time = micros();                          // Take the time, and send it.  This will block until complete
  //Called when STANDBY-I mode is engaged (User is finished sending)
  if (!radio.write( &counter, 1 )) { 
    Serial.println(F("failed."));
    lcd.print("F");
  } else {
    lcd.setCursor(0, 1);
    lcd.print("R..");
    if (!radio.available()) {
      Serial.println(F("Blank Payload Received."));
      lcd.print("B");
    } else {
      while (radio.available() ) {
        unsigned long tim = micros();
        radio.read( &gotByte, 1 );
        printf("Got response %d, round-trip delay: %lu microseconds\n\r", gotByte, tim - time);
        lcd.print(gotByte);
        lcd.print(" T");
        lcd.print(tim - time);
        counter++;
      }
    }
  }
  // Try again later
  delay(2000);
} // Loop
