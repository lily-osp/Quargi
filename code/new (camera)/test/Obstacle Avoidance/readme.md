# Adaptive Robot Control System (Exploration Mode)

## Overview
This project implements an adaptive robot control system using an Arduinor. The robot utilizes ultrasonic sensors for obstacle detection and employs an auto-tuning PID (Proportional-Integral-Derivative) control system for smooth navigation and obstacle avoidance. Additionally, the system now features an exploration mode, where the robot dynamically changes direction at random intervals, creating an autonomous exploration behavior.

The robot continuously adjusts PID parameters to ensure optimal performance in various environments. The interrupt-driven distance measurement system ensures responsive obstacle detection.

## Features
- **Exploration Mode**: Randomly changes direction at set intervals for dynamic exploration of the environment.
- **Obstacle Avoidance**: Detects obstacles using an ultrasonic sensor and avoids them by adjusting its path.
- **Auto-tuning PID Control**: Automatically tunes motor speed using PID control for smooth, adaptive navigation.
- **Interrupt-driven Distance Measurement**: Ultrasonic sensor uses interrupt-based triggering for more precise and timely distance readings.
- **Real-time PID Adjustment**: Continuously adapts the PID parameters based on robot performance in real-time, providing consistent control.

## Hardware Requirements
1. **Arduino Nano** (or any other compatible board)
2. **L298N Dual H-Bridge Motor Driver**
3. **HC-SR04 Ultrasonic Sensor**
4. **Two DC motors** (for differential drive)
5. **Robot chassis**
6. **Wheels compatible with the DC motors**
7. **9V battery** (or suitable power supply for the motors)
8. **Breadboard and jumper wires**

## Pinout

### Arduino Nano to L298N Motor Driver
- D3 -> IN1 (Motor 1 control)
- D4 -> IN2 (Motor 1 control)
- D5 -> IN3 (Motor 2 control)
- D6 -> IN4 (Motor 2 control)
- D9 -> ENA (Enable Motor 1)
- D10 -> ENB (Enable Motor 2)
- 5V -> 5V (Power for L298N)
- GND -> GND

### Arduino Nano to HC-SR04 Ultrasonic Sensor
- D12 -> TRIG (Trigger pin)
- D2 -> ECHO (Echo pin, connected to interrupt)
- 5V -> VCC (Power)
- GND -> GND (Ground)

### Power Connections
- **9V battery** to L298N's power input for motors.
- Connect the **L298N's 5V output** to the Arduino Nano's VIN pin to power the Arduino.

## Software Requirements
- **Arduino IDE**
- **PID Library** by Brett Beauregard (can be installed via Arduino Library Manager)

## Setup Instructions
1. **Assemble** the robot chassis, mount the motors, wheels, and place the components securely.
2. **Connect** the components as per the pinout above (Arduino, L298N, and ultrasonic sensor).
3. **Install** the Arduino IDE and required PID library.
4. **Open** the provided Arduino code in the IDE.
5. **Connect** the Arduino Nano via USB to your computer.
6. In the IDE, select **Arduino Nano** as the board and choose the appropriate **port**.
7. **Upload** the code to the Arduino Nano.
8. **Power on** the robot using the 9V battery or suitable power source.

## Usage Instructions
1. **Power on** the robot by connecting the battery.
2. The robot will start in **Exploration Mode**, moving forward and randomly changing directions every few seconds.
3. The robot will also avoid obstacles detected by the ultrasonic sensor, adjusting its course automatically.
4. The system continuously adjusts PID parameters for smoother operation as it explores.
5. You can monitor distance values and PID adjustments via the Serial Monitor for debugging.

## Code Explanation

- **Exploration Mode**: The robot dynamically changes its direction (left, right, or forward) every 3 seconds using a random number generator. This is handled in the `explore()` function.
  
- **Obstacle Avoidance**: The ultrasonic sensor continuously measures the distance to obstacles. If an obstacle is detected within a threshold distance, the `avoidObstacle()` function is triggered to steer the robot away from the object.

- **PID Control**: 
   - **Auto-tuning**: Initial PID parameters are tuned for optimal motor control using the PID library.
   - **Real-time adjustment**: The PID control parameters are updated continuously during operation to ensure smooth and responsive movement.

- **Interrupt-driven Distance Measurement**: The ultrasonic sensor uses an interrupt on the `ECHO` pin to measure distance, ensuring the robot reacts swiftly to nearby obstacles.

- **Movement Functions**: 
   - `moveForward()`: Moves the robot forward.
   - `turnLeft()` and `turnRight()`: Turn functions to steer the robot when required.
   - `stopMotors()`: Stops the motors if needed.

## Customization

You can modify the following parameters in the code to alter the robot’s behavior:

- `BASE_SPEED`: Adjust the base speed of the motors for normal movement.
- `MAX_SPEED`: Set the maximum allowable speed.
- `OBSTACLE_DISTANCE`: Set the minimum distance from an obstacle before triggering the avoidance behavior.
- `TURN_DURATION`: Modify the duration of turns for obstacle avoidance and exploration.
- `MOVE_CHANGE_INTERVAL`: Change how often the robot randomly changes direction in exploration mode (default: 3 seconds).

## Troubleshooting

- **Robot not moving?** Double-check all wiring, particularly connections between the Arduino, motor driver, and power source.
- **Ultrasonic sensor not detecting obstacles?** Ensure the sensor is properly connected, and the TRIG and ECHO pins are correct.
- **Erratic movement after startup?** This may occur during the initial PID tuning process. If persistent, adjust PID parameters or use a simpler tuning method.
- **Inconsistent obstacle avoidance?** Check the ultrasonic sensor’s field of view and make sure the sensor is not blocked.

## Safety Precautions

- Always operate the robot in a safe, open space with no fragile items nearby.
- Be prepared to disconnect the power if the robot behaves unexpectedly.
- Keep fingers, wires, and loose objects away from the wheels and motors while the robot is powered on.

## Future Improvements

- Implement advanced pathfinding algorithms for more intelligent navigation.
- Integrate additional sensors (e.g., infrared or cameras) for enhanced environmental awareness.
- Add wireless control using Bluetooth, Wi-Fi, or RF modules for remote operation.
- Create a smartphone app or web interface to monitor and control the robot in real-time.

## Acknowledgments
- Special thanks to Brett Beauregard for the PID Library.
- Gratitude to the Arduino community for resources and inspiration.
