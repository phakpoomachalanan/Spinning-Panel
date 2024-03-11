#include "WiFi.h"
#include <esp_now.h>

uint8_t ldrSender[] = { 0x3C, 0x61, 0x05, 0x03, 0xD5, 0xCC };          //PAN
uint8_t displayReceiver[] = { 0x24, 0x6F, 0x28, 0x28, 0x99, 0x9C };    // PONG
uint8_t directionReceiver[] = { 0xE8, 0xDB, 0x84, 0x00, 0xFB, 0xC4 };  // KIT
uint8_t bothReceiver[] = { 0xE8, 0x68, 0xE7, 0x22, 0xB0, 0xE4 };       // NM

typedef struct ldrReceive {
  int ldr1;
  int ldr2;
  int ldr3;
  int ldr4;
} ldrReceive;

typedef struct displaySend {
  float latestGenElec;
  float genElec;
  float saveMoney;
} displaySend;

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
displaySend displayS;
directionSend dirS;
bothSend bothS;

static const int diff = 1000;

int x = 0;
int y = 0;

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void OnDataReceive(const uint8_t *mac_addr, const uint8_t *incomingData, int len) {
  if (mac_addr[0] == ldrSender[0] && mac_addr[1] == ldrSender[1] && mac_addr[2] == ldrSender[2] && mac_addr[3] == ldrSender[3] && mac_addr[4] == ldrSender[4] && mac_addr[5] == ldrSender[5]) {

    Serial.println("Receive Success");

    displayS.latestGenElec = 0;
    displayS.saveMoney = 0;
    dirS.x = 0;
    dirS.y = 0;
    bothS.ldr = 0;
    bothS.x = 0;
    bothS.y = 0;

    memcpy(&ldrR, incomingData, sizeof(ldrR));
    x = 0;
    y = 0;
    // avg ldr
    if (ldrR.ldr1 < 0 || ldrR.ldr2 < 0 || ldrR.ldr3 < 0 || ldrR.ldr4 < 0) {
      return;
    }
    int temp = (ldrR.ldr1 + ldrR.ldr2 + ldrR.ldr3 + ldrR.ldr4) / 4;
    bothS.ldr = temp;

    // get direction
    if (ldrR.ldr1 >= ldrR.ldr2 && ldrR.ldr1 >= ldrR.ldr3 && ldrR.ldr1 >= ldrR.ldr4) {
      x = 45;
      if (ldrR.ldr2 > ldrR.ldr4) {
        if (ldrR.ldr1 - ldrR.ldr2 < diff) {
          x = (90 - (float(ldrR.ldr1 - ldrR.ldr2) / diff * 45));
        }
      } else {
        if (ldrR.ldr1 - ldrR.ldr4 < diff) {
          x = ((float(ldrR.ldr1 - ldrR.ldr4) / diff) * 45);
        }
      }
      if (ldrR.ldr1 - ldrR.ldr3 < diff) {
        y = (float(ldrR.ldr1 - ldrR.ldr3) / diff * 90);
      } else {
        y = 90;
      }
    } else if (ldrR.ldr2 >= ldrR.ldr1 && ldrR.ldr2 >= ldrR.ldr3 && ldrR.ldr2 >= ldrR.ldr4) {
      x = 135;
      if (ldrR.ldr3 > ldrR.ldr1) {
        if (ldrR.ldr2 - ldrR.ldr3 < diff) {
          x = 180 - (float(ldrR.ldr2 - ldrR.ldr3) / diff * 45);
        }
      } else {
        if (ldrR.ldr2 - ldrR.ldr1 < diff) {
          x = 90 + ((float(ldrR.ldr2 - ldrR.ldr1) / diff * 45));
        }
      }
      if (ldrR.ldr2 - ldrR.ldr4 < diff) {
        y = (float(ldrR.ldr2 - ldrR.ldr4) / diff * 90);
      } else {
        y = 90;
      }
    } else if (ldrR.ldr3 >= ldrR.ldr1 && ldrR.ldr3 >= ldrR.ldr2 && ldrR.ldr3 >= ldrR.ldr4) {
      x = 225;
      if (ldrR.ldr4 > ldrR.ldr2) {
        if (ldrR.ldr3 - ldrR.ldr4 < diff) {
          x = 270 - (float(ldrR.ldr3 - ldrR.ldr4) / diff * 45);
        }
      } else {
        if (ldrR.ldr3 - ldrR.ldr2 < diff) {
          x = 180 + ((float(ldrR.ldr3 - ldrR.ldr2) / diff * 45));
        }
      }
      if (ldrR.ldr3 - ldrR.ldr1 < diff) {
        y = (float(ldrR.ldr3 - ldrR.ldr1) / diff * 90);
      } else {
        y = 90;
      }
    } else {
      x = 315;
      if (ldrR.ldr1 > ldrR.ldr3) {
        if (ldrR.ldr4 - ldrR.ldr1 < diff) {
          x = 360 - (float(ldrR.ldr4 - ldrR.ldr3) / diff * 45);
        }
      } else {
        if (ldrR.ldr4 - ldrR.ldr3 < diff) {
          x = 270 + ((float(ldrR.ldr4 - ldrR.ldr3) / diff * 45));
        }
      }
      if (ldrR.ldr4 - ldrR.ldr2 < diff) {
        y = (float(ldrR.ldr4 - ldrR.ldr2) / diff * 90);
      } else {
        y = 90;
      }
    }
    y = 90 - y;

    bothS.x = x;
    bothS.y = y;
    dirS.x = x;
    dirS.y = y;

    Serial.println(temp);
    Serial.println(x);
    Serial.println(y);

    displayS.latestGenElec = (float)temp * 5 / 4096;
    displayS.genElec += displayS.latestGenElec / 3600;
    displayS.saveMoney = displayS.genElec * 7.2;

    esp_err_t result1;
    esp_err_t result2;
    esp_err_t result3;

    result1 = esp_now_send(displayReceiver, (uint8_t *)&displayS, sizeof(displaySend));
    result2 = esp_now_send(directionReceiver, (uint8_t *)&dirS, sizeof(directionSend));
    result3 = esp_now_send(bothReceiver, (uint8_t *)&bothS, sizeof(bothSend));

    if (result1 == ESP_OK) {
      Serial.println("1: Sending confirmed");
    } else {
      Serial.println("1: Sending error");
    }

    if (result2 == ESP_OK) {
      Serial.println("2: Sending confirmed");
    } else {
      Serial.println("2: Sending error");
    }

    if (result3 == ESP_OK) {
      Serial.println("3: Sending confirmed");
    } else {
      Serial.println("3: Sending error");
    }
  }
}

void setup() {
  Serial.begin(115200);

  displayS.latestGenElec = 0;
  displayS.genElec = 0;
  displayS.saveMoney = 0;
  dirS.x = 0;
  dirS.y = 0;
  bothS.ldr = 0;
  bothS.x = 0;
  bothS.y = 0;

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataReceive);

  peerInfo.channel = 1;
  peerInfo.encrypt = false;

  memcpy(peerInfo.peer_addr, ldrSender, sizeof(ldrReceive));
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add Pong");
    return;
  }

  memcpy(peerInfo.peer_addr, displayReceiver, sizeof(displaySend));
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add Pong");
    return;
  }

  memcpy(peerInfo.peer_addr, directionReceiver, sizeof(directionSend));
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add Kit");
    return;
  }

  memcpy(peerInfo.peer_addr, bothReceiver, sizeof(bothSend));
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add Nammon");
    return;
  }
}

void loop() {
}