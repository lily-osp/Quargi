#include "RobotArm.h"

// Pin definitions
const int BASE_PIN = 13;
const int SHOULDER_PIN = 7;
const int ELBOW_PIN = 8;
const int GRIPPER_PIN = 11;

// Enable or disable the help and error message feature
bool enableHelpAndErrorMessages = true;
bool enableSerialOutput = true; // Set this to false to disable all serial printing

// Create robot arm instance with serial control
class CustomRobotArm : public RobotArm {
public:
  using RobotArm::RobotArm;

  // Override any method in RobotArm that uses Serial.print
  void printCurrentAngles() {
    if (enableSerialOutput) {
      RobotArm::printCurrentAngles();
    }
  }

  void printSavedPositions() {
    if (enableSerialOutput) {
      RobotArm::printSavedPositions();
    }
  }
};

CustomRobotArm arm(BASE_PIN, SHOULDER_PIN, ELBOW_PIN, GRIPPER_PIN);

void setup() {
  Serial.begin(115200);
  arm.begin();
  if (enableHelpAndErrorMessages && enableSerialOutput) {
    printHelp();
  }
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    command.toLowerCase();  // Make command case-insensitive

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
        if (command.startsWith("m pos")) {
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
        if (action == 'h' && enableHelpAndErrorMessages && enableSerialOutput) {
          printHelp();
        } else if (action == 's') {
          arm.printSavedPositions();
        }
        break;
      default:
        if (enableHelpAndErrorMessages && enableSerialOutput) {
          Serial.println("Invalid command. Type 'p h' for help.");
        }
        break;
    }

    if (command == "stream") {
      arm.startRecording();
    }

    if (enableSerialOutput) {
      arm.printCurrentAngles();
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
    case 'b':  // Bow
      arm.performBow();
      break;
    case 'r':  // Reach
      arm.performReach();
      break;
    default:
      if (enableHelpAndErrorMessages && enableSerialOutput) {
        Serial.println("Invalid movement command.");
      }
      break;
  }
}

void printHelp() {
  if (enableSerialOutput) {
    Serial.println("\nRobot Arm Control Commands:");
    Serial.println("1. Joint Control:");
    Serial.println("   b/s/e [+/-] - Move base/shoulder/elbow");
    Serial.println("   g [o/c] - Gripper open/close");
    Serial.println("2. Movements:");
    Serial.println("   m h - Move to home");
    Serial.println("   m s - Perform scan");
    Serial.println("   m p - Perform pick");
    Serial.println("   m d - Perform drop");
    Serial.println("   m w - Perform wave");
    Serial.println("   m b - Perform bow");
    Serial.println("   m r - Perform reach");
    Serial.println("3. Position Management:");
    Serial.println("   m pos [num] - Save current position");
    Serial.println("   m save [num] - Execute saved position");
    Serial.println("4. Misc:");
    Serial.println("   p h - Print help");
    Serial.println("   p s - Print saved positions");
    Serial.println("   stream - Start recording commands");
    Serial.println("   done - Stop recording");
    Serial.println("   play - Play recorded commands");
    Serial.println("   clear - Clear recorded commands");
  }
}
