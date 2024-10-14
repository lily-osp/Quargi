#include "SensorControl.h"
#include <Wire.h>
#include <Adafruit_TCS34725.h>

// Initialize color sensor (TCS34725)
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);

// Ultrasonic sensor pins
const int trigPin = 12;
const int echoPin = 2;

void initSensors() {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    if (tcs.begin()) {
        Serial.println("Color sensor initialized.");
    } else {
        Serial.println("Color sensor not found.");
    }
}

int readColorSensor() {
    uint16_t r, g, b, c;
    tcs.getRawData(&r, &g, &b, &c);

    if (r > g && r > b && r > 1500) {  // Adjust threshold values
        return RED;
    } else {
        return 0;  // No target color found
    }
}

float readUltrasonicSensor() {
    long duration;
    float distance;

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = (duration * 0.034) / 2;

    return distance;
}
