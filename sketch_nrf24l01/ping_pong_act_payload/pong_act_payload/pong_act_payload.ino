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
/****************** User Config ***************************/
/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7, 8);
/**********************************************************/

const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };              // Radio pipe addresses for the 2 nodes to communicate.
typedef enum { role_ping_out = 1, role_pong_back } role_e;                 // The various roles supported by this sketch
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};  // The debug-friendly names of those roles
role_e role = role_pong_back;

byte counter = 1;

void setup() {
  printf_begin();
  Serial.begin(115200);
  Serial.println(F("RF24/examples/GettingStarted - pong"));
  lcd.init(); // initialize the lcd
  lcd.noBacklight(); //Close the backlight

  // Setup and configure rf radio
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
  radio.printDetails();

  lcd.print("Pong C:");
  lcd.print(radio.getChannel());
  delay(1000);
  lcd.clear();
}
void loop() {
  /****************** Pong Back Role ***************************/
  lcd.setCursor(0, 0);
  lcd.print("R..");
  byte pipeNo;
  byte gotByte;                                       // Dump the payloads until we've gotten everything
  while (radio.available(&pipeNo)) {
    radio.read( &gotByte, 1 ); 
    lcd.print(gotByte);
    radio.writeAckPayload(pipeNo, &gotByte, 1 );
    lcd.setCursor(0, 1);
    lcd.print("A..");
    lcd.print(gotByte);
    printf("Get %d . ", gotByte);
    delay(1000);
    lcd.clear();
  }
} // Loop
