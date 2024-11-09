#include <WiFi.h>           // Include the library to connect to Wi-Fi
#include <U8g2lib.h>       // Include the library for controlling the display
#include <Wire.h>          // Include the library for I2C communication
#include <NTPClient.h>     // Include the library to get the time from an NTP server
#include <WiFiUdp.h>       // Include the library for sending and receiving UDP packets
#include <time.h>          // Include the library for handling time

// Wi-Fi credentials
const char* ssid = "ARM";           // Name of your Wi-Fi network
const char* password = "ESP32DEHM"; // Password for your Wi-Fi network

// Set up the display (using SH1106 driver)
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); // Create a display object

// Set up NTP with IST time zone (UTC +5:30)
WiFiUDP ntpUDP;                             // Create a UDP object for NTP
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 3600000); // Set up NTP client to fetch time

// Function declarations (we'll define these later)
void drawCalendar(int month, int year, int today); // Draw the calendar on the display
int getDaysInMonth(int month, int year);            // Get the number of days in a month
int calculateFirstDayOfMonth(int year, int month); // Calculate the first day of the month

void setup() {
  // Start serial communication for debugging
  Serial.begin(115200); // Set up Serial at 115200 baud rate
  
  // Initialize the display
  u8g2.begin(); // Prepare the display for use

  // Connect to Wi-Fi
  WiFi.begin(ssid, password); // Start connecting to Wi-Fi
  while (WiFi.status() != WL_CONNECTED) { // Wait until we're connected
    delay(500); // Wait half a second
    Serial.print("."); // Print a dot to show we're still trying to connect
  }
  Serial.println("\nConnected to Wi-Fi"); // Print a message when connected

  // Start the NTP client to get the time
  timeClient.begin(); // Begin fetching time from the NTP server
}

void loop() {
  // Check if the NTP client has updated the time
  if (timeClient.update()) { // If we have new time data
    // Get the current time
    time_t epochTime = timeClient.getEpochTime(); // Get time in seconds since Jan 1, 1970
    struct tm *timeInfo = gmtime(&epochTime); // Convert to a readable format (UTC)
    int year = timeInfo->tm_year + 1900; // Get the year (add 1900 because tm_year starts from 1900)
    int month = timeInfo->tm_mon + 1;    // Get the month (0-11, so add 1 for 1-12)
    int dayOfMonth = timeInfo->tm_mday;  // Get the day of the month (1-31)

    u8g2.clearBuffer(); // Clear the display buffer for new content
    drawCalendar(month, year, dayOfMonth);   // Draw the current month's calendar
    u8g2.sendBuffer();                       // Display the new content on the screen
  }
  
  delay(1000);  // Wait for 1 second before the next update
}

// Function to draw the calendar
void drawCalendar(int month, int year, int today) {
  // Display the month and year at the top
  u8g2.setFont(u8g2_font_5x7_tr); // Use a smaller font for the header
  const char* monthNames[] = {"January", "February", "March", "April", "May", "June", 
                              "July", "August", "September", "October", "November", "December"};
  String header = String(monthNames[month - 1]) + " " + String(year); // Create a string for the month and year
  u8g2.setCursor(0, 8); // Position the cursor at the top
  u8g2.print(header); // Print the month and year

  // Display the day names (Sun, Mon, etc.)
  const char* days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"}; // Array of day names
  for (int i = 0; i < 7; i++) { // Loop through the days
    u8g2.setCursor(i * 18, 16); // Position for each day name
    u8g2.print(days[i]); // Print the day name
  }

  // Draw the days of the month
  int daysInMonth = getDaysInMonth(month, year); // Get the number of days in the current month
  int firstDay = calculateFirstDayOfMonth(year, month); // Find out what day the month starts on
  int x = firstDay; // Start drawing days from the first day of the month
  int y = 24; // Start below the day names

  for (int dayNum = 1; dayNum <= daysInMonth; dayNum++) { // Loop through each day
    if (x > 6) { // If we reach past Saturday
      x = 0; // Go back to Sunday
      y += 7; // Move down to the next row
    }
    
    // Highlight today's date with a circle
    if (dayNum == today) { // Check if this day is today
      u8g2.drawCircle(x * 18 + 4, y - 3, 5); // Draw a circle around today's date
    }
    
    // Print the day number
    u8g2.setCursor(x * 18, y); // Position for the day number
    u8g2.print(dayNum); // Print the day number
    x++; // Move to the next column for the next day
  }
}

// Function to get the number of days in a month
int getDaysInMonth(int month, int year) {
  if (month == 2) { // If it's February
    // Check for leap year: return 29 days if true, otherwise 28
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) ? 29 : 28; 
  }
  // April, June, September, and November have 30 days
  if (month == 4 || month == 6 || month == 9 || month == 11) return 30; // Return 30 days for these months
  return 31; // All other months have 31 days
}

// Function to calculate the first day of the month
int calculateFirstDayOfMonth(int year, int month) {
  if (month < 3) { // If the month is January or February
    month += 12; // Treat them as months 13 and 14 of the previous year
    year--; // Decrease the year by 1
  }
  int K = year % 100; // Get the last two digits of the year
  int J = year / 100; // Get the first two digits of the year
  // Calculate the first day of the month using a formula
  return ((1 + (13 * (month + 1)) / 5 + K + (K / 4) + (J / 4) - (2 * J)) % 7 + 6) % 7; // Adjust so 0 is Sunday
}