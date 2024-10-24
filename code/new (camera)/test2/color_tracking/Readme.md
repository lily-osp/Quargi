# ESP32-CAM Project

## Overview
This project implements a WiFi-enabled camera server using the ESP32-CAM module. It provides a web interface for camera control and image capture, with features for adjusting camera settings and performing color detection.

## Hardware Requirements
- ESP32-CAM module (AI-Thinker model)
- USB-to-TTL converter for programming
- 5V power supply
- Appropriate USB cable for programming

## Pin Configuration
The project uses the AI-Thinker ESP32-CAM module with the following pin configuration:
```
GPIO 0  -> XCLK
GPIO 26 -> SIOD
GPIO 27 -> SIOC
GPIO 35 -> Y9
GPIO 34 -> Y8
GPIO 39 -> Y7
GPIO 36 -> Y6
GPIO 21 -> Y5
GPIO 19 -> Y4
GPIO 18 -> Y3
GPIO 5  -> Y2
GPIO 25 -> VSYNC
GPIO 23 -> HREF
GPIO 22 -> PCLK
```

## Software Requirements
- Arduino IDE
- ESP32 board support package
- Required Libraries:
  - WiFi
  - WiFiClientSecure
  - ESP32 Camera Driver

## Installation Steps

1. **Arduino IDE Setup**
   - Install Arduino IDE from [arduino.cc](https://www.arduino.cc/en/software)
   - Add ESP32 board support to Arduino IDE:
     - Go to File > Preferences
     - Add `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json` to Additional Board URLs
     - Go to Tools > Board > Boards Manager
     - Search for "ESP32" and install "ESP32 by Espressif Systems"

2. **Library Installation**
   - Go to Sketch > Include Library > Manage Libraries
   - Install all required libraries listed above

3. **Configuration**
   - Open the code in Arduino IDE
   - Modify the WiFi credentials:
     ```cpp
     const char* ssid = "ssid";      // Replace with your WiFi SSID
     const char* password = "password"; // Replace with your WiFi password
     ```

4. **Upload the Code**
   - Select "AI Thinker ESP32-CAM" from Tools > Board
   - Select the correct COM port
   - Press and hold the BOOT button while uploading
   - Release the BOOT button after upload begins

## Features

### Camera Settings
The following camera settings can be adjusted via HTTP commands:
- Quality (0-63, lower number means higher quality)
- Contrast
- Brightness

### Available Commands
1. **Reset WiFi Connection**
   - Command: `resetwifi`
   - Parameters: SSID and password

2. **Restart Device**
   - Command: `restart`
   - No parameters required

3. **Adjust Image Quality**
   - Command: `quality`
   - Parameter: quality value (0-63)

4. **Adjust Contrast**
   - Command: `contrast`
   - Parameter: contrast value

5. **Adjust Brightness**
   - Command: `brightness`
   - Parameter: brightness value

6. **Color Detection**
   - Command: `colorDetect`
   - Returns JPEG image stream

## Usage
1. Power up the ESP32-CAM
2. Connect to the WiFi network specified in the code
3. Access the web interface using the IP address printed in the Serial Monitor
4. Use HTTP commands to control the camera settings

## HTTP API Examples
```
http://[ESP-IP]/?quality=10
http://[ESP-IP]/?brightness=2
http://[ESP-IP]/?contrast=1
http://[ESP-IP]/?resetwifi=NewSSID;NewPassword
http://[ESP-IP]/?restart
http://[ESP-IP]/?colorDetect
```

## Troubleshooting

### Common Issues
1. **Camera Init Failed**
   - Check power supply stability
   - Verify all connections
   - Try resetting the module

2. **WiFi Connection Issues**
   - Verify SSID and password
   - Check WiFi signal strength
   - Ensure router is compatible with ESP32

3. **Image Quality Issues**
   - Adjust quality parameter (0-63)
   - Check lighting conditions
   - Verify camera module connection

### LED Indicators
- Built-in LED will flash during initialization
- Solid LED indicates successful WiFi connection

## Security Considerations
- The current implementation doesn't include authentication
- WiFi credentials are stored in plain text
- Consider implementing:
  - HTTPS
  - Authentication
  - Access control
  - Encrypted storage for credentials

## Power Management
- The device requires a stable 5V power supply
- Current consumption varies based on:
  - WiFi activity
  - Camera operation
  - Image processing tasks

## Contributing
1. Fork the repository
2. Create your feature branch
3. Commit your changes
4. Push to the branch
5. Create a new Pull Request

## Acknowledgments
- Based on the ESP32 Camera library by Espressif
- Uses the AI-Thinker ESP32-CAM module
- Community contributions and feedback
