

int switchPin = A5;
int ledPin = 11;
int switchValue = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(switchPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600); 
}

void loop() {
  // put your main code here, to run repeatedly:
  switchValue = analogRead(switchPin);
  analogWrite(ledPin, switchValue/4);
}
