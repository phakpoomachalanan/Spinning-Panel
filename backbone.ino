#include "WiFi.h"
#include <esp_now.h>

uint8_t ldrSender[] = { 0x3C, 0x61, 0x05, 0x03, 0xD5, 0xCC };          //PAN
uint8_t ldrReceiver[] = { 0x24, 0x6F, 0x28, 0x28, 0x99, 0x9C };          // PONG
uint8_t directionReceiver[] = { 0xE8, 0xDB, 0x84, 0x00, 0xFB, 0xC4 };  // KIT
uint8_t bothReceiver[] = { 0xE8, 0x68, 0xE7, 0x22, 0xB0, 0xE4 };       // NM

typedef struct ldrReceive {
  int ldr1;
  int ldr2;
  int ldr3;
  int ldr4;
} ldrReceive;

typedef struct ldrSend {
  int ldr;
} ldrSend;

typedef struct directionSend {
  int x;
  int y;
} directionSend;

typedef struct bothSend {
  int ldr;
  int x;
  int y;
} bothSend;

ldrReceive ldrR;
ldrSend ldrS;
directionSend dirS;
bothSend bothS;

static const int diff = 1000;

int x = 0;
int y = 0;

esp_now_peer_info_t pan;
esp_now_peer_info_t pong;
esp_now_peer_info_t kit;
esp_now_peer_info_t nammon;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void OnDataReceive(const uint8_t *mac_addr, const uint8_t *incomingData, int len) {
  Serial.println("Receive Success");

  memcpy(&ldrR, incomingData, sizeof(ldrR));
  x = 0;
  y = 0;

  // avg ldr
  int temp = (ldrR.ldr1 + ldrR.ldr2 + ldrR.ldr3 + ldrR.ldr4) >> 2;
  ldrS.ldr = temp;
  bothS.ldr = temp;

  // get direction
  if(ldrR.ldr1 >= ldrR.ldr2 && ldrR.ldr1 >= ldrR.ldr3 && ldrR.ldr1 >= ldrR.ldr4) {
    x=45;
    if(ldrR.ldr2 > ldrR.ldr4) {
      if(ldrR.ldr1 - ldrR.ldr2 < diff){
        x=(90-(float(ldrR.ldr1 - ldrR.ldr2) / diff *45));
      }
    } else {
      if(ldrR.ldr1 - ldrR.ldr4 < diff){
        x=((float(ldrR.ldr1 - ldrR.ldr4) / diff) *45);
      }
    }
    if(ldrR.ldr1- ldrR.ldr3 < diff){
      y=(float(ldrR.ldr1 - ldrR.ldr3) / diff *90);
    } else {
      y=90;
    }
  } else if(ldrR.ldr2 >= ldrR.ldr1 && ldrR.ldr2 >= ldrR.ldr3 && ldrR.ldr2 >= ldrR.ldr4) {
    x=135;
    if(ldrR.ldr3 > ldrR.ldr1) {
      if(ldrR.ldr2 - ldrR.ldr3 < diff){
        x=180-(float(ldrR.ldr2 - ldrR.ldr3) / diff *45);
      }
    } else {
      if(ldrR.ldr2 - ldrR.ldr1 < diff){
        x=90+((float(ldrR.ldr2 - ldrR.ldr1) / diff *45));
      }
    }
    if(ldrR.ldr2- ldrR.ldr4 < diff){
      y=(float(ldrR.ldr2 - ldrR.ldr4) / diff *90);
    } else {
      y=90;
    }
  } else if(ldrR.ldr3 >= ldrR.ldr1 && ldrR.ldr3 >= ldrR.ldr2 && ldrR.ldr3 >= ldrR.ldr4) {
    x=225;
    if(ldrR.ldr4 > ldrR.ldr2) {
      if(ldrR.ldr3 - ldrR.ldr4 < diff){
        x=270-(float(ldrR.ldr3 - ldrR.ldr4) / diff *45);
      }
    } else {
      if(ldrR.ldr3 - ldrR.ldr2 < diff){
        x=180+((float(ldrR.ldr3 - ldrR.ldr2) / diff *45));
      }
    }
    if(ldrR.ldr3- ldrR.ldr1 < diff){
      y=(float(ldrR.ldr3 - ldrR.ldr1) / diff *90);
    } else {
      y=90;
    }
  } else {
    x=315;
    if(ldrR.ldr1 > ldrR.ldr3) {
      if(ldrR.ldr4 - ldrR.ldr1 < diff){
        x=360-(float(ldrR.ldr4 - ldrR.ldr3) / diff *45);
      }
    } else {
      if(ldrR.ldr4 - ldrR.ldr3 < diff){
        x=270+((float(ldrR.ldr4 - ldrR.ldr3) / diff *45));
      }
    }
    if(ldrR.ldr4- ldrR.ldr2 < diff){
      y=(float(ldrR.ldr4 - ldrR.ldr2) / diff *90);
    } else {
      y=90;
    }
  }
  y = 90-y;

  bothS.x = x;
  bothS.y = y;
  dirS.x = x;
  dirS.y = y;

  Serial.println(temp);
  Serial.println(x);
  Serial.println(y);
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataReceive);

  memcpy(pan.peer_addr, ldrSender, sizeof(ldrReceive));
  pan.channel = 0;
  pan.encrypt = false;
  memcpy(pong.peer_addr, ldrReceiver, sizeof(ldrSend));
  pong.channel = 0;
  pong.encrypt = false;
  memcpy(kit.peer_addr, directionReceiver, sizeof(dirS));
  kit.channel = 0;
  kit.encrypt = false;
  memcpy(nammon.peer_addr, bothReceiver, sizeof(bothSend));
  nammon.channel = 1;
  nammon.encrypt = false;

  ldrS.ldr = 0;
  dirS.x = 0;
  dirS.y = 0;
  bothS.ldr = 0;
  bothS.x = 0;
  bothS.y = 0;

  if (esp_now_add_peer(&pong) != ESP_OK){
    Serial.println("Failed to add pong");
    return;
  }

  if (esp_now_add_peer(&kit) != ESP_OK){
    Serial.println("Failed to add kit");
    return;
  }

  if (esp_now_add_peer(&nammon) != ESP_OK){
    Serial.println("Failed to add nammon");
    return;
  }
}

void loop() {
  esp_err_t result1;
  esp_err_t result2;
  esp_err_t result3;

  // result1 = esp_now_send(ldrReceiver, (uint8_t *)&ldrS, sizeof(ldrS));
  // result2 = esp_now_send(directionReceiver, (uint8_t *)&dirS, sizeof(directionSend));
  result3 = esp_now_send(bothReceiver, (uint8_t *)&bothS, sizeof(bothSend));

  if (result2 == ESP_OK) {
    Serial.println("Sending confirmed");
  } else {
    Serial.println("Sending error");
  }
  delay(1000);
}