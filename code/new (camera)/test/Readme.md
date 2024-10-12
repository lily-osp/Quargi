# Red Object Tracking Arm Project

## Table of Contents
1. [Overview](#overview)
2. [Detailed Component Description](#detailed-component-description)
3. [Hardware Requirements](#hardware-requirements)
4. [Software Requirements](#software-requirements)
5. [Detailed Setup Instructions](#detailed-setup-instructions)
6. [Code Explanation](#code-explanation)
7. [Usage Guide](#usage-guide)
8. [Customization and Tuning](#customization-and-tuning)
9. [Troubleshooting](#troubleshooting)
10. [Future Improvements](#future-improvements)
11. [Safety Considerations](#safety-considerations)

## Overview

This project implements an intelligent robotic arm system that autonomously tracks and centers a red object using computer vision techniques. The system comprises two primary components working in tandem:

1. **ESP32-CAM**: Handles image capture, red object detection, and coordinate transmission.
2. **Arduino Uno**: Controls the robotic arm based on the received coordinates.

The ESP32-CAM continuously captures images, processes them to detect red objects, and transmits the object's coordinates to the Arduino Uno via I2C communication. The Arduino Uno then adjusts the robotic arm's position in real-time to maintain the red object at the center of the camera's field of view.

## Detailed Component Description

### 1. ESP32-CAM (ESP32_CAM_RedObjectDetection.ino)

The ESP32-CAM module serves as the "eyes" of the system. Its responsibilities include:

- **Camera Initialization**: Configures the onboard camera with appropriate settings.
- **Image Capture**: Continuously captures frames for processing.
- **Red Object Detection**: Analyzes each frame to identify red objects based on color thresholds.
- **Coordinate Calculation**: Determines the centroid of the detected red object.
- **I2C Communication**: Transmits the calculated coordinates to the Arduino Uno.

### 2. Arduino Uno (Arduino_Uno_RedObjectTrackingArm.ino)

The Arduino Uno acts as the "brain" and "muscles" of the system. Its tasks include:

- **Servo Control**: Manages the robotic arm's servos for precise movements.
- **I2C Communication**: Receives coordinate data from the ESP32-CAM.
- **Position Calculation**: Determines necessary adjustments based on received coordinates.
- **Arm Movement**: Executes calculated movements to center the red object.

## Hardware Requirements

- 1x ESP32-CAM module
- 1x Arduino Uno board
- 1x Robotic arm kit with at least 3 servos:
  - 1x Base servo (horizontal rotation)
  - 1x Shoulder servo (vertical lower arm movement)
  - 1x Elbow servo (vertical upper arm movement)
  - 1x Gripper servo (optional, for object manipulation)
- Jumper wires for I2C connection
- USB cables for programming ESP32-CAM and Arduino Uno
- Power supply:
  - 5V for Arduino Uno
  - 5V for ESP32-CAM (can be powered through Arduino Uno)
- Breadboard (optional, for easier connections)
- Red object for tracking (e.g., a red ball or cube)

## Software Requirements

- Arduino IDE (version 1.8.13 or later recommended)
- ESP32 board support for Arduino IDE
  - Follow the [official ESP32 installation guide](https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/boards_manager.md)
- Required libraries:
  - For ESP32-CAM:
    - esp_camera (included in ESP32 board support)
    - Wire (for I2C communication, included in Arduino IDE)
  - For Arduino Uno:
    - Wire (for I2C communication, included in Arduino IDE)
    - Servo (for controlling servos, included in Arduino IDE)

## Detailed Setup Instructions

### ESP32-CAM Setup

1. Install ESP32 board support in Arduino IDE:
   - Open Arduino IDE
   - Go to File > Preferences
   - Add `https://dl.espressif.com/dl/package_esp32_index.json` to Additional Board Manager URLs
   - Go to Tools > Board > Boards Manager
   - Search for "esp32" and install "ESP32 by Espressif Systems"

2. Connect ESP32-CAM to your computer:
   - Use an FTDI programmer or USB-to-TTL converter
   - Connect VCC to 5V, GND to GND, U0R to TX, U0T to RX
   - Connect GPIO 0 to GND (for flashing mode)

3. Open `ESP32_CAM_RedObjectDetection.ino` in Arduino IDE

4. Configure IDE settings:
   - Select "AI Thinker ESP32-CAM" from Tools > Board
   - Set the correct port under Tools > Port

5. Upload the code:
   - Click the Upload button or use Sketch > Upload
   - After upload, disconnect GPIO 0 from GND

### Arduino Uno Setup

1. Connect Arduino Uno to your computer via USB

2. Open `Arduino_Uno_RedObjectTrackingArm.ino` in Arduino IDE

3. Configure IDE settings:
   - Select "Arduino Uno" from Tools > Board
   - Set the correct port under Tools > Port

4. Upload the code:
   - Click the Upload button or use Sketch > Upload

### Hardware Connection

1. I2C Connection:
   - Connect ESP32-CAM SDA (GPIO 26) to Arduino Uno SDA (A4)
   - Connect ESP32-CAM SCL (GPIO 27) to Arduino Uno SCL (A5)
   - Connect ESP32-CAM GND to Arduino Uno GND

2. Servo Connections to Arduino Uno:
   - Base Servo: Signal to Pin 2
   - Shoulder Servo: Signal to Pin 7
   - Elbow Servo: Signal to Pin 8
   - Gripper Servo (if used): Signal to Pin 11

3. Power Connections:
   - Connect Arduino Uno to 5V power supply
   - Connect ESP32-CAM VCC to Arduino Uno 5V
   - Connect all servo power wires to a separate 5V supply (servos can draw too much current for the Arduino to handle safely)

4. Mount the ESP32-CAM securely at the tip of the robotic arm, ensuring an unobstructed view

## Code Explanation

### ESP32-CAM Code (ESP32_CAM_RedObjectDetection.ino)

```cpp
void initCamera() {
  // Camera configuration
}
```
This function initializes the camera with specific settings for resolution, quality, and pin assignments.

```cpp
bool detectRed(camera_fb_t *fb, int &posX, int &posY) {
  // Red detection algorithm
}
```
This function processes each frame to detect red objects. It uses color thresholds to identify red pixels and calculates the centroid of the red area.

```cpp
void sendPosition(int posX, int posY) {
  // I2C transmission of coordinates
}
```
This function sends the calculated X and Y coordinates to the Arduino Uno via I2C.

### Arduino Uno Code (Arduino_Uno_RedObjectTrackingArm.ino)

```cpp
void receiveEvent(int byteCount) {
  // I2C data reception
}
```
This function is called when data is received from the ESP32-CAM. It reads the X and Y coordinates.

```cpp
void adjustArmPosition(int posX, int posY) {
  // Arm movement logic
}
```
This function calculates and executes the necessary servo movements to center the red object.

## Usage Guide

1. Ensure all connections are secure and power on the system.
2. The arm will move to its home position upon startup.
3. Place a red object within the camera's field of view.
4. The system will automatically detect the red object and move the arm to center it.
5. Move the red object slowly to observe the arm tracking it.
6. To stop the system, simply remove the red object from view or power off the devices.

## Customization and Tuning

### ESP32-CAM Customization:

In `ESP32_CAM_RedObjectDetection.ino`:
- Adjust `IMAGE_WIDTH` and `IMAGE_HEIGHT` for different resolutions
- Modify color detection thresholds in `detectRed()` function:
  ```cpp
  if (r > 150 && g < 100 && b < 100) {
    // Adjust these values for different lighting conditions
  }
  ```

### Arduino Uno Customization:

In `Arduino_Uno_RedObjectTrackingArm.ino`:
- Adjust `TOLERANCE` to change movement sensitivity:
  ```cpp
  const int TOLERANCE = 20; // Increase for less frequent movements
  ```
- Modify servo angle limits:
  ```cpp
  const int BASE_MIN_ANGLE = 0;
  const int BASE_MAX_ANGLE = 180;
  // Adjust these for your specific arm's range of motion
  ```
- Change movement speed in `adjustArmPosition()`:
  ```cpp
  currentBaseAngle = min(currentBaseAngle + 1, BASE_MAX_ANGLE);
  // Increase the increment for faster movements
  ```

## Troubleshooting

1. **Red object not detected**:
   - Check lighting conditions
   - Adjust color thresholds in `detectRed()` function
   - Ensure camera lens is clean and focused

2. **Jerky arm movements**:
   - Increase `TOLERANCE` value
   - Decrease movement speed in `adjustArmPosition()`
   - Check for loose servo connections

3. **I2C communication issues**:
   - Verify I2C connections
   - Check I2C addresses in both codes
   - Use I2C scanner sketch to confirm communication

4. **Arm moves out of range**:
   - Adjust servo angle limits in Arduino code
   - Implement software stops to prevent over-rotation

5. **System becomes unresponsive**:
   - Check power supply stability
   - Implement watchdog timer in both ESP32-CAM and Arduino code

## Future Improvements

1. Implement PID control for smoother, more accurate movements
2. Add multi-color object detection and tracking
3. Incorporate distance sensing (e.g., ultrasonic sensor) for depth perception
4. Develop a user interface for manual control and system configuration
5. Implement machine learning for improved object detection and tracking
6. Add data logging and analysis features for performance optimization

## Safety Considerations

1. Implement emergency stop functionality (e.g., a physical button to halt all movements)
2. Add current sensing to detect motor stalls or obstructions
3. Use servo easing libraries for smoother, safer movements
4. Implement software limits to restrict arm movements to safe zones
5. Consider adding protective enclosures around moving parts
6. Include warning LEDs or sounds to indicate when the system is active

