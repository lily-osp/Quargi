// RobotArm.cpp
#include "RobotArm.h"

RobotArm::RobotArm(int bPin, int sPin, int ePin, int gPin) {
  basePin = bPin;
  shoulderPin = sPin;
  elbowPin = ePin;
  gripperPin = gPin;

  baseAngle = HOME_BASE;
  shoulderAngle = HOME_SHOULDER;
  elbowAngle = HOME_ELBOW;
  gripperAngle = HOME_GRIPPER;

  commandCount = 0;
  recording = false;
}

void RobotArm::begin() {
  baseServo.attach(basePin);
  shoulderServo.attach(shoulderPin);
  elbowServo.attach(elbowPin);
  gripperServo.attach(gripperPin);

  loadPositionsFromEEPROM();
  moveToHome();
}

void RobotArm::moveJoint(char joint, char direction) {
  switch (joint) {
    case 'b':
      moveServo(baseServo, direction, &baseAngle);
      break;
    case 's':
      moveServo(shoulderServo, direction, &shoulderAngle);
      break;
    case 'e':
      moveServo(elbowServo, direction, &elbowAngle);
      break;
  }
}

void RobotArm::moveGripper(char action) {
  if (action == 'o') {
    gripperAngle = GRIPPER_OPEN;
    gripperServo.write(gripperAngle);
  }
  else if (action == 'c') {
    gripperAngle = GRIPPER_CLOSE;
    gripperServo.write(gripperAngle);
  }
}

void RobotArm::moveServo(Servo &servo, char direction, int *currentAngle) {
  int newAngle = *currentAngle;

  if (direction == '+') {
    newAngle = min(newAngle + STEP_ANGLE, MAX_ANGLE);
  }
  else if (direction == '-') {
    newAngle = max(newAngle - STEP_ANGLE, MIN_ANGLE);
  }

  if (newAngle != *currentAngle) {
    *currentAngle = newAngle;
    servo.write(newAngle);
    delay(15);
  }
}

void RobotArm::moveToAngle(Servo &servo, int *currentAngle, int targetAngle) {
  targetAngle = constrain(targetAngle, MIN_ANGLE, MAX_ANGLE);
  *currentAngle = targetAngle;
  servo.write(targetAngle);
  delay(500);
}

void RobotArm::moveToHome() {
  moveToAngle(baseServo, &baseAngle, HOME_BASE);
  moveToAngle(shoulderServo, &shoulderAngle, HOME_SHOULDER);
  moveToAngle(elbowServo, &elbowAngle, HOME_ELBOW);
  moveToAngle(gripperServo, &gripperAngle, HOME_GRIPPER);
}

// Predefined movements
void RobotArm::performScan() {
  moveToHome();
  for (int angle = 0; angle <= 180; angle += 45) {
    moveToAngle(baseServo, &baseAngle, angle);
    delay(500);
  }
  moveToAngle(baseServo, &baseAngle, HOME_BASE);
}

void RobotArm::performPick() {
  moveToAngle(gripperServo, &gripperAngle, GRIPPER_OPEN);
  moveToAngle(shoulderServo, &shoulderAngle, 45);
  moveToAngle(elbowServo, &elbowAngle, 45);
  moveToAngle(gripperServo, &gripperAngle, GRIPPER_CLOSE);
  moveToAngle(shoulderServo, &shoulderAngle, 90);
  moveToAngle(elbowServo, &elbowAngle, 90);
}

void RobotArm::performDrop() {
  moveToAngle(baseServo, &baseAngle, 180);
  moveToAngle(shoulderServo, &shoulderAngle, 45);
  moveToAngle(elbowServo, &elbowAngle, 45);
  moveToAngle(gripperServo, &gripperAngle, GRIPPER_OPEN);
  moveToHome();
}

void RobotArm::performWave() {
  moveToAngle(baseServo, &baseAngle, 90);
  moveToAngle(shoulderServo, &shoulderAngle, 45);
  moveToAngle(elbowServo, &elbowAngle, 0);

  for (int i = 0; i < 3; i++) {
    moveToAngle(elbowServo, &elbowAngle, 45);
    moveToAngle(elbowServo, &elbowAngle, 0);
  }

  moveToHome();
}

void RobotArm::performBow() {
  moveToHome();
  moveToAngle(shoulderServo, &shoulderAngle, 60);
  moveToAngle(elbowServo, &elbowAngle, 30);
  delay(1000);
  moveToAngle(shoulderServo, &shoulderAngle, 0);
  moveToAngle(elbowServo, &elbowAngle, 0);
  moveToHome();
}

void RobotArm::performReach() {
  moveToHome();
  moveToAngle(shoulderServo, &shoulderAngle, 180);
  moveToAngle(elbowServo, &elbowAngle, 135);
  delay(1000);
  moveToAngle(gripperServo, &gripperAngle, GRIPPER_CLOSE);
  delay(500);
  moveToHome();
}

// Position memory
void RobotArm::saveCurrentPosition(int posNum) {
  if (posNum >= 1 && posNum <= 3) {
    int index = posNum - 1;
    savedPositions[index] = {baseAngle, shoulderAngle, elbowAngle, gripperAngle};
    positionUsed[index] = true;
    savePositionsToEEPROM();
    Serial.println("Position " + String(posNum) + " saved");
  } else {
    Serial.println("Invalid position number (use 1-3)");
  }
}

void RobotArm::executeSavedPosition(int posNum) {
  if (posNum >= 1 && posNum <= 3) {
    int index = posNum - 1;
    if (positionUsed[index]) {
      Position pos = savedPositions[index];
      moveToAngle(baseServo, &baseAngle, pos.base);
      moveToAngle(shoulderServo, &shoulderAngle, pos.shoulder);
      moveToAngle(elbowServo, &elbowAngle, pos.elbow);
      moveToAngle(gripperServo, &gripperAngle, pos.gripper);
      Serial.println("Moved to saved position " + String(posNum));
    } else {
      Serial.println("Position " + String(posNum) + " not yet saved");
    }
  }
}

// Command recording
void RobotArm::startRecording() {
  recording = true;
  commandCount = 0;
  Serial.println("Recording started");
}

void RobotArm::stopRecording() {
  recording = false;
  Serial.println("Recording stopped. Total commands: " + String(commandCount));
}

void RobotArm::processRecordedCommand(String command) {
  if (commandCount < MAX_COMMANDS) {
    recordedCommands[commandCount++] = command;
    Serial.println("Command recorded: " + command);
  } else {
    Serial.println("Maximum commands reached!");
  }
}

void RobotArm::executeRecordedCommands() {
  Serial.println("Executing recorded commands...");
  for (int i = 0; i < commandCount; i++) {
    Serial.println("Executing: " + recordedCommands[i]);
    // Command processing logic will be handled in main sketch
    delay(500);
  }
  Serial.println("Execution completed");
}

void RobotArm::clearRecordedCommands() {
  commandCount = 0;
  Serial.println("Recorded commands cleared");
}

// EEPROM operations
void RobotArm::savePositionsToEEPROM() {
  for (int i = 0; i < 3; i++) {
    int addr = i * sizeof(Position);
    EEPROM.put(addr, savedPositions[i]);
    EEPROM.put(addr + sizeof(Position), positionUsed[i]);
  }
}

void RobotArm::loadPositionsFromEEPROM() {
  for (int i = 0; i < 3; i++) {
    int addr = i * sizeof(Position);
    EEPROM.get(addr, savedPositions[i]);
    EEPROM.get(addr + sizeof(Position), positionUsed[i]);
  }
}

void RobotArm::printCurrentAngles() {
  Serial.println("\nCurrent angles:");
  Serial.print("Base: "); Serial.println(baseAngle);
  Serial.print("Shoulder: "); Serial.println(shoulderAngle);
  Serial.print("Elbow: "); Serial.println(elbowAngle);
  Serial.print("Gripper: "); Serial.print(gripperAngle);
  Serial.println(gripperAngle == GRIPPER_OPEN ? " (Open)" : " (Closed)");
}

void RobotArm::printSavedPositions() {
  Serial.println("\nSaved Positions:");
  for (int i = 0; i < 3; i++) {
    if (positionUsed[i]) {
      Serial.print(i + 1); Serial.println(":");
      Serial.print("  Base: "); Serial.println(savedPositions[i].base);
      Serial.print("  Shoulder: "); Serial.println(savedPositions[i].shoulder);
      Serial.print("  Elbow: "); Serial.println(savedPositions[i].elbow);
      Serial.print("  Gripper: "); Serial.println(savedPositions[i].gripper);
    } else {
      Serial.println(String(i + 1) + ": [Empty]");
    }
  }
}
