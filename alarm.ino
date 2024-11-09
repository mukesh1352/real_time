#include <U8g2lib.h>

int seconds = 0;
int minutes = 0;
int count = 0;

const int study_minutes = 2;
const int short_break_minutes = 1;
const int long_break_minutes = 3;
const int repeats = 2;

int break_duration;
bool timerActive = false; // Variable to track if the timer is active

// Initialize U8g2 for OLED display (update clock and data pins as per your setup)
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

const int button = 5;

const int debounceDelay = 200; // Debounce time for button press
unsigned long lastPressTime = 0;
unsigned long previousMillis = 0;
const long interval = 1000; // 1-second update for timer

enum TimerState {IDLE, STUDY, BREAK};
TimerState currentState = IDLE;

int minutesElapsed = 0;
int secondsElapsed = 0;

void setup() {
  Serial.begin(9600);  // Initialize serial communication for debugging
  u8g2.begin();
  pinMode(button, INPUT_PULLUP); // Use internal pull-up resistor

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr); // Choose a font
  u8g2.drawStr(0, 10, "Press button");
  u8g2.drawStr(0, 30, "to start/reset");
  u8g2.sendBuffer();
}

void loop() {
  unsigned long currentMillis = millis();
  
  // Check for button press with debouncing
  if (digitalRead(button) == LOW && (currentMillis - lastPressTime) > debounceDelay) {
    lastPressTime = currentMillis;

    if (!timerActive) {
      timerActive = true;
      displayMessage("Timer Started");  // Feedback on the display
      delay(1000);  // Show message for 1 second
      startTimer();
    } else {
      resetTimer();
    }
  }

  // If the timer is running, update it at regular intervals
  if (timerActive) {
    // Check for timeout conditions (1-second interval)
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      switch (currentState) {
        case STUDY:
          updateTimer(study_minutes, "Study time!");
          break;
        case BREAK:
          updateTimer(break_duration, "Break time!");
          break;
        default:
          break;
      }
    }
  }
}

void updateTimer(int duration, const char* mode) {
  // Timer logic that tracks minutes and seconds elapsed
  if (secondsElapsed >= 60) {
    secondsElapsed = 0;
    minutesElapsed++;
  }

  if (minutesElapsed >= duration) {
    // Reset timer and move to the next phase (study/break)
    minutesElapsed = 0;
    secondsElapsed = 0;
    
    if (currentState == STUDY) {
      if (count == (repeats - 1)) {
        break_duration = long_break_minutes;
        currentState = BREAK;
        displayMessage("Long break!");
      } else {
        break_duration = short_break_minutes;
        currentState = BREAK;
        displayMessage("Short break!");
      }
    } else {
      count++;
      if (count < repeats) {
        currentState = STUDY;
        displayMessage("Study time!");
      } else {
        resetTimer();
      }
    }
  } else {
    // Update the display with elapsed time
    secondsElapsed++;
    displayTime(minutesElapsed, secondsElapsed, mode);
  }
}

void displayTime(int minutes, int seconds, const char* mode) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);

  char timeStr[6];
  snprintf(timeStr, sizeof(timeStr), "%02d:%02d", minutes, seconds);

  u8g2.drawStr(0, 10, mode);
  u8g2.drawStr(0, 20, "Time:");
  u8g2.drawStr(0, 40, timeStr);
  u8g2.sendBuffer();
}

void startTimer() {
  // Reset all timer variables before starting the timer
  minutesElapsed = 0;
  secondsElapsed = 0;
  count = 0;
  currentState = STUDY;  // Start in study mode
  displayMessage("Study time!");
}

void resetTimer() {
  // Stop the timer and reset all variables to initial state
  timerActive = false;
  minutesElapsed = 0;
  secondsElapsed = 0;
  count = 0;
  currentState = IDLE;  // Reset to IDLE state

  // Clear the display and show the reset message
  u8g2.clearBuffer();
  u8g2.drawStr(0, 10, "Press button");
  u8g2.drawStr(0, 30, "to start/reset");
  u8g2.sendBuffer();
}

void displayMessage(const char* message) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0, 10, message);
  u8g2.sendBuffer();
}
