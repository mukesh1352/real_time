#include <BluetoothSerial.h>
#include <U8g2lib.h>

// Initialize Bluetooth Serial
BluetoothSerial SerialBT;

// Initialize the OLED display (adjust the parameters as needed)
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

// Variable to store received messages
String receivedMessage = "";

void setup() {
    Serial.begin(115200);
    SerialBT.begin("ESP32_BT"); // Name of your Bluetooth device
    Serial.println("Bluetooth device is ready to pair");
    
    // Initialize OLED
    u8g2.begin();
    u8g2.clearBuffer(); // Clear the internal memory
}

void loop() {
    // Check if there is any data available to read
    if (SerialBT.available()) {
        receivedMessage = SerialBT.readString();
        displayNotification(receivedMessage); // Display the received message
    }
}

// Function to display the received notification on the OLED screen
void displayNotification(String message) {
    u8g2.clearBuffer(); // Clear the previous content

    // Draw a stylish header
    u8g2.setFont(u8g2_font_6x10_tr); // Smaller font for the header
    u8g2.drawBox(0, 0, 128, 16);     // Draw a header box
    u8g2.setDrawColor(0);             // Set text color to "inverted" for the header
    u8g2.drawStr(10, 12, "New Notification");
    u8g2.setDrawColor(1);             // Reset text color

    // Check if the message is too long; truncate if necessary
    if (message.length() > 20) {
        message = message.substring(0, 20) + "...";
    }

    // Display the message in a larger font
    u8g2.setFont(u8g2_font_fur14_tr); // Larger font for main message
    u8g2.drawStr(0, 35, message.c_str());

    // Add a bottom border for style
    u8g2.drawLine(0, 50, 128, 50);

    // Transfer buffer to the display
    u8g2.sendBuffer();
}