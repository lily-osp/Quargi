# ESP32 Camera Object Detection System

This code is an implementation of an object detection system using an ESP32 camera module. The system is designed to detect red objects in the camera's field of view and send their X and Y positions to a master device via I2C communication.

## Components

- **ESP32 Camera**: The camera module is used to capture images and detect red objects.

## Camera Parameters

- **Image Dimensions**: The camera's image dimensions are set to 320x240 pixels.
- **Tolerance**: The tolerance for object detection is set to 20 pixels.

## Functions

- **initCamera**: This function initializes the ESP32 camera module with the specified configuration.
- **detectRed**: This function detects red objects in the camera's field of view and returns their average X and Y positions.
- **onRequest**: This function is called when the master device requests data. It captures an image, detects red objects, and sends their positions to the master device via I2C communication.

## Setup and Loop Functions

- **setup**: This function initializes the serial communication, I2C communication as a slave, registers the onRequest callback, and initializes the camera module.
- **loop**: This function can be empty, as the main action is handled in the onRequest function. A delay of 100 milliseconds is added to adjust the loop's frequency if necessary.
