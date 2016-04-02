/*
 * 蜂鸣器试验
 * 
 * 目的:每隔2秒钟响500毫秒,响5次之后停止,5秒之后继续重复.
 *    
 * 前提条件: 一边接GND,另一边接入pin7
 *    
 */
const int PIN_BUZZER = 7;
int loops = 0;
boolean beep = true;

void setup() {
  pinMode(PIN_BUZZER, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  loops ++;
  delay(2000);
  if(beep) {
    digitalWrite(PIN_BUZZER, HIGH);
    delay(500);
    digitalWrite(PIN_BUZZER, LOW);
  }

  if(loops % 5 == 0) {
    beep = !beep;
  }
}
