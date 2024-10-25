#include "UltrasonicSensor.h"

UltrasonicSensor::UltrasonicSensor(uint8_t trig, uint8_t echo) {
    trigPin = trig;
    echoPin = echo;
    lastReadTime = 0;
    lastDistance = 0;
}

void UltrasonicSensor::begin() {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

float UltrasonicSensor::getDistance() {
    unsigned long currentTime = millis();
    if (currentTime - lastReadTime >= READ_INTERVAL) {
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
        
        long duration = pulseIn(echoPin, HIGH);
        lastDistance = duration * 0.034 / 2;
        lastReadTime = currentTime;
    }
    return lastDistance;
}

float UltrasonicSensor::getFilteredDistance(int samples) {
    float sum = 0;
    for (int i = 0; i < samples; i++) {
        sum += getDistance();
        delay(10);
    }
    return sum / samples;
}