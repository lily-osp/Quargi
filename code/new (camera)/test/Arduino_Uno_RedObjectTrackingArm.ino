#include <Wire.h>
#include <Servo.h>

// Define servos
Servo baseServo;
Servo shoulderServo;
Servo elbowServo;
Servo gripperServo;

// Define pin assignments
const int BASE_SERVO_PIN = 2;
const int SHOULDER_SERVO_PIN = 7;
const int ELBOW_SERVO_PIN = 8;
const int GRIPPER_SERVO_PIN = 11;

// Define movement parameters
const int BASE_MIN_ANGLE = 0;
const int BASE_MAX_ANGLE = 180;
const int SHOULDER_MIN_ANGLE = 0;
const int SHOULDER_MAX_ANGLE = 180;
const int ELBOW_MIN_ANGLE = 0;
const int ELBOW_MAX_ANGLE = 180;

// Define middle (home) positions for each servo
const int BASE_HOME_ANGLE = 90;
const int SHOULDER_HOME_ANGLE = 90;
const int ELBOW_HOME_ANGLE = 90;

// Define I2C parameters
const int I2C_SLAVE_ADDRESS = 0x08;

// Define camera parameters
const int IMAGE_WIDTH = 320;
const int IMAGE_HEIGHT = 240;
const int CENTER_X = IMAGE_WIDTH / 2;
const int CENTER_Y = IMAGE_HEIGHT / 2;
const int TOLERANCE = 20;

// Current positions
int currentBaseAngle = BASE_HOME_ANGLE;
int currentShoulderAngle = SHOULDER_HOME_ANGLE;
int currentElbowAngle = ELBOW_HOME_ANGLE;

void setup() {
  Serial.begin(9600);

  // Initialize servos
  baseServo.attach(BASE_SERVO_PIN);
  shoulderServo.attach(SHOULDER_SERVO_PIN);
  elbowServo.attach(ELBOW_SERVO_PIN);
  gripperServo.attach(GRIPPER_SERVO_PIN);

  // Initialize I2C communication
  Wire.begin(I2C_SLAVE_ADDRESS);
  Wire.onReceive(receiveEvent);

  // Move to home position
  moveToHome();

  Serial.println("Red Object Tracking Arm Initialized");
}

void loop() {
  // Main loop is empty as we're using interrupt-driven I2C communication
}

void receiveEvent(int byteCount) {
  if (byteCount == 2) {
    int posX = Wire.read();
    int posY = Wire.read();

    if (posX == 0xFF && posY == 0xFF) {
      Serial.println("Red object not detected");
      return;
    }

    Serial.print("Received position: X=");
    Serial.print(posX);
    Serial.print(", Y=");
    Serial.println(posY);

    adjustArmPosition(posX, posY);
  }
}

void adjustArmPosition(int posX, int posY) {
  // Adjust base (left-right movement)
  if (abs(posX - CENTER_X) > TOLERANCE) {
    if (posX < CENTER_X) {
      currentBaseAngle = min(currentBaseAngle + 1, BASE_MAX_ANGLE);
    } else {
      currentBaseAngle = max(currentBaseAngle - 1, BASE_MIN_ANGLE);
    }
    baseServo.write(currentBaseAngle);
  }

  // Adjust shoulder and elbow (up-down movement)
  if (abs(posY - CENTER_Y) > TOLERANCE) {
    if (posY < CENTER_Y) {
      currentShoulderAngle = max(currentShoulderAngle - 1, SHOULDER_MIN_ANGLE);
      currentElbowAngle = min(currentElbowAngle + 1, ELBOW_MAX_ANGLE);
    } else {
      currentShoulderAngle = min(currentShoulderAngle + 1, SHOULDER_MAX_ANGLE);
      currentElbowAngle = max(currentElbowAngle - 1, ELBOW_MIN_ANGLE);
    }
    shoulderServo.write(currentShoulderAngle);
    elbowServo.write(currentElbowAngle);
  }

  Serial.print("New angles: Base=");
  Serial.print(currentBaseAngle);
  Serial.print(", Shoulder=");
  Serial.print(currentShoulderAngle);
  Serial.print(", Elbow=");
  Serial.println(currentElbowAngle);
}

void moveToHome() {
  baseServo.write(BASE_HOME_ANGLE);
  shoulderServo.write(SHOULDER_HOME_ANGLE);
  elbowServo.write(ELBOW_HOME_ANGLE);
  currentBaseAngle = BASE_HOME_ANGLE;
  currentShoulderAngle = SHOULDER_HOME_ANGLE;
  currentElbowAngle = ELBOW_HOME_ANGLE;
  delay(1000); // Allow time for servos to reach home position
}
