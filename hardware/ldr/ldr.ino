

// credit: https://randomnerdtutorials.com/esp32-servo-motor-web-server-arduino-ide/
// req: ServoESP32 V 1.0.3
#include "WiFi.h"
#include <esp_now.h>
static const int ldr1 = 36;
static const int ldr2 = 39;
static const int ldr3 = 34;
static const int ldr4 = 35;


uint8_t broadcastAddress[] = {0xE8, 0xDB, 0x84, 0x00, 0xFB, 0xC4};
int x=0;
int y=0;
typedef struct struct_massage {
  int x;
  int y;
} struct_massage;

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
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  x=0;
  y=0;
  int ldrVal1 = analogRead(ldr1);
 
  int ldrVal2 = analogRead(ldr2);

  int ldrVal3 = analogRead(ldr3);
  
  int ldrVal4 = analogRead(ldr4);
  printf("[%d][%d][%d][%d]",ldrVal1,ldrVal2,ldrVal3,ldrVal4);
  esp_err_t result;
  if(ldrVal1 >= ldrVal2 && ldrVal1 >= ldrVal3 && ldrVal1 >= ldrVal4) {
    if(ldrVal2 > ldrVal4) {
      if(ldrVal1 - ldrVal2 < diff){
        x=(90-(float(ldrVal1 - ldrVal2) / diff *45));
      }
    } else {
      if(ldrVal1 - ldrVal4 < diff){
        x=((float(ldrVal1 - ldrVal4) / diff) *45);
      }
    }
    if(ldrVal1- ldrVal3 < diff){
      y=(float(ldrVal1 - ldrVal3) / diff *90);
    } else {
      y=90;
    }
  } else if(ldrVal2 >= ldrVal1 && ldrVal2 >= ldrVal3 && ldrVal2 >= ldrVal4) {
    if(ldrVal3 > ldrVal1) {
      if(ldrVal2 - ldrVal3 < diff){
        x=180-(float(ldrVal2 - ldrVal3) / diff *45);
      }
    } else {
      if(ldrVal2 - ldrVal1 < diff){
        x=90+((float(ldrVal2 - ldrVal1) / diff *45));
      }
    }
    if(ldrVal2- ldrVal4 < diff){
      y=(float(ldrVal2 - ldrVal4) / diff *90);
    } else {
      y=90;
    }
  } else if(ldrVal3 >= ldrVal1 && ldrVal3 >= ldrVal2 && ldrVal3 >= ldrVal4) {
    if(ldrVal4 > ldrVal2) {
      if(ldrVal3 - ldrVal4 < diff){
        x=270-(float(ldrVal3 - ldrVal4) / diff *45);
      }
    } else {
      if(ldrVal2 - ldrVal1 < diff){
        x=180+((float(ldrVal3 - ldrVal2) / diff *45));
      }
    }
    if(ldrVal3- ldrVal1 < diff){
      y=(float(ldrVal3 - ldrVal1) / diff *90);
    } else {
      y=90;
    }
  } else {
    if(ldrVal1 > ldrVal3) {
      if(ldrVal4 - ldrVal1 < diff){
        x=360-(float(ldrVal4 - ldrVal3) / diff *45);
      }
    } else {
      if(ldrVal4 - ldrVal3 < diff){
        x=270+((float(ldrVal4 - ldrVal3) / diff *45));
      }
    }
    if(ldrVal4- ldrVal2 < diff){
      y=(float(ldrVal4 - ldrVal2) / diff *90);
    } else {
      y=90;
    }
  }
  y = 90-y;
  printf("[%d, %d]\n", x, y);
  Deg.x = x;
  Deg.y = y;
  result = esp_now_send(broadcastAddress, (uint8_t *) &Deg, sizeof(Deg));
  if (result == ESP_OK) {
    Serial.println("Sending confirmed");
  }
  else {
    Serial.println("Sending error");
  }
  delay(700);

}