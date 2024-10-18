# Adaptive Unified Robot Control System with ESP32 Camera Object Detection

This code is an implementation of an adaptive unified robot control system that integrates various components such as servos, motors, and an ultrasonic sensor to enable object tracking and obstacle avoidance. The system utilizes a PID (Proportional-Integral-Derivative) controller for base and arm movement, and an ESP32 camera for object detection. The ESP32 camera module is used to capture images and detect red objects, sending their X and Y positions to the master device via I2C communication.

## Components

- **Servos**: Four servos are used to control the base, shoulder, elbow, and gripper movements of the robot.
- **Motors**: Two motors are used for base movement, allowing the robot to move forward, turn right, and turn left.
- **Ultrasonic Sensor**: The sensor is used to measure distances to obstacles, enabling the robot to avoid collisions.
- **ESP32 Camera**: The camera module is used to capture images and detect red objects.

## Movement Parameters

- **Base Speed**: The speed at which the base moves is set to 150.
- **Max Speed**: The maximum speed for the motors is set to 255.
- **Obstacle Distance**: The distance threshold for obstacle detection is set to 15 cm.
- **Target Distance**: The desired distance to maintain from the object is set to 30 cm.

## Servo Angle Limits

- **Base**: The base servo can rotate between 0 and 180 degrees.
- **Shoulder**: The shoulder servo can rotate between 0 and 180 degrees.
- **Elbow**: The elbow servo can rotate between 0 and 180 degrees.

## Camera Parameters

- **Image Dimensions**: The camera's image dimensions are set to 320x240 pixels.
- **Center Coordinates**: The center of the image is defined as (160, 120) pixels.
- **Tolerance**: The tolerance for object centering is set to 20 pixels.

## PID Variables

- **Base Movement**: The PID variables for base movement are defined with initial values of Kp=2, Ki=5, and Kd=1.
- **Arm Movement**: The PID variables for arm movement are defined with initial values of Kp=2, Ki=5, and Kd=1.

## Functions

- **echoInterrupt**: This function is called when the echo pin of the ultrasonic sensor changes state, allowing for distance measurement.
- **getDistance**: This function calculates and returns the distance measured by the ultrasonic sensor.
- **moveForward**: This function controls the speed and direction of the motors to move the robot forward.
- **turnRight**: This function turns the robot right by setting the left motor to maximum speed and the right motor to the opposite maximum speed.
- **turnLeft**: This function turns the robot left by setting the right motor to maximum speed and the left motor to the opposite maximum speed.
- **stopMotors**: This function stops both motors by setting their speed to 0.
- **avoidObstacle**: This function checks the distance to obstacles and turns the robot right or left to avoid them.
- **adjustArmPosition**: This function adjusts the arm's position based on the object's X and Y positions using PID control.
- **moveToHome**: This function moves the robot to its home position, which is defined as the middle position for each servo.
- **moveBaseAndArm**: This function moves the base and arm based on the object's X position using PID control.
- **adjustPIDParameters**: This function adjusts the PID parameters based on the distance to the object.
- **initCamera**: This function initializes the ESP32 camera module with the specified configuration.
- **detectRed**: This function detects red objects in the camera's field of view and returns their average X and Y positions.
- **onRequest**: This function is called when the master device requests data. It captures an image, detects red objects, and sends their positions to the master device via I2C communication.

## Setup and Loop Functions

- **setup**: This function initializes the I2C communication, servos, motor control pins, ultrasonic sensor pins, PIDs, and the ESP32 camera module. It also moves the robot to its home position.
- **loop**: This function requests object data from the ESP32 camera, triggers the ultrasonic sensor, and performs object tracking and obstacle avoidance based on the received data. The main action for the ESP32 camera module is handled in the onRequest function.
