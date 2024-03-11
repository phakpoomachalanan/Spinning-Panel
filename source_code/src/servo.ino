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
int degree1 ;
int degree2 ;

struct angle {
  int x, y;
};

// for received data
esp_now_peer_info_t peerInfo;
angle data; 

uint8_t sender[] = { 0x3C, 0x61, 0x05, 0x03, 0x41, 0xA0 };

void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
    if (mac_addr[0] == sender[0] && mac_addr[1] == sender[1] && mac_addr[2] == sender[2] && mac_addr[3] == sender[3] && mac_addr[4] == sender[4] && mac_addr[5] == sender[5]) {
    memcpy(&data, incomingData, sizeof(data));
  }
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
  if (data.x > 90 && data.x < 270) {
    degree1 = data.x - 90;
    degree2 = 180 - data.y ;
  } else {
    degree1 = (data.x + 90) % 360;
    degree2 = data.y;
  }

  Serial.printf("---------------------------------------\n ");
  Serial.printf("x: %d, degree_x: %d\n", data.x, degree1);
  Serial.printf("y: %d, degree_y: %d\n", data.y, degree2);
  
  servo1.write(degree1);
  servo2.write(degree2);

  delay(1000);
}