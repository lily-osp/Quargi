#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include <Arduino.h>

class UltrasonicSensor {
  private:
    uint8_t trigPin, echoPin;
    unsigned long lastReadTime;
    float lastDistance;
    const unsigned long READ_INTERVAL = 50; // 50ms between readings
    
  public:
    UltrasonicSensor(uint8_t trig, uint8_t echo);
    void begin();
    float getDistance();
    float getFilteredDistance(int samples = 3);
};

#endif