#include <WiFi.h>
#include <WiFiUdp.h>
#include <esp_now.h>

const char * ssid = "Bruh";
const char * pwd = "bruh1234";
const char * udpAddress = "192.168.247.10";
//bool flag = false;
//int firstAngle = 0;

WiFiUDP udp;
esp_now_peer_info_t peerInfo;

struct Data {
  int ldr1;
  int ldr2;
  int ldr3;
  int ldr4;
  int x, y;
};

Data data;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&data, incomingData, sizeof(data));
  //if (!flag) {
  //  firstAngle = data.x;
  //  flag = true;
  //}
  //int ang = data.y;
  //if (flag && abs(data.x-firstAngle) > 160) {
  //  ang = 180-ang;
  //}

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
  /*Serial.printf("ldr1: %d\n", data.ldr1);
  Serial.printf("ldr2: %d\n", data.ldr2);
  Serial.printf("ldr3: %d\n", data.ldr3);
  Serial.printf("ldr4: %d\n", data.ldr4);*/
  double avg = (data.ldr1+data.ldr2+data.ldr3+data.ldr4)/4;
  udp.beginPacket(udpAddress, 30002);
  udp.write(reinterpret_cast<const uint8_t*>(std::to_string((int)avg).c_str()), sizeof(const uint8_t)*std::to_string((int)avg).length());
  Serial.printf("brightness: %d\n", (int) avg);
  udp.endPacket();
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // esp now init
  WiFi.mode(WIFI_STA);
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);

  // wifi init
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
}

void loop() {
  // put your main code here, to run repeatedly:
}