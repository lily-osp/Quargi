#ifndef OBSTACLE_AVOIDANCE_H
#define OBSTACLE_AVOIDANCE_H

#include "MotorController.h"
#include "UltrasonicSensor.h"

class ObstacleAvoidance {
  private:
    MotorController* motors;
    UltrasonicSensor* sensor;
    bool isEnabled;
    float stopDistance;
    float turnDistance;
    float criticalDistance;
    unsigned long lastCheckTime;
    const unsigned long CHECK_INTERVAL = 100; // 100ms between checks
    
  public:
    ObstacleAvoidance(MotorController* m, UltrasonicSensor* s);
    void begin();
    void enable();
    void disable();
    bool isActive();
    void setDistances(float stop, float turn, float critical);
    bool check();
    void navigate();
};

#endif