#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <time.h>
#include <WiFi.h>

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=/ 22, / data=/ 21, / reset=*/ U8X8_PIN_NONE);


//Mode button
// Button and counter setup
const int modePin = 4;      // Define the button pin
int mode = 0;              // Counter to display on OLED
int lastButtonState = HIGH;   // Previous button state for debounce



//(time code)
// Wi-Fi credentials
const char* ssid = "ARM"; // Replace with your Wi-Fi SSID
const char* password = "ESP32DEHM"; // Replace with your Wi-Fi password

// NTP server and time zone
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 19800; // IST is UTC + 5:30
const int daylightOffset_sec = 0; // No daylight saving time in India


//(stopwatch code)

#define BUTTON_PIN 5  // Button pin
// Stopwatch variables
int minutes = 0;
int seconds = 0;
int milliseconds = 0;
unsigned long startTime = 0;
unsigned long pausedTime = 0; // Tracks time when paused
bool isRunning = false;       // Tracks if the stopwatch is running
int Smode = 0;                 // Tracks the current Smode (0 = reset, 1 = running, 2 = paused, 3 = resume)

// Button state tracking
bool buttonPressed = false;



void setup()
{

    // for time code
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

  //for stop watch code

  pinMode(BUTTON_PIN, INPUT_PULLUP); // Setup button with pull-up
  u8g2.begin();                      // Initialize the display

  u8g2.setFont(u8g2_font_ncenB08_tr);
  // Show initial splash screen
  
  u8g2.clearBuffer();
  u8g2.drawStr(10, 30, "Project By: EDMH");
  
  
  
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.drawStr(10, 45, "Hyouka");      // Adjust y-position to fit below the previous line
  
  u8g2.sendBuffer();
  delay(2000);

  //mode code

    pinMode(modePin, INPUT_PULLUP); // Set up button pin
    

}


void loop()
{
    int reading = digitalRead(modePin);  // Read the button state
    DisplayOnWatch();

    if (reading == LOW && lastButtonState == HIGH) {
    mode = (mode + 1) % 2;         // Toggle mode between 0 and 1
    DisplayOnWatch();              // Update display based on new mode
    delay(200);                    // Simple debounce delay
  }
  
  lastButtonState = reading;       // Update last button state

}

void DisplayOnWatch(){

    if (mode == 0) {
    TimeClock();
  } else {
    StopWatch();
  }

}

void TimeClock(){

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

void StopWatch(){

    handleButtonPress();
  updateStopwatch();
  updateDisplay();

}

// Handle button press to cycle through stopwatch Smodes
void handleButtonPress() {
  if (digitalRead(BUTTON_PIN) == LOW && !buttonPressed) {
    buttonPressed = true;           // Debounce: register press once
    Smode = (Smode + 1) % 4;          // Cycle through Smodes: 0 (reset), 1 (running), 2 (paused), 3 (resume)
    
    if (Smode == 1) {
      // Start or resume stopwatch
      isRunning = true;
      startTime = millis() - pausedTime; // Account for paused time
    } else if (Smode == 2) {
      // Pause stopwatch
      isRunning = false;
      pausedTime = millis() - startTime; // Store elapsed time when paused
    } else if (Smode == 3) {
      // Resume stopwatch
      isRunning = true;
      startTime = millis() - pausedTime; // Adjust startTime for resumed time
    } else {
      // Reset stopwatch
      resetStopwatch();
    }
  } else if (digitalRead(BUTTON_PIN) == HIGH) {
    buttonPressed = false; // Button is released
  }
}

// Update the stopwatch values if it's running
void updateStopwatch() {
  if (isRunning) {
    unsigned long elapsed = millis() - startTime;
    minutes = elapsed / 60000;
    seconds = (elapsed / 1000) % 60;
    milliseconds = elapsed % 1000; // Display full milliseconds for precision
  }
}

// Reset stopwatch values
void resetStopwatch() {
  minutes = 0;
  seconds = 0;
  milliseconds = 0;
  pausedTime = 0;
  isRunning = false;
}

// Update the OLED display based on the current mode
void updateDisplay() {
  u8g2.clearBuffer();
  
  // Draw border around the display area for a "stopwatch" look
  u8g2.drawFrame(5, 5, 118, 54);

  // Draw a label at the top based on the current mode
  u8g2.setFont(u8g2_font_ncenB08_tr); // Use a smaller font for labels
  if (Smode == 0) {
    u8g2.drawStr(38, 15, "STOPWATCH");
    u8g2.drawStr(32, 30, "Press Start");
  } 
  else if (Smode == 1) {
    u8g2.drawStr(38, 15, "Running...");
    displayTime();
  } 
  else if (Smode == 2) {
    u8g2.drawStr(38, 15, "Paused");
    displayTime();
  } 
  else if (Smode == 3) {
    u8g2.drawStr(38, 15, "Resuming...");
    displayTime();
  }

  u8g2.sendBuffer(); // Send buffer to the display
}

// Display formatted time (MM:SS:MMM) with larger font and centered
void displayTime() {
  u8g2.setFont(u8g2_font_fur20_tr); // Use a larger font for time display
  char timeString[12];
  snprintf(timeString, sizeof(timeString), "%02d:%02d:%03d", minutes, seconds, milliseconds);

  // Calculate centered position for the larger font
  int xPos = (128 - u8g2.getStrWidth(timeString)) / 2;
  int yPos = 48; // Y position for centered look
  u8g2.drawStr(xPos, yPos, timeString);  // Draw the centered time display
}