#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

struct Humiture {
  float humidity;
  float temperature;
};

class HumitureDHT11 {
  public:
    HumitureDHT11(int pin):dht(pin, DHT11) {
      inputPin = pin;
      pinMode(inputPin, INPUT);
      dht.begin();
    }

    Humiture readSensor() {
      float humidity = dht.readHumidity();
      float temperature = dht.readTemperature();
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

class TempratureLM35 {
  public:
    TempratureLM35(int pin) {
      inputPin = pin;
      pinMode(pin, INPUT);
    }

    float readSensor() {
      int val = analogRead(inputPin);
      float temp = (val * 0.0048828125 * 100);
      return temp;
    }

  private:
    int inputPin;
};

class FlameSensor {
  public:
    FlameSensor(int pin, int t) {
      inputPin = pin;
      threshold = t;
      pinMode(inputPin, INPUT);
    }

    int readSensor() {
      return analogRead(inputPin);
    }

    boolean isOverThreshold() {
      int val = readSensor();
      return val >= threshold;
    }
  private:
    int inputPin;
    int threshold;
};

/**
   利用红外三极管探测火焰,检测到则出发红色LED报警

    - LM35 信号接A0
    - DHT11 GND接地,VCC接入5v电源,信号接入到PIN 8
    - Flame Sensor 正极(长针脚)接电源, 负极(短针脚)接10k电阻, 10k电阻另一端接地,Flame Sensor和10k电阻之间信号接入A1
    - 红色LED接入3,蓝色LED接4
*/

const int RATE = 2000;
//const int PIN_LM35 = A0;
const int PIN_FLAME = A1;
const int PIN_DHT11 = 8;

LiquidCrystal_I2C lcd(0x3F, 16, 2);
//TempratureLM35 lm35(A0);
HumitureDHT11 dht11(PIN_DHT11);
FlameSensor fs(A1, 200);

void setup() {
  // Init LCD
  lcd.init(); // initialize the lcd
  lcd.backlight(); //Open the backlight
  lcd.print("Octopus"); // Print a message to the LCD.
  lcd.setCursor(0, 1); //newline
  lcd.print("Starting......");// Print a message to the LCD

  Serial.begin(9600);
  delay(1000);  //dela and wait the sensor ready
}

void loop() {
  lcd.clear();

  int fire = fs.readSensor();
  //int temp = lm35.readSensor();
  Humiture h = dht11.readSensor();

  lcd.print("F");
  lcd.print(fire);
  //lcd.print(" T");
  //lcd.print(temp);
  lcd.print(" H");
  lcd.print((int)h.humidity);
  lcd.print(",");
  lcd.print((int)h.temperature);

  lcd.setCursor(0, 1);
  lcd.print("...");

  delay(RATE);
}
