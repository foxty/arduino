/**
 * 利用LM35传感器测试温度,超过高温阀值则出红色LED警报,低于低温阀值则发出蓝色LED警报.同时串联了蜂鸣器告警.
 * 
 *  - LM35中间信号线接入到A0针脚
 *  - 红色LED接入2
 *  - 蓝色LED接入3
 *  
 */
const int PIN_LM35 = A0;
const int COLD_TEMP = 10;
const int HOT_TEMP = 20;
const int PIN_LED_BLUE = 2;
const int PIN_LED_RED = 3;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(PIN_LED_BLUE, OUTPUT);
  pinMode(PIN_LED_RED, OUTPUT);
  delay(2000);  //dela and wait the sensor ready
  Serial.println("GO!");
}

void alarm(int pin, int duration) {
  digitalWrite(pin, HIGH);
  delay(duration);
  digitalWrite(pin, LOW);
}

void loop() {
  // put your main code here, to run repeatedly
  int val = analogRead(PIN_LM35);
  float temp = (val*0.0048828125*100); 
  if(temp > HOT_TEMP) {
    alarm(PIN_LED_RED, 200);
  } else if(temp < COLD_TEMP) {
    alarm(PIN_LED_BLUE, 200);
  } else {
    digitalWrite(PIN_LED_BLUE, LOW);
    digitalWrite(PIN_LED_RED, LOW);
  }
  Serial.print(val);
  Serial.print(" -> ");
  Serial.println(temp);
  delay(2000);
}
