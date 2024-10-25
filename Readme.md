### Quargi

This repository hosts a dual-system project featuring a **serial-controlled robot** powered by an Arduino, and an **ESP32-CAM-based color detection and tracking module**. The combination of these two systems enables advanced, responsive tracking and movement based on real-time visual input, ideal for a wide range of robotics applications.

#### Key Components

1. **Arduino Serial-Controlled Robot**:
   - **Movement Control**: The Arduino microcontroller receives serial commands for precise directional control, allowing the robot to navigate smoothly.
   - **Modular Design**: Easily adaptable for additional sensors or motors, enhancing its functionality.
   - **Versatile Communication**: Accepts serial input, making it compatible with various control sources, such as Bluetooth modules, RF, or serial-based applications.

2. **ESP32-CAM Color Detection and Tracking**:
   - **Real-Time Color Detection**: Utilizes the ESP32-CAM's capabilities to detect specified colors within its field of view.
   - **Tracking Capabilities**: Once a target color is detected, the system can send data to the Arduino, enabling the robot to orient or move toward the target.
   - **Efficient Processing**: Designed to leverage the ESP32's onboard resources, ensuring smooth detection and communication without offloading tasks.

#### Highlights
- **Inter-System Communication**: Seamless serial communication between Arduino and ESP32, ensuring a responsive and synchronized operation.
- **Robust, Modular Codebase**: The project features clean, modular code, allowing easy adjustments to parameters like target color, movement speed, and more.
- **Expandable Framework**: This setup is a strong foundation for further enhancements, such as integrating machine learning models on the ESP32 for more complex object recognition.
