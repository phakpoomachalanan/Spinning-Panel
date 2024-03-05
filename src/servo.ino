#include <Servo.h>
#include <esp_now.h>
#include <WiFi.h>

// credit: https://randomnerdtutorials.com/esp32-servo-motor-web-server-arduino-ide/
// req: ServoESP32 V 1.0.3

static const int servoPin1 = 13;
static const int servoPin2 = 12;
static const int delayTime = 50;
Servo servo1;
Servo servo2;

struct angle {
  int x, y;
};

// for received data
esp_now_peer_info_t peerInfo;
angle data; 



void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&data, incomingData, sizeof(data));
  Serial.print("Data received: \n");
  Serial.printf("    x: %d\n",data.x);
  Serial.printf("    y: %d\n",data.y);
}

void setup() {
  Serial.begin(115200);
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
  WiFi.mode(WIFI_STA);
    // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
    for(int posDegrees = 0; posDegrees <= 180; posDegrees+=90) {
      if(posDegrees%10 == 0) {
        Serial.println(posDegrees);
      }
      servo1.write(posDegrees);
      for (int j = 0; j <= 180; j+=90) {
        servo2.write(j);
        delay(2500);
      }
  }
}