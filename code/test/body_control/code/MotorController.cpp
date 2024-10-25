#include "MotorController.h"

MotorController::MotorController(uint8_t in1, uint8_t in2, uint8_t in3, uint8_t in4, uint8_t enA, uint8_t enB) {
    in1Pin = in1;
    in2Pin = in2;
    in3Pin = in3;
    in4Pin = in4;
    enAPin = enA;
    enBPin = enB;
    currentSpeed = 200;
}

void MotorController::begin() {
    pinMode(in1Pin, OUTPUT);
    pinMode(in2Pin, OUTPUT);
    pinMode(in3Pin, OUTPUT);
    pinMode(in4Pin, OUTPUT);
    pinMode(enAPin, OUTPUT);
    pinMode(enBPin, OUTPUT);
    stop();
}

void MotorController::moveForward() {
    digitalWrite(in1Pin, HIGH);
    digitalWrite(in2Pin, LOW);
    digitalWrite(in3Pin, HIGH);
    digitalWrite(in4Pin, LOW);
    analogWrite(enAPin, currentSpeed);
    analogWrite(enBPin, currentSpeed);
}

void MotorController::moveBackward() {
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, HIGH);
    digitalWrite(in3Pin, LOW);
    digitalWrite(in4Pin, HIGH);
    analogWrite(enAPin, currentSpeed);
    analogWrite(enBPin, currentSpeed);
}

void MotorController::turnLeft() {
    digitalWrite(in1Pin, HIGH);
    digitalWrite(in2Pin, LOW);
    digitalWrite(in3Pin, HIGH);
    digitalWrite(in4Pin, LOW);
    analogWrite(enAPin, currentSpeed / 2);
    analogWrite(enBPin, currentSpeed);
}

void MotorController::turnRight() {
    digitalWrite(in1Pin, HIGH);
    digitalWrite(in2Pin, LOW);
    digitalWrite(in3Pin, HIGH);
    digitalWrite(in4Pin, LOW);
    analogWrite(enAPin, currentSpeed);
    analogWrite(enBPin, currentSpeed / 2);
}

void MotorController::rotateLeft() {
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, HIGH);
    digitalWrite(in3Pin, HIGH);
    digitalWrite(in4Pin, LOW);
    analogWrite(enAPin, currentSpeed);
    analogWrite(enBPin, currentSpeed);
}

void MotorController::rotateRight() {
    digitalWrite(in1Pin, HIGH);
    digitalWrite(in2Pin, LOW);
    digitalWrite(in3Pin, LOW);
    digitalWrite(in4Pin, HIGH);
    analogWrite(enAPin, currentSpeed);
    analogWrite(enBPin, currentSpeed);
}

void MotorController::stop() {
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, LOW);
    digitalWrite(in3Pin, LOW);
    digitalWrite(in4Pin, LOW);
    analogWrite(enAPin, 0);
    analogWrite(enBPin, 0);
}

void MotorController::setSpeed(int speed) {
    currentSpeed = constrain(speed, 0, 255);
}

int MotorController::getSpeed() {
    return currentSpeed;
}