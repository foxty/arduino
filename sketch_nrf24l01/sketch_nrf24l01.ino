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
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 0;
// Used to control whether this node is sending or receiving
bool role = 0;
/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7, 8);
/**********************************************************/
byte addresses[][6] = {"1Node", "2Node"};
LiquidCrystal_I2C lcd(0x3F, 16, 2);

void setup() {
  printf_begin();
  Serial.begin(115200);
  Serial.println(F("RF24/examples/GettingStarted"));
  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));
  lcd.init(); // initialize the lcd
  lcd.noBacklight(); //Close the backlight

  radio.begin();
  // Set the PA Level low to prevent power supply related issues since this is a
  // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);

  // Open a writing and reading pipe on each radio, with opposite addresses
  if (radioNumber) {
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1, addresses[0]);
    radio.setAutoAck(false);
    lcd.print("1Node");
  } else {
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1, addresses[1]);
    radio.setAutoAck(false);
    lcd.print("2Node");
  }

  // Start the radio listening for data
  radio.startListening();
}
void loop() {
  lcd.clear();
  /****************** Ping Out Role ***************************/
  if (role == 1)  {
    radio.stopListening();                                    // First, stop listening so we can talk.

    Serial.println(F("Now sending"));
    lcd.print("S...");
    unsigned long time = micros();                             // Take the time, and send it.  This will block until complete
    if (!radio.write( &time, sizeof(unsigned long) )) {
      Serial.println(F("Failed, wait to next loop."));
      lcd.print("Fail");
    }else {
      Serial.println("Success!");
      lcd.print("Succ");
    }
    //radio.printDetails();
    lcd.setCursor(0, 1);
    lcd.print("R...");
    radio.startListening();                                    // Now, continue listening

    unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
    boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not

    while ( ! radio.available() ) {                            // While nothing is received
      if (micros() - started_waiting_at > 500000 ) {           // If waited longer than 200ms, indicate timeout and exit while loop
        timeout = true;
        break;
      }
    }

    if ( timeout ) {                                            // Describe the results
      Serial.println(F("Response timed out."));
      lcd.print("Fail");
    } else {
      unsigned long got_time;                                 // Grab the response, compare, and send to debugging spew
      radio.read( &got_time, sizeof(unsigned long) );
      unsigned long time = micros();

      // Spew it
      Serial.print(F("Sent "));
      Serial.print(time);
      Serial.print(F(", Got response "));
      Serial.print(got_time);
      Serial.print(F(", Round-trip delay "));
      Serial.print(time - got_time);
      Serial.println(F(" microseconds"));
      lcd.print(time - got_time);
    }

    // Try again 1s later
    delay(5000);
  }
  /****************** Pong Back Role ***************************/
  if ( role == 0 )
  {
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
      delay(100); //Delay 100ms wait receiver to start listening.
      radio.stopListening();                                        // First, stop listening so we can talk
      radio.write( &got_time, sizeof(unsigned long) );              // Send the final one back.
      radio.startListening();                                       // Now, resume listening so we catch the next packets.
      Serial.print(F("Sent response "));
      Serial.println(got_time);
      lcd.print("Done");
      delay(2000);
    }
    //radio.printDetails();
  }
  /****************** Change Roles via Serial Commands ***************************/
  if ( Serial.available() )
  {
    char c = toupper(Serial.read());
    if ( c == 'T' && role == 0 ) {
      Serial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK"));
      role = 1;                  // Become the primary transmitter (ping out)

    } else if ( c == 'R' && role == 1 ) {
      Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));
      role = 0;                // Become the primary receiver (pong back)
      radio.startListening();

    }
  }
} // Loop
