// credit: https://randomnerdtutorials.com/esp32-servo-motor-web-server-arduino-ide/
// req: ServoESP32 V 1.0.3
#include "WiFi.h"
#include <esp_now.h>
static const int ldr1 = 36;
static const int ldr2 = 39;
static const int ldr3 = 34;
static const int ldr4 = 35;


uint8_t broadcastAddress1[] = {0xE8, 0xDB, 0x84, 0x00, 0xFB, 0xC4};
uint8_t broadcastAddress2[] = {0xE8, 0x68, 0xE7, 0x22, 0xB0, 0xE4};
int x=0;
int y=0;

typedef struct struct_massage {
  int x;
  int y;
} struct_massage;

typedef struct struct_massage2 {
  int ldr1;
  int ldr2;
  int ldr3;
  int ldr4;
} struct_massage2;

struct_massage2 ldr;
struct_massage Deg;

static const int diff = 1000;


esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  // Set up Serial Monitor
  Serial.begin(115200);
 
  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
 
  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
 
  // Register the send callback
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress1, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  memcpy(peerInfo2.peer_addr, broadcastAddress2, 6);
  peerInfo2.channel = 1;  
  peerInfo2.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer1");
    return;
  }
  if (esp_now_add_peer(&peerInfo2) != ESP_OK){
    Serial.println("Failed to add peer2");
    return;
  }
}

void loop() {
  x=0;
  y=0;
  int ldr.ldr1 = analogRead(ldr1);
  
  int ldr.ldr2 = analogRead(ldr2);

  int ldr.ldr3 = analogRead(ldr3);
  
  int ldr.ldr4 = analogRead(ldr4);
  printf("[%d][%d][%d][%d]",ldr.ldr1,ldr.ldr2,ldr.ldr3,ldr.ldr4);
  esp_err_t result;
  if(ldr.ldr1 >= ldr.ldr2 && ldr.ldr1 >= ldr.ldr3 && ldr.ldr1 >= ldr.ldr4) {
    if(ldr.ldr2 > ldr.ldr4) {
      if(ldr.ldr1 - ldr.ldr2 < diff){
        x=(90-(float(ldr.ldr1 - ldr.ldr2) / diff *45));
      }
    } else {
      if(ldr.ldr1 - ldr.ldr4 < diff){
        x=((float(ldr.ldr1 - ldr.ldr4) / diff) *45);
      }
    }
    if(ldr.ldr1- ldr.ldr3 < diff){
      y=(float(ldr.ldr1 - ldr.ldr3) / diff *90);
    } else {
      y=90;
    }
  } else if(ldr.ldr2 >= ldr.ldr1 && ldr.ldr2 >= ldr.ldr3 && ldr.ldr2 >= ldr.ldr4) {
    if(ldr.ldr3 > ldr.ldr1) {
      if(ldr.ldr2 - ldr.ldr3 < diff){
        x=180-(float(ldr.ldr2 - ldr.ldr3) / diff *45);
      }
    } else {
      if(ldr.ldr2 - ldr.ldr1 < diff){
        x=90+((float(ldr.ldr2 - ldr.ldr1) / diff *45));
      }
    }
    if(ldr.ldr2- ldr.ldr4 < diff){
      y=(float(ldr.ldr2 - ldr.ldr4) / diff *90);
    } else {
      y=90;
    }
  } else if(ldr.ldr3 >= ldr.ldr1 && ldr.ldr3 >= ldr.ldr2 && ldr.ldr3 >= ldr.ldr4) {
    if(ldr.ldr4 > ldr.ldr2) {
      if(ldr.ldr3 - ldr.ldr4 < diff){
        x=270-(float(ldr.ldr3 - ldr.ldr4) / diff *45);
      }
    } else {
      if(ldr.ldr2 - ldr.ldr1 < diff){
        x=180+((float(ldr.ldr3 - ldr.ldr2) / diff *45));
      }
    }
    if(ldr.ldr3- ldr.ldr1 < diff){
      y=(float(ldr.ldr3 - ldr.ldr1) / diff *90);
    } else {
      y=90;
    }
  } else {
    if(ldr.ldr1 > ldr.ldr3) {
      if(ldr.ldr4 - ldr.ldr1 < diff){
        x=360-(float(ldr.ldr4 - ldr.ldr3) / diff *45);
      }
    } else {
      if(ldr.ldr4 - ldr.ldr3 < diff){
        x=270+((float(ldr.ldr4 - ldr.ldr3) / diff *45));
      }
    }
    if(ldr.ldr4- ldr.ldr2 < diff){
      y=(float(ldr.ldr4 - ldr.ldr2) / diff *90);
    } else {
      y=90;
    }
  }
  y = 90-y;
  printf("[%d, %d]\n", x, y);
  Deg.x = x;
  Deg.y = y;
  result1 = esp_now_send(broadcastAddress1, (uint8_t *) &Deg, sizeof(Deg));
  result2 = esp_now_send(broadcastAddress2, (uint8_t *) &ldr, sizeof(ldr));
  if (result1 == ESP_OK && result2 == ESP_OK) {
    Serial.println("Sending confirmed");
  }
  else {
    Serial.println("Sending error");
  }
  delay(1000);

}