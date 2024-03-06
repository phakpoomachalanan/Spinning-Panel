#include <esp_now.h>
#include "WiFi.h"
#include <Wire.h>
#include <ezButton.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define BUTTON_PIN 23
ezButton button(BUTTON_PIN);

#define OLED_RESET 16
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(OLED_RESET);

typedef struct receiveValue {
  float latestGenElec;
  float genElec;
  float saveMoney;
} receiveValue;

receiveValue recvVal;

void OnDataReceive(const uint8_t *mac_addr, const uint8_t *incomingData, int len) {
  memcpy(&recvVal, incomingData, sizeof(receiveValue));
}

void setup() {
  Serial.begin(9600);

  recvVal.latestGenElec = 0;
  recvVal.genElec = 0;
  recvVal.saveMoney = 0;

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_register_recv_cb(OnDataReceive);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.display();
  delay(1000);
}

void loop() {
  button.loop();

  switch (button.getCount() % 3) {
  case 0:
      displayAverage();
      break;
    case 1:
      displayGeneratedElectricity();
      break;
    case 2:
      displayMoneySave();
      break;
    default:
      break;
  }
}

void displayAverage() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0, 0);
  display.println("Avg. gen. electricity");
  display.printf("%.2f kWh", recvVal.latestGenElec);

  display.display();
  delay(100);
}

void displayGeneratedElectricity() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0, 0);
  display.println("Generated electricity");
  display.printf("%.2f kWh", recvVal.genElec);

  display.display();
  delay(100);
}

void displayMoneySave() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0, 0);
  display.println("Money Save");
  display.printf("%.2f Baht", recvVal.saveMoney);

  display.display();
  delay(100);
}
