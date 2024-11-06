#include "MotorController.h"
#include "UltrasonicSensor.h"
#include "ObstacleAvoidance.h"
#include "RobotArm.h"

// Pin definitions for mobile base
const uint8_t MOTOR1_IN1 = 3;
const uint8_t MOTOR1_IN2 = 4;
const uint8_t MOTOR2_IN1 = 5;
const uint8_t MOTOR2_IN2 = 6;
const uint8_t MOTOR1_ENA = 9;
const uint8_t MOTOR2_ENB = 10;
const uint8_t TRIG_PIN = 12;
const uint8_t ECHO_PIN = 2;

const int BASE_PIN = 13;
const int SHOULDER_PIN = 7;
const int ELBOW_PIN = 8;
const int GRIPPER_PIN = 11;

MotorController motors(MOTOR1_IN1, MOTOR1_IN2, MOTOR2_IN1, MOTOR2_IN2, MOTOR1_ENA, MOTOR2_ENB);
UltrasonicSensor sensor(TRIG_PIN, ECHO_PIN);
ObstacleAvoidance oa(&motors, &sensor);
RobotArm arm(BASE_PIN, SHOULDER_PIN, ELBOW_PIN, GRIPPER_PIN);

String command = "";

void setup() {
    Serial.begin(115200);
    motors.begin();
    sensor.begin();
    oa.begin();
    arm.begin();
    Serial.println(" ");
}

void loop() {
    if (oa.isActive()) {
        oa.check();
    }

    if (Serial.available() > 0) {
        command = Serial.readStringUntil('\n');
        command.trim();
        command.toLowerCase(); // Make commands case-insensitive

        if (arm.isRecording()) {
            processRecordingMode(command);
        } else {
            executeCommand(command);
        }
    }
}

// Functions for different command modes
void processRecordingMode(String command) {
    if (command == "done") {
        arm.stopRecording();
    } else if (command == "play") {
        arm.executeRecordedCommands();
    } else if (command == "clear") {
        arm.clearRecordedCommands();
    } else {
        arm.processRecordedCommand(command);
    }
}

void executeCommand(String command) {
    // Motor control commands
    if (command == "mv") { motors.moveForward(); }
    else if (command == "bk") { motors.moveBackward(); }
    else if (command == "lt") { motors.turnLeft(); }
    else if (command == "rt") { motors.turnRight(); }
    else if (command == "rl") { motors.rotateLeft(); }
    else if (command == "rr") { motors.rotateRight(); }
    else if (command == "st") { motors.stop(); }
    
    // Speed command with argument
    else if (command.startsWith("spd ")) {
        int speed = command.substring(4).toInt();
        motors.setSpeed(speed);
        Serial.println("Speed set to: " + String(speed));
    }

    // Obstacle avoidance and navigation
    else if (command == "oa on") { oa.enable(); }
    else if (command == "oa off") { oa.disable(); }
    else if (command == "oa nav") {
        startNavigationMode();
    }

    // Distance measurement
    else if (command == "dist") {
        float distance = sensor.getFilteredDistance(5);
        Serial.println("Distance: " + String(distance) + " cm");
    }

    // Arm joint control
    else if (command.length() >= 3) {
        handleArmCommands(command);
    } 
    else {
        Serial.println("Invalid command.");
    }
}

// Function to handle arm movement and joint commands
void handleArmCommands(String command) {
    char type = command.charAt(0);
    char action = command.charAt(2);

    switch (type) {
        case 'b': case 's': case 'e':
            arm.moveJoint(type, action);
            break;
        case 'g':
            arm.moveGripper(action);
            break;
        case 'm':
            processMovementOrSave(command, action);
            break;
        case 'p':
            if (action == 's') { arm.printSavedPositions(); }
            break;
    }

    if (command == "stream") {
        arm.startRecording();
    }

    if (type == 'b' || type == 's' || type == 'e' || type == 'g') {
        arm.printCurrentAngles();
    }
}

void startNavigationMode() {
    oa.enable();
    Serial.println("Starting autonomous navigation");
    while (oa.isActive()) {
        oa.navigate();
        if (Serial.available() > 0) {
            String stopCmd = Serial.readStringUntil('\n');
            stopCmd.trim();
            if (stopCmd == "st") break;
        }
    }
    motors.stop();
    Serial.println("Navigation stopped");
}

void processMovementOrSave(String command, char action) {
    if (command.startsWith("m pos")) {
        int posNum = command.substring(6).toInt();
        arm.saveCurrentPosition(posNum);
    } else if (command.startsWith("m save")) {
        int posNum = command.substring(6).toInt();
        arm.executeSavedPosition(posNum);
    } else {
        processArmMovement(action);
    }
}

void processArmMovement(char movement) {
    switch (movement) {
        case 'h': arm.moveToHome(); break;
        case 's': arm.performScan(); break;
        case 'p': arm.performPick(); break;
        case 'd': arm.performDrop(); break;
        case 'w': arm.performWave(); break;
        case 'b': arm.performBow(); break;
        case 'r': arm.performReach(); break;
        default:
            Serial.println("Invalid arm movement command.");
            break;
    }
}
