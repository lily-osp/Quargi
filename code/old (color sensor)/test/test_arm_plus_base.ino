#include <Servo.h>

// Servo Definitions
Servo baseServo, shoulderServo, elbowServo, gripperServo;

// Pin Assignments
const int BASE_SERVO_PIN = 2, SHOULDER_SERVO_PIN = 7, ELBOW_SERVO_PIN = 8, GRIPPER_SERVO_PIN = 11;
const int MOTOR1_IN1_PIN = 3, MOTOR1_IN2_PIN = 4, MOTOR2_IN3_PIN = 5, MOTOR2_IN4_PIN = 6;
const int MOTOR1_EN_PIN = 9, MOTOR2_EN_PIN = 10;
const int TRIG_PIN = 12, ECHO_PIN = 13;

// Servo Movement Parameters
const int BASE_MIN_ANGLE = 0, BASE_MAX_ANGLE = 180;
const int SHOULDER_MIN_ANGLE = 0, SHOULDER_MAX_ANGLE = 180;
const int ELBOW_MIN_ANGLE = 0, ELBOW_MAX_ANGLE = 180;
const int GRIPPER_OPEN_ANGLE = 0, GRIPPER_CLOSE_ANGLE = 90;

// Home Positions
const int BASE_HOME_ANGLE = (BASE_MIN_ANGLE + BASE_MAX_ANGLE) / 2;
const int SHOULDER_HOME_ANGLE = (SHOULDER_MIN_ANGLE + SHOULDER_MAX_ANGLE) / 2;
const int ELBOW_HOME_ANGLE = (ELBOW_MIN_ANGLE + ELBOW_MAX_ANGLE) / 2;
const int GRIPPER_HOME_ANGLE = GRIPPER_OPEN_ANGLE;

// Timing Parameters
const int DELAY_MS = 50, GRIPPER_DELAY_MS = 200;
const int MOVE_DURATION = 2000, TURN_DELAY = 1000;

// Movement Parameters
const int MOTOR_SPEED = 200;
const int OBSTACLE_DISTANCE = 15;

// Obstacle detection parameters
const unsigned long OBSTACLE_CHECK_INTERVAL = 100; // Check every 100ms
unsigned long lastObstacleCheckTime = 0;
bool obstacleDetected = false;

void setup() {
  initializeServos();
  initializeMotors();
  initializeUltrasonicSensor();
  initializeSerialAndRandom();
}

void loop() {
  unsigned long currentTime = millis();

  // Check for obstacles periodically
  if (currentTime - lastObstacleCheckTime >= OBSTACLE_CHECK_INTERVAL) {
    checkForObstacles();
    lastObstacleCheckTime = currentTime;
  }

  if (obstacleDetected) {
    handleObstacle();
  } else {
    testArmMovements();
    testBaseMovements();
  }
}

// Initialization Functions
void initializeServos() {
  baseServo.attach(BASE_SERVO_PIN);
  shoulderServo.attach(SHOULDER_SERVO_PIN);
  elbowServo.attach(ELBOW_SERVO_PIN);
  gripperServo.attach(GRIPPER_SERVO_PIN);
}

void initializeMotors() {
  pinMode(MOTOR1_IN1_PIN, OUTPUT);
  pinMode(MOTOR1_IN2_PIN, OUTPUT);
  pinMode(MOTOR2_IN3_PIN, OUTPUT);
  pinMode(MOTOR2_IN4_PIN, OUTPUT);
  pinMode(MOTOR1_EN_PIN, OUTPUT);
  pinMode(MOTOR2_EN_PIN, OUTPUT);
}

void initializeUltrasonicSensor() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void initializeSerialAndRandom() {
  Serial.begin(9600);
  Serial.println("Starting Combined Robot Arm and Base Test with Periodic Obstacle Avoidance...");
  randomSeed(analogRead(0));
}

// Obstacle Detection and Handling
void checkForObstacles() {
  float distance = readUltrasonic();
  obstacleDetected = (distance < OBSTACLE_DISTANCE);
}

void handleObstacle() {
  Serial.println("Obstacle detected! Stopping and turning...");
  stopMotors();
  delay(TURN_DELAY);
  turnRight();
  delay(TURN_DELAY);
  stopMotors();
}

// Arm Movement Functions
void testArmMovements() {
  Serial.println("Testing Arm Movements...");
  testIndividualServos();
  delay(3000);
  backToHome();
  performRandomArmMovements();
  delay(1000);
}

void testIndividualServos() {
  Serial.println("Initial Testing of Each Servo One by One...");
  testServo("Base", baseServo, BASE_MIN_ANGLE, BASE_MAX_ANGLE);
  testServo("Shoulder", shoulderServo, SHOULDER_MIN_ANGLE, SHOULDER_MAX_ANGLE);
  testServo("Elbow", elbowServo, ELBOW_MIN_ANGLE, ELBOW_MAX_ANGLE);
  testGripperServo();
}

void testServo(const char* servoName, Servo& servo, int minAngle, int maxAngle) {
  Serial.print("Testing ");
  Serial.print(servoName);
  Serial.println(" Servo...");
  moveServo(servo, minAngle, maxAngle);
}

void moveServo(Servo& servo, int minAngle, int maxAngle) {
  for (int pos = minAngle; pos <= maxAngle; pos += 10) {
    servo.write(pos);
    delay(DELAY_MS);
  }
  for (int pos = maxAngle; pos >= minAngle; pos -= 10) {
    servo.write(pos);
    delay(DELAY_MS);
  }
}

void testGripperServo() {
  Serial.println("Testing Gripper Servo...");
  gripperServo.write(GRIPPER_OPEN_ANGLE);
  delay(GRIPPER_DELAY_MS);
  gripperServo.write(GRIPPER_CLOSE_ANGLE);
  delay(GRIPPER_DELAY_MS);
}

void performRandomArmMovements() {
  Serial.println("Starting Random and Combined Arm Movements...");
  for (int i = 0; i < 5; i++) {
    performRandomMove();
    delay(500);
  }
  performComplexMovements();
  backToHome();
}

void performRandomMove() {
  int randomTest = random(1, 5);
  switch (randomTest) {
    case 1: randomMoveServo("Base", baseServo, BASE_MIN_ANGLE, BASE_MAX_ANGLE); break;
    case 2: randomMoveServo("Shoulder", shoulderServo, SHOULDER_MIN_ANGLE, SHOULDER_MAX_ANGLE); break;
    case 3: randomMoveServo("Elbow", elbowServo, ELBOW_MIN_ANGLE, ELBOW_MAX_ANGLE); break;
    case 4: randomGripper(); break;
  }
}

void randomMoveServo(const char* servoName, Servo& servo, int minAngle, int maxAngle) {
  Serial.print("Random ");
  Serial.print(servoName);
  Serial.println(" Servo Movement...");
  int randomAngle = random(minAngle, maxAngle);
  servo.write(randomAngle);
  delay(random(100, 500));
}

void randomGripper() {
  Serial.println("Random Gripper Movement...");
  gripperServo.write(random(0, 2) == 0 ? GRIPPER_OPEN_ANGLE : GRIPPER_CLOSE_ANGLE);
  delay(GRIPPER_DELAY_MS);
}

void performComplexMovements() {
  Serial.println("Performing Complex Movement Sequences...");
  complexMovementSequence1();
  complexMovementSequence2();
  complexMovementSequence3();
  complexMovementSequence4();
}

void complexMovementSequence1() {
  Serial.println("Complex Movement 1: Rotate and Reach");
  baseServo.write(90);
  shoulderServo.write(45);
  delay(500);
  elbowServo.write(90);
  delay(500);
  gripperServo.write(GRIPPER_CLOSE_ANGLE);
  delay(GRIPPER_DELAY_MS);
  backToHome();
}

void complexMovementSequence2() {
  Serial.println("Complex Movement 2: Pick and Drop");
  baseServo.write(45);
  shoulderServo.write(60);
  delay(400);
  elbowServo.write(120);
  delay(400);
  gripperServo.write(GRIPPER_CLOSE_ANGLE);
  delay(GRIPPER_DELAY_MS);
  elbowServo.write(60);
  baseServo.write(BASE_HOME_ANGLE);
  delay(400);
  gripperServo.write(GRIPPER_OPEN_ANGLE);
  delay(GRIPPER_DELAY_MS);
  backToHome();
}

void complexMovementSequence3() {
  Serial.println("Complex Movement 3: Scan Left to Right");
  for (int angle = BASE_MIN_ANGLE; angle <= BASE_MAX_ANGLE; angle += 20) {
    baseServo.write(angle);
    delay(300);
  }
  for (int angle = BASE_MAX_ANGLE; angle >= BASE_MIN_ANGLE; angle -= 20) {
    baseServo.write(angle);
    delay(300);
  }
  backToHome();
}

void complexMovementSequence4() {
  Serial.println("Complex Movement 4: Wave Motion");
  shoulderServo.write(90);
  elbowServo.write(90);
  for (int i = 0; i < 3; i++) {
    shoulderServo.write(70);
    delay(300);
    shoulderServo.write(110);
    delay(300);
  }
  backToHome();
}

void backToHome() {
  Serial.println("Returning to Home Position...");
  baseServo.write(BASE_HOME_ANGLE);
  shoulderServo.write(SHOULDER_HOME_ANGLE);
  elbowServo.write(ELBOW_HOME_ANGLE);
  gripperServo.write(GRIPPER_HOME_ANGLE);
  delay(500);
}

// Base Movement Functions
void testBaseMovements() {
  Serial.println("Testing Base Movements with Obstacle Avoidance...");
  float distance = readUltrasonic();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance > OBSTACLE_DISTANCE) {
    performBaseMovements();
  } else {
    handleObstacle();
  }
}

void performBaseMovements() {
  moveAndPause("Moving Forward...", moveForward);
  moveAndPause("Moving Backward...", moveBackward);
  moveAndPause("Turning Left...", turnLeft);
  moveAndPause("Turning Right...", turnRight);
  Serial.println("Stopping Motors...");
  stopMotors();
  delay(2000);
}

void moveAndPause(const char* message, void (*moveFunction)()) {
  Serial.println(message);
  moveFunction();
  delay(MOVE_DURATION);
}

// Ultrasonic Sensor Function
float readUltrasonic() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  return (duration * 0.034) / 2;
}

// Motor Control Functions
void moveForward() {
  setMotorDirection(HIGH, LOW, HIGH, LOW);
}

void moveBackward() {
  setMotorDirection(LOW, HIGH, LOW, HIGH);
}

void turnLeft() {
  setMotorDirection(LOW, HIGH, HIGH, LOW);
}

void turnRight() {
  setMotorDirection(HIGH, LOW, LOW, HIGH);
}

void stopMotors() {
  analogWrite(MOTOR1_EN_PIN, 0);
  analogWrite(MOTOR2_EN_PIN, 0);
  setMotorDirection(LOW, LOW, LOW, LOW);
}

void setMotorDirection(int in1, int in2, int in3, int in4) {
  digitalWrite(MOTOR1_IN1_PIN, in1);
  digitalWrite(MOTOR1_IN2_PIN, in2);
  digitalWrite(MOTOR2_IN3_PIN, in3);
  digitalWrite(MOTOR2_IN4_PIN, in4);
  analogWrite(MOTOR1_EN_PIN, MOTOR_SPEED);
  analogWrite(MOTOR2_EN_PIN, MOTOR_SPEED);
}
