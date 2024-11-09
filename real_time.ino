#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <time.h>
#include <WiFi.h>

// Initialize the display (SH1106 driver, I2C)
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Wi-Fi credentials
const char* ssid = "ARM"; // Replace with your Wi-Fi SSID
const char* password = "ESP32DEHM"; // Replace with your Wi-Fi password

// NTP server and time zone
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 19800; // IST is UTC + 5:30
const int daylightOffset_sec = 0; // No daylight saving time in India

void setup() {
  Serial.begin(115200);
  
  // Initialize the display
  u8g2.begin(); 

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  // Configure time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // Print initial time
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
  } else {
    Serial.printf("Current Time: %02d:%02d:%02d\n",
                  timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  }
}

void loop() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  // Format time to display as "HH:MM:SS"
  char timeString[10];
  strftime(timeString, sizeof(timeString), "%H:%M:%S", &timeinfo);

  // Display time on OLED
  u8g2.clearBuffer(); // Clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr); // Choose a font
  u8g2.drawStr(0, 10, "Current Time:");
  u8g2.drawStr(0, 30, timeString); // Display the time string
  u8g2.sendBuffer(); // Transfer the internal memory to the display

  delay(1000); // Update every second
}