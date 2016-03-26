#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include "OCT_Sensor.h"
#include "nRF24L01.h"
#include "RF24.h"

/**
   利用红外三极管探测火焰
    - DHT11 GND接地,VCC接入5v电源,信号接入到PIN 8
    - Flame Sensor 正极(长针脚)接电源, 负极(短针脚)接10k电阻, 10k电阻另一端接地,Flame Sensor和10k电阻之间信号接入A1
*/

const int RATE = 2000;

//0x27(GREEN), 0x3F(BLUE) - but sometimes it changes...
LiquidCrystal_I2C lcd(0x3F, 16, 2);
HumitureDHT11 dht11A(6);
FlameSensor fs0(FLAME_SENSOR_ANALOG, A0, 50);
FlameSensor fs1(FLAME_SENSOR_ANALOG, A1, 50);
FlameSensor fs2(FLAME_SENSOR_ANALOG, A2, 50);
RF24 radio(7, 8);

//flag of watch dog: 1 - wakup, 0 - sleep
volatile int f_wdt = 1;

void setup() {
  // Init LCD
  lcd.init(); // initialize the lcd
  lcd.noBacklight(); //Close the backlight
  lcd.print("    Octopus"); // Print a message to the LCD.

  wdt_setup();
  Serial.begin(9600);
  //radio.begin();
  //radio.openWritingPipe(pipe);
  //radio.printDetails();
  delay(1000);  //dela and wait the sensor ready
}

/***************************************************
    Name:        ISR(WDT_vect)
    Returns:     Nothing.
    Parameters:  None.
    Description: Watchdog Interrupt Service. This
                 is executed when watchdog timed out.
 ***************************************************/
ISR(WDT_vect) {
  if (f_wdt == 0) {
    f_wdt = 1;
  }
  else {
    Serial.println("WDT Overrun!!!");
  }
}


/*
   Setup watch watch dog
*/
void wdt_setup() {
  /*** Setup the WDT ***/
  /* Clear the reset flag. */
  MCUSR &= ~(1 << WDRF);
  /* In order to change WDE or the prescaler, we need to
     set WDCE (This will allow updates for 4 clock cycles).
  */
  WDTCSR |= (1 << WDCE) | (1 << WDE);
  /* set new watchdog timeout prescaler value */
  WDTCSR = 1 << WDP0 | 1 << WDP3; /* 8.0 seconds */
  /* Enable the WD interrupt (note no reset). */
  WDTCSR |= _BV(WDIE);
}

/***************************************************
    Name:        enterSleep
    Returns:     Nothing.
    Parameters:  None.
    Description: Enters the arduino into sleep mode.

 ***************************************************/
void enter_sleep(void) {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);   /* EDIT: could also use SLEEP_MODE_PWR_DOWN for lowest power consumption. */
  sleep_enable();
  /* Now enter sleep mode. */
  sleep_mode();
  /* The program will continue from here after the WDT timeout*/
  sleep_disable(); /* First thing to do is disable sleep. */
  /* Re-enable the peripherals. */
  power_all_enable();
}

void loop() {
  Serial.print("loop at : ");
  Serial.println(millis());
  lcd.clear();
  int fire0 = fs0.readSensor();
  int fire1 = fs1.readSensor();
  int fire2 = fs2.readSensor();
  Humiture ha = dht11A.readSensor();

  if (fs0.isDetected() || fs1.isDetected() || fs2.isDetected()) {
    Serial.println("Flame Detected!");
    // Send notification to central node.
  }

  lcd.print((int)ha.humidity);
  lcd.print("H ");
  lcd.print((int)ha.temperature);
  lcd.print("T");

  lcd.setCursor(0, 1);
  lcd.print(fire0);
  lcd.print((char)fs0.getState());
  lcd.print(",");
  lcd.print(fire1);
  lcd.print((char)fs1.getState());
  lcd.print(",");
  lcd.print(fire2);
  lcd.print((char)fs2.getState());
  delay(500); //Delay 500 ms to let ohter actions are finished.
  f_wdt = 0;
  enter_sleep();
}
