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

// Pin definitions for robot arm
const int BASE_PIN = 13;
const int SHOULDER_PIN = 7;
const int ELBOW_PIN = 8;
const int GRIPPER_PIN = 11;

// Create objects
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
        command.toLowerCase(); // Make commands case-insensitive

        if (arm.isRecording()) {
            processRecordingMode(command);
        } else {
            executeCommand(command);
        }
    }
}

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
    // Base movement commands
    if (command == "mv") {
        motors.moveForward();
        Serial.println("Moving forward");
    }
    else if (command == "bk") {
        motors.moveBackward();
        Serial.println("Moving backward");
    }
    else if (command == "lt") {
        motors.turnLeft();
        Serial.println("Turning left");
    }
    else if (command == "rt") {
        motors.turnRight();
        Serial.println("Turning right");
    }
    else if (command == "rl") {
        motors.rotateLeft();
        Serial.println("Rotating left");
    }
    else if (command == "rr") {
        motors.rotateRight();
        Serial.println("Rotating right");
    }
    else if (command == "st") {
        motors.stop();
        Serial.println("Stopping");
    }
    // Base speed commands
    else if (command.startsWith("spd ")) {
        int speed = command.substring(4).toInt();
        motors.setSpeed(speed);
        Serial.println("Speed set to: " + String(speed));
    }
    // Obstacle avoidance commands
    else if (command == "oa on") {
        oa.enable();
        Serial.println("Obstacle avoidance enabled");
    }
    else if (command == "oa off") {
        oa.disable();
        Serial.println("Obstacle avoidance disabled");
    }
    else if (command == "oa nav") {
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
    else if (command == "dist") {
        float distance = sensor.getFilteredDistance(5);
        Serial.println("Distance: " + String(distance) + " cm");
    }
    // Arm commands
    else if (command.length() >= 3) {
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
                if (command.startsWith("m pos")) {
                    int posNum = command.substring(6).toInt();
                    arm.saveCurrentPosition(posNum);
                } else if (command.startsWith("m save")) {
                    int posNum = command.substring(6).toInt();
                    arm.executeSavedPosition(posNum);
                } else {
                    processArmMovement(action);
                }
                break;

            case 'p':
                if (action == 's') {
                    arm.printSavedPositions();
                }
                break;
        }

        if (command == "stream") {
            arm.startRecording();
        }

        if (type == 'b' || type == 's' || type == 'e' || type == 'g') {
            arm.printCurrentAngles();
        }
    }
    else {
        Serial.println("Invalid command. Type 'help' for available commands.");
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
