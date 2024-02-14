#include <Servo.h>

// credit: https://randomnerdtutorials.com/esp32-servo-motor-web-server-arduino-ide/
// req: ServoESP32 V 1.0.3

static const int servoPin = 13;

Servo servo1;

void setup() {
  Serial.begin(115200);
  servo1.attach(servoPin);
}

void loop() {
  for(int posDegrees = 0; posDegrees <= 180; posDegrees++) {
    if(posDegrees%10 == 0) {
      Serial.println(posDegrees);
    }
    servo1.write(posDegrees);
    delay(50);
  }

  for(int posDegrees = 180; posDegrees >= 0; posDegrees--) {
    if(posDegrees%10 == 0) {
      Serial.println(posDegrees);
    }
    servo1.write(posDegrees);
    delay(50);
  }
}