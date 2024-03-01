#include <SPI.h>
#include <Wire.h>
#include <ezButton.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define BUTTON_PIN 23
ezButton button(BUTTON_PIN);

#define OLED_RESET 16
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(OLED_RESET);

float latestGenElec;
float genElec;

void setup() {
  Serial.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.display();
  delay(1000);

  latestGenElec = 0;
  genElec = 0;
}

void loop() {
  button.loop();

  updateValue();

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

void updateValue() {
  latestGenElec = (float)random(0, 4096) * 5 / 4095;
  genElec += latestGenElec * 0.5 /3600;
}

void displayAverage() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0, 0);
  display.println("Avg. gen. electricity");
  display.printf("%.2f kWh", latestGenElec);

  display.display();
  delay(100);
}

void displayGeneratedElectricity() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0, 0);
  display.println("Generated electricity");
  display.printf("%.2f kWh", genElec);

  display.display();
  delay(100);
}

void displayMoneySave() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0, 0);
  display.println("Money Save");
  display.printf("%.2f Baht", genElec * 7);

  display.display();
  delay(100);
}
