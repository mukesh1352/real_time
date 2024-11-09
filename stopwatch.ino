#include <U8g2lib.h>

// Initialize the OLED display with correct pins (modify SCL and SDA as needed)
//U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=/ 22, / data=/ 21, / reset=*/ U8X8_PIN_NONE);
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 22, /* data=*/ 21, /* reset=*/ U8X8_PIN_NONE);



#define BUTTON_PIN 5  // Button pin

// Stopwatch variables
int minutes = 0;
int seconds = 0;
int milliseconds = 0;
unsigned long startTime = 0;
unsigned long pausedTime = 0; // Tracks time when paused
bool isRunning = false;       // Tracks if the stopwatch is running
int mode = 0;                 // Tracks the current mode (0 = reset, 1 = running, 2 = paused, 3 = resume)

// Button state tracking
bool buttonPressed = false;

void setup() {
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
}


void loop() {
  handleButtonPress();
  updateStopwatch();
  updateDisplay();
}

// Handle button press to cycle through stopwatch modes
void handleButtonPress() {
  if (digitalRead(BUTTON_PIN) == LOW && !buttonPressed) {
    buttonPressed = true;           // Debounce: register press once
    mode = (mode + 1) % 4;          // Cycle through modes: 0 (reset), 1 (running), 2 (paused), 3 (resume)
    
    if (mode == 1) {
      // Start or resume stopwatch
      isRunning = true;
      startTime = millis() - pausedTime; // Account for paused time
    } else if (mode == 2) {
      // Pause stopwatch
      isRunning = false;
      pausedTime = millis() - startTime; // Store elapsed time when paused
    } else if (mode == 3) {
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
  if (mode == 0) {
    u8g2.drawStr(38, 15, "STOPWATCH");
    u8g2.drawStr(32, 30, "Press Start");
  } 
  else if (mode == 1) {
    u8g2.drawStr(38, 15, "Running...");
    displayTime();
  } 
  else if (mode == 2) {
    u8g2.drawStr(38, 15, "Paused");
    displayTime();
  } 
  else if (mode == 3) {
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