#include <Wire.h>
#include <Servo.h>
#include <PID_v1.h>

// I2C address for ESP32 camera
#define ESP32_CAMERA_ADDRESS 0x30

// Define servos
Servo baseServo;
Servo shoulderServo;
Servo elbowServo;
Servo gripperServo;

// Define servo pins
const int BASE_SERVO_PIN = 13;
const int SHOULDER_SERVO_PIN = 7;
const int ELBOW_SERVO_PIN = 8;
const int GRIPPER_SERVO_PIN = 11;

// Define motor control pins
const int MOTOR1_IN1_PIN = 3;
const int MOTOR1_IN2_PIN = 4;
const int MOTOR2_IN3_PIN = 5;
const int MOTOR2_IN4_PIN = 6;
const int MOTOR1_EN_PIN = 9;
const int MOTOR2_EN_PIN = 10;

// Define ultrasonic sensor pins
const int TRIG_PIN = 12;
const int ECHO_PIN = 2;

// Define movement parameters
const int BASE_SPEED = 150;
const int MAX_SPEED = 255;
const int OBSTACLE_DISTANCE = 15; // cm
const int TARGET_DISTANCE = 30;   // cm, distance to maintain from the object

// Define servo angle limits
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

// Object tracking variables
int objectX = -1;
int objectY = -1;
bool objectDetected = false;

// PID variables for base movement
double baseSetpoint, baseInput, baseOutput;
double baseKp = 2, baseKi = 5, baseKd = 1;
PID basePID(&baseInput, &baseOutput, &baseSetpoint, baseKp, baseKi, baseKd, DIRECT);

// PID variables for arm movement
double armSetpoint, armInput, armOutput;
double armKp = 2, armKi = 5, armKd = 1;
PID armPID(&armInput, &armOutput, &armSetpoint, armKp, armKi, armKd, DIRECT);

// Variables for adaptive PID
double baseLastError = 0;
double armLastError = 0;
double baseErrorSum = 0;
double armErrorSum = 0;
unsigned long lastTime = 0;

// Variables for interrupt-based distance measurement
volatile unsigned long pulseStartTime = 0;
volatile unsigned long pulseDuration = 0;
volatile bool newDistanceAvailable = false;

// Function prototypes
void echoInterrupt();
float getDistance();
void moveForward(int leftSpeed, int rightSpeed);
void turnRight();
void turnLeft();
void stopMotors();
void adjustArmPosition(int posX, int posY);
void trackObject();
void moveToHome();
void adjustPIDParameters();
void moveBaseAndArm();

// Setup function
void setup() {
  Serial.begin(115200);

  // Initialize I2C as master
  Wire.begin();

  // Initialize servos
  baseServo.attach(BASE_SERVO_PIN);
  shoulderServo.attach(SHOULDER_SERVO_PIN);
  elbowServo.attach(ELBOW_SERVO_PIN);
  gripperServo.attach(GRIPPER_SERVO_PIN);

  // Initialize motor control pins
  pinMode(MOTOR1_IN1_PIN, OUTPUT);
  pinMode(MOTOR1_IN2_PIN, OUTPUT);
  pinMode(MOTOR2_IN3_PIN, OUTPUT);
  pinMode(MOTOR2_IN4_PIN, OUTPUT);
  pinMode(MOTOR1_EN_PIN, OUTPUT);
  pinMode(MOTOR2_EN_PIN, OUTPUT);

  // Initialize ultrasonic sensor pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(ECHO_PIN), echoInterrupt, CHANGE);

  // Initialize PIDs
  baseSetpoint = BASE_HOME_ANGLE;
  basePID.SetMode(AUTOMATIC);
  basePID.SetOutputLimits(-MAX_SPEED, MAX_SPEED);

  armSetpoint = CENTER_Y;
  armPID.SetMode(AUTOMATIC);
  armPID.SetOutputLimits(-90, 90);

  // Move to home position
  moveToHome();

  Serial.println("Adaptive Unified Robot Control System Initialized");
}

void loop() {
  // Request object data from ESP32 camera
  Wire.requestFrom(ESP32_CAMERA_ADDRESS, 3);
  if (Wire.available() == 3) {
    objectDetected = Wire.read();
    objectX = Wire.read();
    objectY = Wire.read();

    // Print object X and Y positions
    Serial.print("Object X: ");
    Serial.print(objectX);
    Serial.print(", Object Y: ");
    Serial.println(objectY);
  }

  // Trigger ultrasonic sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Wait for new distance measurement
  unsigned long startTime = millis();
  while (!newDistanceAvailable) {
    if (millis() - startTime > 50) {
      Serial.println("Distance measurement timeout");
      break;
    }
  }

  if (newDistanceAvailable) {
    float distance = getDistance();
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    if (distance < OBSTACLE_DISTANCE) {
      avoidObstacle();
    }

    if (objectDetected) {
      if (distance > OBSTACLE_DISTANCE) {
        adjustArmPosition(objectX, objectY);
        moveBaseAndArm();
        adjustPIDParameters();
      } else {
        stopMotors();
        Serial.println("Object reached or obstacle detected");
      }
    } else {
      stopMotors();
      moveToHome();
      Serial.println("No object detected");
    }

    newDistanceAvailable = false;
  }
}

void echoInterrupt() {
  if (digitalRead(ECHO_PIN) == HIGH) {
    pulseStartTime = micros();
  } else {
    if (pulseStartTime != 0) {
      pulseDuration = micros() - pulseStartTime;
      pulseStartTime = 0;
      newDistanceAvailable = true;
    }
  }
}

float getDistance() {
  noInterrupts();
  unsigned long duration = pulseDuration;
  interrupts();
  return (duration * 0.034) / 2;
}

void moveForward(int leftSpeed, int rightSpeed) {
  analogWrite(MOTOR1_EN_PIN, abs(leftSpeed));
  analogWrite(MOTOR2_EN_PIN, abs(rightSpeed));
  digitalWrite(MOTOR1_IN1_PIN, leftSpeed > 0 ? HIGH : LOW);
  digitalWrite(MOTOR1_IN2_PIN, leftSpeed > 0 ? LOW : HIGH);
  digitalWrite(MOTOR2_IN3_PIN, rightSpeed > 0 ? HIGH : LOW);
  digitalWrite(MOTOR2_IN4_PIN, rightSpeed > 0 ? LOW : HIGH);
}

void turnRight() {
  moveForward(MAX_SPEED, -MAX_SPEED);
}

void turnLeft() {
  moveForward(-MAX_SPEED, MAX_SPEED);
}

void stopMotors() {
  analogWrite(MOTOR1_EN_PIN, 0);
  analogWrite(MOTOR2_EN_PIN, 0);
  digitalWrite(MOTOR1_IN1_PIN, LOW);
  digitalWrite(MOTOR1_IN2_PIN, LOW);
  digitalWrite(MOTOR2_IN3_PIN, LOW);
  digitalWrite(MOTOR2_IN4_PIN, LOW);
}

void avoidObstacle() {
  float distance = getDistance();
  if (distance < OBSTACLE_DISTANCE) {
    // Turn right or left to avoid the obstacle
    if (random(2) == 0) {
      turnRight();
    } else {
      turnLeft();
    }
    delay(500); // Wait for a short period to avoid the obstacle
    stopMotors();
  }
}

void adjustArmPosition(int posX, int posY) {
  armInput = posY;
  armPID.Compute();

  // Adjust shoulder and elbow (up-down movement)
  int shoulderAdjustment = map(armOutput, -90, 90, -5, 5);
  int elbowAdjustment = -shoulderAdjustment; // Inverse movement for elbow

  currentShoulderAngle = constrain(currentShoulderAngle + shoulderAdjustment, SHOULDER_MIN_ANGLE, SHOULDER_MAX_ANGLE);
  currentElbowAngle = constrain(currentElbowAngle + elbowAdjustment, ELBOW_MIN_ANGLE, ELBOW_MAX_ANGLE);

  shoulderServo.write(currentShoulderAngle);
  elbowServo.write(currentElbowAngle);

  // Adjust base (left-right movement)
  currentBaseAngle = map(posX, 0, IMAGE_WIDTH, BASE_MIN_ANGLE, BASE_MAX_ANGLE);
  baseServo.write(currentBaseAngle);

  Serial.print("Arm angles - Base: ");
  Serial.print(currentBaseAngle);
  Serial.print(", Shoulder: ");
  Serial.print(currentShoulderAngle);
  Serial.print(", Elbow: ");
  Serial.println(currentElbowAngle);
}

void moveToHome() {
  baseServo.write(BASE_HOME_ANGLE);
  shoulderServo.write(SHOULDER_HOME_ANGLE);
  elbowServo.write(ELBOW_HOME_ANGLE);
  gripperServo.write(0); // Open gripper initially

  currentBaseAngle = BASE_HOME_ANGLE;
  currentShoulderAngle = SHOULDER_HOME_ANGLE;
  currentElbowAngle = ELBOW_HOME_ANGLE;
}

void moveBaseAndArm() {
  // PID base movement
  baseInput = objectX;
  baseSetpoint = CENTER_X;
  basePID.Compute();

  int baseSpeed = map(baseOutput, -180, 180, -MAX_SPEED, MAX_SPEED);
  moveForward(baseSpeed, baseSpeed);

  // If the object is centered, stop base movement
  if (abs(objectX - CENTER_X) < TOLERANCE) {
    stopMotors();
    Serial.println("Object centered");
  }
}

void adjustPIDParameters() {
  // Adjust PID based on distance to the object
  float distance = getDistance();
  baseKp = map(distance, 0, TARGET_DISTANCE, 2, 5);
  baseKi = map(distance, 0, TARGET_DISTANCE, 0.1, 2);
  baseKd = map(distance, 0, TARGET_DISTANCE, 0.01, 1);

  armKp = baseKp;
  armKi = baseKi;
  armKd = baseKd;

  basePID.SetTunings(baseKp, baseKi, baseKd);
  armPID.SetTunings(armKp, armKi, armKd);
}
