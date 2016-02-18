#include <DHT.h>

/*
 * DHT11温湿度计获取温湿度
 * 
 * 目的:通过读取DHT11模块的信号数据来读取温湿度数据并输出到串口
 * 
 * 前提:GND接地,VCC接入5v电源,信号接入到PIN 8
 * 
 */

#define PIN_DHT 8
#define DHTTYPE DHT11

DHT dht(PIN_DHT, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if(isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;  
  }
  
  Serial.print("Humidity = ");
  Serial.print(h);
  Serial.print(", Temp = ");
  Serial.println(t);
}
