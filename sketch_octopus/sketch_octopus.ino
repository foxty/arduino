#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OCT_Sensor.h>

/**
   利用红外三极管探测火焰,检测到则出发红色LED报警

    - LM35 信号接A0
    - DHT11 GND接地,VCC接入5v电源,信号接入到PIN 8
    - Flame Sensor 正极(长针脚)接电源, 负极(短针脚)接10k电阻, 10k电阻另一端接地,Flame Sensor和10k电阻之间信号接入A1
    - 红色LED接入3,蓝色LED接4
*/  

const int RATE = 2000;

LiquidCrystal_I2C lcd(0x3F, 16, 2);
HumitureDHT11 dht11A(8);
FlameSensor fs(A1, 200);

void setup() {
  // Init LCD
  lcd.init(); // initialize the lcd
  lcd.backlight(); //Open the backlight
  lcd.print("     Octopus"); // Print a message to the LCD.

  Serial.begin(9600);
  delay(1000);  //dela and wait the sensor ready
}

void loop() {
  lcd.clear();

  int fire = fs.readSensor();
  //int temp = lm35.readSensor();
  Humiture ha = dht11A.readSensor();

  lcd.print("Ha");
  lcd.print((int)ha.humidity);
  lcd.print(",");
  lcd.print((int)ha.temperature);
  lcd.print(" F");
  lcd.print(fire);

  lcd.setCursor(0, 1);

  delay(RATE);
}
