// main.ino
#include "RobotArm.h"

// Pin definitions
const int BASE_PIN = 13;
const int SHOULDER_PIN = 7;
const int ELBOW_PIN = 8;
const int GRIPPER_PIN = 11;

// Create robot arm instance
RobotArm arm(BASE_PIN, SHOULDER_PIN, ELBOW_PIN, GRIPPER_PIN);

void setup() {
  Serial.begin(9600);
  arm.begin();
  printHelp();
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (arm.isRecording()) {
      processRecordingMode(command);
    } else {
      processCommand(command);
    }
  }
}

void processCommand(String command) {
  if (command.length() >= 3) {
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
        if (action == 's') {  // Save position
          int posNum = command.substring(6).toInt();
          arm.saveCurrentPosition(posNum);
        } else if (command.startsWith("m save")) {
          int posNum = command.substring(6).toInt();
          arm.executeSavedPosition(posNum);
        } else {
          processMovementCommand(action);
        }
        break;

      case 'p':
        if (action == 'h') {
          printHelp();
        } else if (action == 's') {
          arm.printSavedPositions();
        }
        break;
    }

    if (command == "stream") {
      arm.startRecording();
    }

    arm.printCurrentAngles();
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

void processMovementCommand(char movement) {
  switch (movement) {
    case 'h':  // Home position
      arm.moveToHome();
      break;
    case 's':  // Scan
      arm.performScan();
      break;
    case 'p':  // Pick
      arm.performPick();
      break;
    case 'd':  // Drop
      arm.performDrop();
      break;
    case 'w':  // Wave
      arm.performWave();
      break;
    default:
      Serial.println("Invalid movement command.");
      break;
  }
}

void printHelp() {
  Serial.println("\nRobot Arm Control Commands:");
  Serial.println("1. Joint Control:");
  Serial.println("   b/s/e [+/-] - Move base/shoulder/elbow");
  Serial.println("   g [o/c] - Gripper open/close");
  Serial.println("2. Movements:");
  Serial.println("   m h - Home position");
  Serial.println("   m s - Scan");
  Serial.println("   m p - Pick");
  Serial.println("   m d - Drop");
  Serial.println("   m w - Wave");
  Serial.println("3. Position Memory:");
  Serial.println("   m pos ## - Save current position (1-3)");
  Serial.println("   m save ## - Move to saved position (1-3)");
  Serial.println("4. Command Recording:");
  Serial.println("   stream - Start recording commands");
  Serial.println("   done - Stop recording");
  Serial.println("   play - Execute recorded commands");
  Serial.println("   clear - Clear recorded commands");
  Serial.println("5. Information:");
  Serial.println("   p h - Print this help");
  Serial.println("   p s - Print saved positions");
}

