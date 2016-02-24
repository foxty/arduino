#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OCT_Sensor.h>
//#include <SPI.h>
//#include "nRF24L01.h"
//#include "RF24.h"

/**
   利用红外三极管探测火焰,检测到则出发红色LED报警

    - LM35 信号接A0
    - DHT11 GND接地,VCC接入5v电源,信号接入到PIN 8
    - Flame Sensor 正极(长针脚)接电源, 负极(短针脚)接10k电阻, 10k电阻另一端接地,Flame Sensor和10k电阻之间信号接入A1
    - 红色LED接入3,蓝色LED接4
*/  

const int RATE = 2000;
//const uint64_t pipe = 0xE8E8F0F0E1LL;

//blue is 0x27, green is 0x3F
LiquidCrystal_I2C lcd(0x3F, 16, 2);
HumitureDHT11 dht11A(6);
FlameSensor fs0(FLAME_SENSOR_ANALOG, A0, 200);
FlameSensor fs1(FLAME_SENSOR_ANALOG, A1, 200);
FlameSensor fs2(FLAME_SENSOR_ANALOG, A2, 200);
//RF24 radio(7, 8);

void setup() {
  // Init LCD
  lcd.init(); // initialize the lcd
  lcd.noBacklight(); //Close the backlight
  lcd.autoscroll();
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

  lcd.print("H");
  lcd.print((int)ha.humidity);
  lcd.print(",T");
  lcd.print((int)ha.temperature);

  lcd.setCursor(0, 1);
  lcd.print(fire0);
  lcd.print(",");
  lcd.print(fire1);
  lcd.print(",");
  lcd.print(fire2);
  Serial.print("loop");
  Serial.print(ha.humidity);

  delay(RATE);
}
