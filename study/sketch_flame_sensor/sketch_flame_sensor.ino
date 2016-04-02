
/**
 * 利用红外三极管探测火焰,检测到则出发红色LED报警
 * 
 *  - Flame Sensor 正极(长针脚)接电源
 *  - Flame Sensor 负极(短针脚)接10k电阻
 *  - 10k电阻另一端接地
 *  - Flame Sensor和10k电阻之间信号接入A1
 *  - 红色LED接入3,蓝色LED接4
 */

const int PIN_FLAME = A1;
const int PIN_LED_BLUE = 2;
const int PIN_LED_RED = 3;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(PIN_LED_BLUE, OUTPUT);
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_FLAME, INPUT);
  delay(2000);  //dela and wait the sensor ready
  Serial.println("GO!");
}


void alarm(int pin, int duration) {
  digitalWrite(pin, HIGH);
  delay(duration);
  digitalWrite(pin, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  int val = analogRead(PIN_FLAME);
  if (val > 0) {
    alarm(PIN_LED_RED, 200);
  }
  Serial.println(val);
  delay(500);
}
