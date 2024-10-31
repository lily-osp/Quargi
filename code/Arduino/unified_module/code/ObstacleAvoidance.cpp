#include "ObstacleAvoidance.h"

ObstacleAvoidance::ObstacleAvoidance(MotorController* m, UltrasonicSensor* s) {
    motors = m;
    sensor = s;
    isEnabled = false;
    stopDistance = 30.0;  // Stop if obstacle is closer than 30cm
    turnDistance = 50.0;  // Start turning if obstacle is closer than 50cm
    criticalDistance = 15.0; // Emergency stop and back up if closer than 15cm
    lastCheckTime = 0;
}

void ObstacleAvoidance::begin() {
    // Nothing to initialize
}

void ObstacleAvoidance::enable() {
    isEnabled = true;
}

void ObstacleAvoidance::disable() {
    isEnabled = false;
}

bool ObstacleAvoidance::isActive() {
    return isEnabled;
}

void ObstacleAvoidance::setDistances(float stop, float turn, float critical) {
    stopDistance = stop;
    turnDistance = turn;
    criticalDistance = critical;
}

bool ObstacleAvoidance::check() {
    if (!isEnabled) return true;
    
    unsigned long currentTime = millis();
    if (currentTime - lastCheckTime >= CHECK_INTERVAL) {
        float distance = sensor->getFilteredDistance(3);
        lastCheckTime = currentTime;
        
        if (distance <= criticalDistance) {
            motors->stop();
            delay(100);
            motors->moveBackward();
            delay(500);
            motors->rotateRight();
            delay(750);
            motors->stop();
            return false;
        }
        else if (distance <= stopDistance) {
            motors->stop();
            delay(100);
            motors->rotateRight();
            delay(500);
            motors->stop();
            return false;
        }
        else if (distance <= turnDistance) {
            motors->turnRight();
            return true;
        }
    }
    return true;
}

void ObstacleAvoidance::navigate() {
    if (!isEnabled) return;
    
    float distance = sensor->getFilteredDistance(3);
    
    if (distance <= criticalDistance) {
        // Emergency maneuver
        motors->stop();
        delay(100);
        motors->moveBackward();
        delay(1000);
        motors->rotateRight();
        delay(750);
    }
    else if (distance <= stopDistance) {
        // Find new path
        motors->stop();
        delay(100);
        motors->rotateRight();
        delay(500);
    }
    else if (distance <= turnDistance) {
        // Gentle turn
        motors->turnRight();
    }
    else {
        motors->moveForward();
    }
}