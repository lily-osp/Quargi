#include "BaseControl.h"
#include "ArmControl.h"
#include "SensorControl.h"

const int TARGET_COLOR = RED;  // Define the target color
const unsigned long SCAN_INTERVAL = 60000; // Scan interval of 1 minute

unsigned long lastScanTime = 0;

void setup() {
    Serial.begin(9600);

    // Initialize modules
    initBase();
    initArm();
    initSensors();
}

void loop() {
    unsigned long currentTime = millis();

    // Read sensor data
    int detectedColor = readColorSensor();
    float distance = readUltrasonicSensor();

    if (detectedColor == TARGET_COLOR) {
        moveToObject();
        pickUpObject();
        moveToDropLocation();
        dropObject();
        lastScanTime = currentTime; // Reset scan time after action
    } else {
        if (currentTime - lastScanTime > SCAN_INTERVAL) {
            // Time to scan again
            scanForTarget();
            lastScanTime = currentTime;
        } else {
            // Sleep if no target detected
            sleepMode();
        }
    }
}
