#include "MotorController.h"
#include "UltrasonicSensor.h"
#include "ObstacleAvoidance.h"

// Pin definitions
const uint8_t MOTOR1_IN1 = 3;
const uint8_t MOTOR1_IN2 = 4;
const uint8_t MOTOR2_IN1 = 5;
const uint8_t MOTOR2_IN2 = 6;
const uint8_t MOTOR1_ENA = 9;
const uint8_t MOTOR2_ENB = 10;
const uint8_t TRIG_PIN = 12;
const uint8_t ECHO_PIN = 2;

// Enable or disable command printing and invalid command handling
bool enableCommandFeedback = false;
bool enableSerialOutput = false; // Set this to false to disable all serial printing

// Create objects
MotorController motors(MOTOR1_IN1, MOTOR1_IN2, MOTOR2_IN1, MOTOR2_IN2, MOTOR1_ENA, MOTOR2_ENB);
UltrasonicSensor sensor(TRIG_PIN, ECHO_PIN);
ObstacleAvoidance oa(&motors, &sensor);

String command = "";

void setup() {
    Serial.begin(115200);
    motors.begin();
    sensor.begin();
    oa.begin();
    if (enableSerialOutput) {
        printCommands();
    }
}

void loop() {
    // Check obstacle avoidance if enabled
    if (oa.isActive()) {
        oa.check();
    }

    // Read serial commands
    if (Serial.available() > 0) {
        command = Serial.readStringUntil('\n');
        command.trim();
        executeCommand(command);
    }
}

void executeCommand(String cmd) {
    // Movement commands
    if (cmd == "mv") {
        motors.moveForward();
        if (enableSerialOutput) Serial.println("Moving forward");
    }
    else if (cmd == "bk") {
        motors.moveBackward();
        if (enableSerialOutput) Serial.println("Moving backward");
    }
    else if (cmd == "lt") {
        motors.turnLeft();
        if (enableSerialOutput) Serial.println("Turning left");
    }
    else if (cmd == "rt") {
        motors.turnRight();
        if (enableSerialOutput) Serial.println("Turning right");
    }
    else if (cmd == "rl") {
        motors.rotateLeft();
        if (enableSerialOutput) Serial.println("Rotating left");
    }
    else if (cmd == "rr") {
        motors.rotateRight();
        if (enableSerialOutput) Serial.println("Rotating right");
    }
    else if (cmd == "st") {
        motors.stop();
        if (enableSerialOutput) Serial.println("Stopping");
    }
    // Speed commands
    else if (cmd.startsWith("spd ")) {
        int speed = cmd.substring(4).toInt();
        motors.setSpeed(speed);
        if (enableSerialOutput) Serial.println("Speed set to: " + String(speed));
    }
    // Obstacle avoidance commands
    else if (cmd == "oa on") {
        oa.enable();
        if (enableSerialOutput) Serial.println("Obstacle avoidance enabled");
    }
    else if (cmd == "oa off") {
        oa.disable();
        if (enableSerialOutput) Serial.println("Obstacle avoidance disabled");
    }
    else if (cmd == "oa nav") {
        oa.enable();
        if (enableSerialOutput) Serial.println("Starting autonomous navigation");
        while (oa.isActive()) {
            oa.navigate();
            if (Serial.available() > 0) {
                String stopCmd = Serial.readStringUntil('\n');
                stopCmd.trim(); // Ensure stopCmd is trimmed
                if (stopCmd == "st") break;
            }
        }
        motors.stop();
        if (enableSerialOutput) Serial.println("Navigation stopped");
    }
    else if (cmd == "dist") {
        float distance = sensor.getFilteredDistance(5);
        if (enableSerialOutput) Serial.println("Distance: " + String(distance) + " cm");
    }
    else if (cmd == "help") {
        if (enableCommandFeedback && enableSerialOutput) printCommands();
    }
    else {
        if (enableCommandFeedback && enableSerialOutput) {
            Serial.println("Invalid command. Type 'help' for available commands.");
        }
    }
}

void printCommands() {
    Serial.println("\nAvailable commands:");
    Serial.println("Movement commands:");
    Serial.println("  mv  - Move forward");
    Serial.println("  bk  - Move backward");
    Serial.println("  lt  - Turn left");
    Serial.println("  rt  - Turn right");
    Serial.println("  rl  - Rotate left");
    Serial.println("  rr  - Rotate right");
    Serial.println("  st  - Stop motors");
    Serial.println("\nSpeed control:");
    Serial.println("  spd <0-255> - Set motor speed");
    Serial.println("\nObstacle avoidance:");
    Serial.println("  oa on   - Enable obstacle avoidance");
    Serial.println("  oa off  - Disable obstacle avoidance");
    Serial.println("  oa nav  - Start autonomous navigation");
    Serial.println("  dist    - Read distance sensor");
    Serial.println("  help    - Show this help message");
}
