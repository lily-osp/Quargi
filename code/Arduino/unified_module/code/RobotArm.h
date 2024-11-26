// RobotArm.h
#ifndef ROBOT_ARM_H
#define ROBOT_ARM_H

#include <Arduino.h>
#include <Servo.h>
#include <EEPROM.h>

class RobotArm {
  public:
    RobotArm(int basePin, int shoulderPin, int elbowPin, int gripperPin);
    void begin();

    // Basic movement controls
    void moveJoint(char joint, char direction);
    void moveToHome();
    void moveGripper(char action);

    // Predefined movements
    void performScan();
    void performPick();
    void performDrop();
    void performWave();
    void performBow();
    void performReach();

    // Position memory
    void saveCurrentPosition(int posNum);
    void executeSavedPosition(int posNum);
    void printSavedPositions();

    // Command recording
    void startRecording();
    void stopRecording();
    void processRecordedCommand(String command);
    void executeRecordedCommands();
    void clearRecordedCommands();
    bool isRecording() { return recording; }

    // Status
    void printCurrentAngles();

  private:
    // Servo objects
    Servo baseServo;
    Servo shoulderServo;
    Servo elbowServo;
    Servo gripperServo;

    // Current angles
    int baseAngle;
    int shoulderAngle;
    int elbowAngle;
    int gripperAngle;

    // Pins
    int basePin;
    int shoulderPin;
    int elbowPin;
    int gripperPin;

    // Constants
    static const int STEP_ANGLE = 15;
    static const int MIN_ANGLE = 0;
    static const int MAX_ANGLE = 180;
    static const int GRIPPER_OPEN = 90;
    static const int GRIPPER_CLOSE = 60;
    static const int HOME_BASE = 90;
    static const int HOME_SHOULDER = 90;
    static const int HOME_ELBOW = 90;
    static const int HOME_GRIPPER = 90;
    static const int MAX_COMMANDS = 20;

    // Saved positions
    struct Position {
      int base;
      int shoulder;
      int elbow;
      int gripper;
    };
    Position savedPositions[3];
    bool positionUsed[3];

    // Command recording
    String recordedCommands[MAX_COMMANDS];
    int commandCount;
    bool recording;

    // Helper functions
    void moveServo(Servo &servo, char direction, int *currentAngle);
    void moveToAngle(Servo &servo, int *currentAngle, int targetAngle);
    void loadPositionsFromEEPROM();
    void savePositionsToEEPROM();
};

#endif
