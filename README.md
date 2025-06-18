# Real-Time Embedded Systems Project

This repository contains multiple modules developed for an embedded systems project using microcontroller platforms such as Arduino. Each `.ino` file represents a distinct functionality including real-time clock operations, BLE communication, alarms, calendars, and more.

## Project Structure

| File               | Description                                                                 |
|--------------------|-----------------------------------------------------------------------------|
| `alarm.ino`        | Implements an alarm clock with settable time and sound/buzzer alert system.|
| `ble.ino`          | Bluetooth Low Energy (BLE) communication module.                            |
| `calender.ino`     | Calendar implementation with date tracking and navigation.                  |
| `combined_two.ino` | Combines two modules for integrated functionality.                          |
| `real_time.ino`    | Core real-time clock (RTC) functionality using an RTC module.               |
| `stopwatch.ino`    | Digital stopwatch with start/stop/reset features.                           |
| `temperature.ino`  | Temperature sensor integration for real-time readings.                      |
| `ES REPORT.pdf`    | Final project report with technical details and documentation.              |
| `README.md`        | You're reading it :)                                                        |

## Technologies Used

- **Arduino IDE**
- **C/C++** for embedded firmware
- **Real-Time Clock (RTC) Modules**
- **BLE Modules**
- **DS18B20 / DHT Sensors** for temperature
- **EEPROM / Serial Communication** where applicable

## Getting Started

### Prerequisites

- Arduino IDE installed
- Required libraries:
  - `RTClib`
  - `OneWire`
  - `DallasTemperature`
  - `Adafruit_Sensor`
  - `BLEPeripheral` or relevant BLE library

### Uploading

1. Open the desired `.ino` file in Arduino IDE.
2. Connect your microcontroller via USB.
3. Select the correct board and COM port.
4. Click **Upload**.

## Demo Video

Check out the working demo of the project on YouTube:

[![Watch the video](https://img.youtube.com/vi/uhUrj592S4Y/0.jpg)](https://youtu.be/uhUrj592S4Y)

## Project Highlights

- **Modular Design:** Each functionality is broken into its own sketch for clarity and reusability.
- **Real-Time Integration:** Seamlessly combines clock-based features with user interaction.
- **BLE Support:** Communicate with mobile or desktop apps wirelessly.
- **Sensors Integration:** Real-world interaction through temperature and time-based sensing.

## Documentation

Refer to `ES REPORT.pdf` for a detailed explanation of the system architecture, components used, circuit diagrams, and test results.

## Author

**DEFCOM AND THE EMBEDDED MINDS**

---

Feel free to open issues or contribute if you have improvements or suggestions.
