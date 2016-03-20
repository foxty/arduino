#include <SoftwareSerial.h>

// right wheel
#define PIN_IN1 2
#define PIN_IN2 4

// left wheel
#define PIN_IN3 5
#define PIN_IN4 7

#define PIN_RX 10
#define PIN_TX 9

SoftwareSerial blueTooth(PIN_RX, PIN_TX);

enum Direction {Forward, Stop, Backward};

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);
  Serial.begin(9600);
  Serial.println("Motors Setup.");
  setupBlueToothConnection();
  delay(500);
}

void setupBlueToothConnection()
{
  Serial.println("Setting Bluetooth parameters");
  blueTooth.begin(9600); //Set BluetoothBee BaudRate to default baud rate 38400
  delay(100);
  sendBlueToothCommand("AT\r\n");
  sendBlueToothCommand("AT+NAME=HC-ISAAC-05\r\n");//命名模块名
  sendBlueToothCommand("AT+ROLE=0\r\n");//设置主从模式：0从机，1主机
  sendBlueToothCommand("AT+PSWD=0000\r\n");//设置配对密码，如0123
  sendBlueToothCommand("AT+UART=9600,0,0\r\n");//设置波特率9600，停止位1，校验位无
  //sendBlueToothCommand("AT+RMAAD\r\n");//清空配对列表
  delay(100);
  Serial.println("Setup complete");
}

void sendBlueToothCommand(char command[])
{
  char a;
  blueTooth.print(command);
  Serial.print(command);
  delay(100);
  while (blueTooth.available()) {
    Serial.print(char(blueTooth.read()));
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  while (blueTooth.available()) {
    char c = (char(blueTooth.read()));
    Serial.println(c);
    switch (c) {
      case 'F':
      case 'f':
        forward(100);
        break;
      case 'B':
      case 'b':
        backward(100);
        break;
      case 'L':
      case 'l':
        left(100);
        break;
      case 'R':
      case 'r':
        right(100);
        break;
      case 'S':
      case 's':
        stop();
        break;
    }
  }
}

/*
   Run left weehl
   @Direction
   @Speed
*/
void runLeftWheel(Direction dir, int speed) {
  switch (dir) {
    case Forward:
      digitalWrite(PIN_IN3, HIGH);
      digitalWrite(PIN_IN4, LOW);
      break;
    case Stop:
      digitalWrite(PIN_IN3, LOW);
      digitalWrite(PIN_IN4, LOW);
      break;
    case Backward:
      digitalWrite(PIN_IN3, LOW);
      digitalWrite(PIN_IN4, HIGH);
      break;
  }
}

void runRightWheel(Direction dir, int speed) {
  switch (dir) {
    case Forward:
      digitalWrite(PIN_IN1, HIGH);
      digitalWrite(PIN_IN2, LOW);
      break;
    case Stop:
      digitalWrite(PIN_IN1, LOW);
      digitalWrite(PIN_IN2, LOW);
      break;
    case Backward:
      digitalWrite(PIN_IN1, LOW);
      digitalWrite(PIN_IN2, HIGH);
      break;
  }

}

void forward(int speed) {
  runLeftWheel(Forward, speed);
  runRightWheel(Forward, speed);
}

void backward(int speed) {
  runLeftWheel(Backward, speed);
  runRightWheel(Backward, speed);
}

void left(int speed) {
  runLeftWheel(Stop, speed);
  runRightWheel(Forward, speed);
}

void right(int speed) {
  runLeftWheel(Forward, speed);
  runRightWheel(Stop, speed);
}

void stop() {
  runLeftWheel(Stop, 0);
  runRightWheel(Stop, 0);
}

