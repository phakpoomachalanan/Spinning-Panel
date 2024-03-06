#include <WiFi.h>
#include <WiFiUdp.h>
#include <esp_now.h>
#include <esp_wifi.h>

const char * ssid = "Bruh";
const char * pwd = "bruh1234";
const char * udpAddress = "192.168.156.10";
//bool flag = false;
//int firstAngle = 0;

WiFiUDP udp;
esp_now_peer_info_t peerInfo;

struct Data {
  int ldr;
  int x, y;
};

Data data;

void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  memcpy(&data, incomingData, sizeof(data));
  uint8_t sender[] = { 0x3C, 0x61, 0x05, 0x03, 0x41, 0xA0 };
  if (mac_addr[0] == sender[0] && mac_addr[1] == sender[1] && mac_addr[2] == sender[2] && mac_addr[3] == sender[3] && mac_addr[4] == sender[4] && mac_addr[5] == sender[5]) {
    //sun angle
    udp.beginPacket(udpAddress, 30000);
    udp.write(reinterpret_cast<const uint8_t*>(std::to_string(data.y).c_str()), sizeof(const uint8_t)*std::to_string(data.y).length());
    Serial.printf("angle: %d\n", data.y);
    udp.endPacket();
    //sun direction
    udp.beginPacket(udpAddress, 30001);
    udp.write(reinterpret_cast<const uint8_t*>(std::to_string(data.x).c_str()), sizeof(const uint8_t)*std::to_string(data.x).length());
    Serial.printf("direction: %d\n", data.x);
    udp.endPacket();
    // avg brightness
    udp.beginPacket(udpAddress, 30002);
    udp.write(reinterpret_cast<const uint8_t*>(std::to_string(data.ldr).c_str()), sizeof(const uint8_t)*std::to_string(data.ldr).length());
    Serial.printf("brightness: %d\n", data.ldr);
    udp.endPacket();
  }
  
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(2000);
  
  // esp now init
  WiFi.mode(WIFI_AP_STA);
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);

  // wifi init
  WiFi.printDiag(Serial); 
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous(false);
  Serial.println("connecting to wifi");
  WiFi.begin(ssid, pwd);
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("connected to ");
  Serial.print(ssid);
  Serial.println("");
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous(false);
  WiFi.printDiag(Serial);
}

void loop() {
  // put your main code here, to run repeatedly:
}