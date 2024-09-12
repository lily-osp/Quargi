#include "ArmControl.h"
#include <Servo.h>

// Servos for the 4 DOF arm
Servo baseServo;
Servo shoulderServo;
Servo elbowServo;
Servo gripperServo;

// Servo pin definitions
const int basePin = 2;
const int shoulderPin = 7;
const int elbowPin = 8;
const int gripperPin = 11;

void initArm() {
    baseServo.attach(basePin);
    shoulderServo.attach(shoulderPin);
    elbowServo.attach(elbowPin);
    gripperServo.attach(gripperPin);

    // Set initial positions
    baseServo.write(90);
    shoulderServo.write(90);
    elbowServo.write(90);
    gripperServo.write(0); // Open gripper
}

void pickUpObject() {
    baseServo.write(90);    // Adjust for precise movement
    shoulderServo.write(45);
    elbowServo.write(135);
    gripperServo.write(50); // Close gripper
}

void dropObject() {
    baseServo.write(90);
    shoulderServo.write(90);
    elbowServo.write(90);
    gripperServo.write(0); // Open gripper
}

void moveToObject() {
    baseServo.write(90);
    shoulderServo.write(45);
    elbowServo.write(135);
}

void moveToDropLocation() {
    baseServo.write(180);
    shoulderServo.write(90);
    elbowServo.write(90);
}

void scanForTarget() {
    // Scan movement pattern to find target
    for (int pos = 0; pos <= 180; pos += 10) {
        baseServo.write(pos);
        delay(200);
        int detectedColor = readColorSensor();
        if (detectedColor == TARGET_COLOR) {
            return; // Stop scan if target found
        }
    }
}

void sleepMode() {
    stop(); // Stop all movements
    delay(60000); // Sleep for 1 minute
}
