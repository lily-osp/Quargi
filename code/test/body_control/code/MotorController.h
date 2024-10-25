#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include <Arduino.h>

class MotorController {
  private:
    uint8_t in1Pin, in2Pin, in3Pin, in4Pin;
    uint8_t enAPin, enBPin;
    int currentSpeed;
    
  public:
    MotorController(uint8_t in1, uint8_t in2, uint8_t in3, uint8_t in4, uint8_t enA, uint8_t enB);
    void begin();
    void moveForward();
    void moveBackward();
    void turnLeft();
    void turnRight();
    void rotateLeft();
    void rotateRight();
    void stop();
    void setSpeed(int speed);
    int getSpeed();
};

#endif