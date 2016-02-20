#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

class TempratureLM35 {
  public:
    TempratureLM35(int pin) {
      inputPin = pin;
      pinMode(pin, INPUT);
    }

    float readSensor() {
      int val = analogRead(inputPin);
      float temp = (val*0.0048828125*100); 
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
 * 利用红外三极管探测火焰,检测到则出发红色LED报警
 *  
 *  - LM35 信号接A0 
 *  - Flame Sensor 正极(长针脚)接电源, 负极(短针脚)接10k电阻, 10k电阻另一端接地,Flame Sensor和10k电阻之间信号接入A1
 *  - 红色LED接入3,蓝色LED接4
 */

const int PIN_LM35 = A0;
const int PIN_FLAME = A1;
const int PIN_LED_BLUE = 2;
const int PIN_LED_RED = 3;

LiquidCrystal_I2C lcd(0x3F,16,2); 
TempratureLM35 lm35(A0);
FlameSensor fs(A1, 5);

// show alarm in LED 
void alarm(int pin, int duration) {
  digitalWrite(pin, HIGH);
  delay(duration);
  digitalWrite(pin, LOW);
}

void setup() {
  // Init LCD
  lcd.init(); // initialize the lcd 
  lcd.backlight(); //Open the backlight
  lcd.print("Octopus"); // Print a message to the LCD.
  lcd.setCursor(0,1); //newline
  lcd.print("Starting......");// Print a message to the LCD

  pinMode(PIN_LED_BLUE, OUTPUT);
  pinMode(PIN_LED_RED, OUTPUT);  
  delay(500);  //dela and wait the sensor ready
}

void loop() {
  lcd.clear();

  int fire = fs.readSensor();
  if (fs.isOverThreshold()) {
    alarm(PIN_LED_RED, 200);
  }
  int temp = lm35.readSensor();
  
  lcd.print("F:");
  lcd.print(fire);
  lcd.print(",T:");
  lcd.print(temp);
  lcd.print("");
  lcd.setCursor(0,1);
  lcd.print("...");
  delay(1000);
}
