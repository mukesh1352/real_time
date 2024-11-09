#include <Wire.h>
#include <U8g2lib.h>
#include "DHT.h"

// Define DHT sensor
#define DHTPIN 19     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 // DHT 11
DHT dht(DHTPIN, DHTTYPE);

// Initialize the OLED display (SH1106 128x64)
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void setup() {
  Serial.begin(115200);
  Serial.println(F("DHT11 test!"));

  // Initialize the DHT sensor
  dht.begin();

  // Initialize the OLED display
  u8g2.begin();
}

void loop() {
  // Wait a few seconds between measurements
  delay(2000);

  // Reading temperature and humidity
  float h = dht.readHumidity();
  float t = dht.readTemperature();      // Temperature in Celsius
  float f = dht.readTemperature(true);  // Temperature in Fahrenheit

  // Check if any reads failed and exit early
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    
    // Display error message on OLED screen
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_helvB08_tf);
    u8g2.drawStr(20, 30, "Failed to read");
    u8g2.drawStr(20, 45, "from DHT sensor!");
    u8g2.sendBuffer();
    return;
  }

  // Compute heat index in Fahrenheit and Celsius
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  // Display data on the OLED screen with a decent and clean layout
  u8g2.clearBuffer();

  // Title
  u8g2.setFont(u8g2_font_helvB08_tf);  // Decent-sized font for title
  u8g2.drawStr(25, 12, "Weather Station");

  // Draw a thin horizontal line for separation
  u8g2.drawHLine(0, 16, 128);

  // Display Humidity
  u8g2.setFont(u8g2_font_helvR08_tf); // Regular font
  u8g2.setCursor(5, 30);
  u8g2.print("Humidity: ");
  u8g2.print(h);
  u8g2.print(" %");

  // Display Temperature in Celsius
  u8g2.setCursor(5, 45);
  u8g2.print("Temp: ");
  u8g2.print(t);
  u8g2.print(" C");

  // Display Temperature in Fahrenheit
  u8g2.setCursor(70, 45);
  u8g2.print(f);
  u8g2.print(" F");

  // Display Heat Index
  u8g2.setCursor(5, 60);
  u8g2.print("Heat Index: ");
  u8g2.print(hic);
  u8g2.print(" C");

  // Send the buffer to the display
  u8g2.sendBuffer();
}