#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include "OCT_Sensor.h"
#include "nRF24L01.h"
#include "RF24.h"

/**
   利用红外三极管探测火焰,检测到则出发红色LED报警

    - LM35 信号接A0
    - DHT11 GND接地,VCC接入5v电源,信号接入到PIN 8
    - Flame Sensor 正极(长针脚)接电源, 负极(短针脚)接10k电阻, 10k电阻另一端接地,Flame Sensor和10k电阻之间信号接入A1
    - 红色LED接入3,蓝色LED接4
*/  

const int RATE = 2000;
//const uint64_t pipe = 0xE8E8F0F0E1LL;

//0x27(GREEN), 0x3F(BLUE) - but sometimes it changes...
LiquidCrystal_I2C lcd(0x27, 16, 2);
HumitureDHT11 dht11A(6);
FlameSensor fs0(FLAME_SENSOR_ANALOG, A0, 50);
FlameSensor fs1(FLAME_SENSOR_ANALOG, A1, 50);
FlameSensor fs2(FLAME_SENSOR_ANALOG, A2, 50); 
//RF24 radio(7, 8);

void setup() {
  // Init LCD
  lcd.init(); // initialize the lcd
  lcd.noBacklight(); //Close the backlight
  lcd.print("    Octopus"); // Print a message to the LCD.

  Serial.begin(9600);
  //Serial.begin(57600);
  //radio.begin();
  //radio.openWritingPipe(pipe);
  //radio.printDetails();
  delay(1000);  //dela and wait the sensor ready
}

void loop() {
  lcd.clear();

  int fire0 = fs0.readSensor();
  int fire1 = fs1.readSensor();
  int fire2 = fs2.readSensor();
  Humiture ha = dht11A.readSensor();

  if(fs0.isDetected() || fs1.isDetected() || fs2.isDetected()) {
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
  delay(RATE);
}
