#include <DHT.h>

/*
   DHT11温湿度计获取温湿度

   目的:通过读取DHT11模块的信号数据来读取温湿度数据并输出到串口

   前提:GND接地,VCC接入5v电源,信号接入到PIN 8

*/

#define PIN_DHT 8
#define DHTTYPE DHT11
struct Humiture {
  float humidity;
  float temperature;
};

class HumitureDHT11 {
  public:
    HumitureDHT11(int pin):dht(pin, DHT11) {
      inputPin = pin;
      pinMode(inputPin, INPUT);
      (dht).begin();
    }

    Humiture readSensor() {
      float humidity = (dht).readHumidity();
      float temperature = (dht).readTemperature();
      humiture.humidity = humidity;
      humiture.temperature = temperature;
      if (isnan(humidity)) {
        Serial.println("Failed to read humidity from DHT sensor!");
        humiture.humidity = -1;
      }
      if (isnan(temperature)) {
        Serial.println("Failed to read temperature from DHT sensor!");
        humiture.temperature = -1;
      }
      return humiture;
    }

  private:
    int inputPin;
    DHT dht;
    Humiture humiture;
};

//DHT dht(PIN_DHT, DHTTYPE);
HumitureDHT11 dht11(PIN_DHT);

void setup() {
  Serial.begin(9600);
  //dht.begin();
  Serial.println("GO!");
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  Serial.print("LOOP:");
  /*float h = dht.readHumidity();
    float t = dht.readTemperature();

    if(isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
    }*/

  Humiture h = dht11.readSensor();
  h.humidity = h.humidity;
  h.temperature = h.temperature;
  Serial.print("Humidity = ");
  Serial.print(h.humidity);
  Serial.print(", Temp = ");
  Serial.println(h.temperature);
}
