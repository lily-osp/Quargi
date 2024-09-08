#include "BaseControl.h"

// Motor driver pins (L298N)
const int motor1Pin1 = 3;
const int motor1Pin2 = 4;
const int motor2Pin1 = 5;
const int motor2Pin2 = 6;
const int enable1 = 9; // PWM pin for speed control of motor 1
const int enable2 = 10; // PWM pin for speed control of motor 2

void initBase() {
    pinMode(motor1Pin1, OUTPUT);
    pinMode(motor1Pin2, OUTPUT);
    pinMode(motor2Pin1, OUTPUT);
    pinMode(motor2Pin2, OUTPUT);
    pinMode(enable1, OUTPUT);
    pinMode(enable2, OUTPUT);

    analogWrite(enable1, 255);  // Full speed
    analogWrite(enable2, 255);  // Full speed
}

void moveForward() {
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
}

void moveBackward() {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
}

void turnLeft() {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
}

void turnRight() {
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
}

void stop() {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
}

void navigate(float distance) {
    if (distance < 20) {  // Threshold for obstacle detection
        stop();
        delay(500);
        turnRight();
        delay(1000);
        stop();
    } else {
        moveForward();
    }
}
