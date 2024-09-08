#ifndef SENSORCONTROL_H
#define SENSORCONTROL_H

#define RED 1 // Define constants for color identification

void initSensors();
int readColorSensor();
float readUltrasonicSensor();

#endif
