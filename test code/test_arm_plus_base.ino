#include <Servo.h>

// Define servos
Servo baseServo;
Servo shoulderServo;
Servo elbowServo;
Servo gripperServo;

// Define servo pin assignments
const int BASE_SERVO_PIN = 2;
const int SHOULDER_SERVO_PIN = 7;
const int ELBOW_SERVO_PIN = 8;
const int GRIPPER_SERVO_PIN = 11;

// Define arm movement parameters
const int BASE_MIN_ANGLE = 0;
const int BASE_MAX_ANGLE = 180;
const int SHOULDER_MIN_ANGLE = 0;
const int SHOULDER_MAX_ANGLE = 180;
const int ELBOW_MIN_ANGLE = 0;
const int ELBOW_MAX_ANGLE = 180;
const int GRIPPER_OPEN_ANGLE = 0;
const int GRIPPER_CLOSE_ANGLE = 90;

// Define middle (home) positions for each servo
const int BASE_HOME_ANGLE = (BASE_MIN_ANGLE + BASE_MAX_ANGLE) / 2;
const int SHOULDER_HOME_ANGLE = (SHOULDER_MIN_ANGLE + SHOULDER_MAX_ANGLE) / 2;
const int ELBOW_HOME_ANGLE = (ELBOW_MIN_ANGLE + ELBOW_MAX_ANGLE) / 2;
const int GRIPPER_HOME_ANGLE = GRIPPER_OPEN_ANGLE;

// Define delay parameters
const int DELAY_MS = 50;
const int GRIPPER_DELAY_MS = 200;

// Define motor control pins
const int MOTOR1_IN1_PIN = 3;
const int MOTOR1_IN2_PIN = 4;
const int MOTOR2_IN3_PIN = 5;
const int MOTOR2_IN4_PIN = 6;
const int MOTOR1_EN_PIN = 9;
const int MOTOR2_EN_PIN = 10;

// Define ultrasonic sensor pins
#define TRIG_PIN 12
#define ECHO_PIN 13

// Define movement parameters
const int MOTOR_SPEED = 200;
const int MOVE_DURATION = 2000;
const int OBSTACLE_DISTANCE = 15;
const int TURN_DELAY = 1000;

void setup() {
  // Initialize servos
  baseServo.attach(BASE_SERVO_PIN);
  shoulderServo.attach(SHOULDER_SERVO_PIN);
  elbowServo.attach(ELBOW_SERVO_PIN);
  gripperServo.attach(GRIPPER_SERVO_PIN);

  // Initialize motor control pins as outputs
  pinMode(MOTOR1_IN1_PIN, OUTPUT);
  pinMode(MOTOR1_IN2_PIN, OUTPUT);
  pinMode(MOTOR2_IN3_PIN, OUTPUT);
  pinMode(MOTOR2_IN4_PIN, OUTPUT);
  pinMode(MOTOR1_EN_PIN, OUTPUT);
  pinMode(MOTOR2_EN_PIN, OUTPUT);

  // Initialize ultrasonic sensor pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.begin(9600);
  Serial.println("Starting Combined Robot Arm and Base Test...");

  // Seed the random generator
  randomSeed(analogRead(0));
}

void loop() {
  // Test arm movements
  testArmMovements();

  // Test base movements with obstacle avoidance
  testBaseMovements();
}

// Function to test arm movements
void testArmMovements() {
  Serial.println("Testing Arm Movements...");

  // Initial testing of each servo one by one
  Serial.println("Initial Testing of Each Servo One by One...");
  testBaseServo();
  testShoulderServo();
  testElbowServo();
  testGripperServo();

  delay(3000);

  backToHome();

  // Dynamic and random testing
  Serial.println("Starting Random and Combined Arm Movements...");
  performRandomArmMovements();

  delay(1000);
}

// Function to test base movements with obstacle avoidance
void testBaseMovements() {
  Serial.println("Testing Base Movements with Obstacle Avoidance...");

  // Read distance from ultrasonic sensor
  float distance = readUltrasonic();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance > OBSTACLE_DISTANCE) {
    // Test forward movement
    Serial.println("Moving Forward...");
    moveForward();
    delay(MOVE_DURATION);

    // Test backward movement
    Serial.println("Moving Backward...");
    moveBackward();
    delay(MOVE_DURATION);

    // Test left turn
    Serial.println("Turning Left...");
    turnLeft();
    delay(MOVE_DURATION);

    // Test right turn
    Serial.println("Turning Right...");
    turnRight();
    delay(MOVE_DURATION);

    // Stop motors
    Serial.println("Stopping Motors...");
    stopMotors();
    delay(2000);
  } else {
    // If obstacle detected, stop and turn
    Serial.println("Obstacle detected. Stopping and turning...");
    stopMotors();
    delay(TURN_DELAY);
    turnRight();
    delay(TURN_DELAY);
  }
}

// Arm movement functions
void testBaseServo() {
  Serial.println("Testing Base Servo...");
  moveServo(baseServo, BASE_MIN_ANGLE, BASE_MAX_ANGLE);
}

void testShoulderServo() {
  Serial.println("Testing Shoulder Servo...");
  moveServo(shoulderServo, SHOULDER_MIN_ANGLE, SHOULDER_MAX_ANGLE);
}

void testElbowServo() {
  Serial.println("Testing Elbow Servo...");
  moveServo(elbowServo, ELBOW_MIN_ANGLE, ELBOW_MAX_ANGLE);
}

void testGripperServo() {
  Serial.println("Testing Gripper Servo...");
  testGripper(gripperServo);
}

void moveServo(Servo &servo, int minAngle, int maxAngle) {
  for (int pos = minAngle; pos <= maxAngle; pos += 10) {
    servo.write(pos);
    delay(DELAY_MS);
  }
  for (int pos = maxAngle; pos >= minAngle; pos -= 10) {
    servo.write(pos);
    delay(DELAY_MS);
  }
}

void testGripper(Servo &servo) {
  servo.write(GRIPPER_OPEN_ANGLE);
  delay(GRIPPER_DELAY_MS);
  servo.write(GRIPPER_CLOSE_ANGLE);
  delay(GRIPPER_DELAY_MS);
}

void performRandomArmMovements() {
  for (int i = 0; i < 5; i++) {
    int randomTest = random(1, 5);

    switch (randomTest) {
      case 1:
        Serial.println("Random Base Servo Movement...");
        randomMoveServo(baseServo, BASE_MIN_ANGLE, BASE_MAX_ANGLE);
        break;
      case 2:
        Serial.println("Random Shoulder Servo Movement...");
        randomMoveServo(shoulderServo, SHOULDER_MIN_ANGLE, SHOULDER_MAX_ANGLE);
        break;
      case 3:
        Serial.println("Random Elbow Servo Movement...");
        randomMoveServo(elbowServo, ELBOW_MIN_ANGLE, ELBOW_MAX_ANGLE);
        break;
      case 4:
        Serial.println("Random Gripper Movement...");
        randomGripper(gripperServo);
        break;
    }

    delay(500);
  }

  Serial.println("Performing Combined Random Movement...");
  combinedRandomMovement();

  Serial.println("Performing Complex Movement Sequences...");
  complexMovementSequence1();
  complexMovementSequence2();
  complexMovementSequence3();
  complexMovementSequence4();

  backToHome();
}

void randomMoveServo(Servo &servo, int minAngle, int maxAngle) {
  int randomAngle = random(minAngle, maxAngle);
  servo.write(randomAngle);
  delay(random(100, 500));
}

void randomGripper(Servo &servo) {
  int randomGripperAction = random(0, 2);
  if (randomGripperAction == 0) {
    servo.write(GRIPPER_OPEN_ANGLE);
  } else {
    servo.write(GRIPPER_CLOSE_ANGLE);
  }
  delay(GRIPPER_DELAY_MS);
}

void combinedRandomMovement() {
  int basePos = random(BASE_MIN_ANGLE, BASE_MAX_ANGLE);
  int shoulderPos = random(SHOULDER_MIN_ANGLE, SHOULDER_MAX_ANGLE);
  int elbowPos = random(ELBOW_MIN_ANGLE, ELBOW_MAX_ANGLE);

  baseServo.write(basePos);
  shoulderServo.write(shoulderPos);
  elbowServo.write(elbowPos);

  delay(random(200, 700));
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

// Base movement and sensor functions
float readUltrasonic() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = (duration * 0.034) / 2;
  return distance;
}

void moveForward() {
  analogWrite(MOTOR1_EN_PIN, MOTOR_SPEED);
  analogWrite(MOTOR2_EN_PIN, MOTOR_SPEED);
  digitalWrite(MOTOR1_IN1_PIN, HIGH);
  digitalWrite(MOTOR1_IN2_PIN, LOW);
  digitalWrite(MOTOR2_IN3_PIN, HIGH);
  digitalWrite(MOTOR2_IN4_PIN, LOW);
}

void moveBackward() {
  analogWrite(MOTOR1_EN_PIN, MOTOR_SPEED);
  analogWrite(MOTOR2_EN_PIN, MOTOR_SPEED);
  digitalWrite(MOTOR1_IN1_PIN, LOW);
  digitalWrite(MOTOR1_IN2_PIN, HIGH);
  digitalWrite(MOTOR2_IN3_PIN, LOW);
  digitalWrite(MOTOR2_IN4_PIN, HIGH);
}

void turnLeft() {
  analogWrite(MOTOR1_EN_PIN, MOTOR_SPEED);
  analogWrite(MOTOR2_EN_PIN, MOTOR_SPEED);
  digitalWrite(MOTOR1_IN1_PIN, LOW);
  digitalWrite(MOTOR1_IN2_PIN, HIGH);
  digitalWrite(MOTOR2_IN3_PIN, HIGH);
  digitalWrite(MOTOR2_IN4_PIN, LOW);
}

void turnRight() {
  analogWrite(MOTOR1_EN_PIN, MOTOR_SPEED);
  analogWrite(MOTOR2_EN_PIN, MOTOR_SPEED);
  digitalWrite(MOTOR1_IN1_PIN, HIGH);
  digitalWrite(MOTOR1_IN2_PIN, LOW);
  digitalWrite(MOTOR2_IN3_PIN, LOW);
  digitalWrite(MOTOR2_IN4_PIN, HIGH);
}

void stopMotors() {
  analogWrite(MOTOR1_EN_PIN, 0);
  analogWrite(MOTOR2_EN_PIN, 0);
  digitalWrite(MOTOR1_IN1_PIN, LOW);
  digitalWrite(MOTOR1_IN2_PIN, LOW);
  digitalWrite(MOTOR2_IN3_PIN, LOW);
  digitalWrite(MOTOR2_IN4_PIN, LOW);
}
