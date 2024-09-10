#include <Servo.h>

// Define servos
Servo baseServo;
Servo shoulderServo;
Servo elbowServo;
Servo gripperServo;

// Define pin assignments
const int BASE_SERVO_PIN = 7;
const int SHOULDER_SERVO_PIN = 8;
const int ELBOW_SERVO_PIN = 9;
const int GRIPPER_SERVO_PIN = 11;

// Define movement parameters
const int BASE_MIN_ANGLE = 0;
const int BASE_MAX_ANGLE = 180;
const int SHOULDER_MIN_ANGLE = 0;
const int SHOULDER_MAX_ANGLE = 180;
const int ELBOW_MIN_ANGLE = 0;
const int ELBOW_MAX_ANGLE = 180;
const int GRIPPER_OPEN_ANGLE = 0;
const int GRIPPER_CLOSE_ANGLE = 90;

// Define middle (home) positions for each servo
const int BASE_HOME_ANGLE = (BASE_MIN_ANGLE + BASE_MAX_ANGLE) / 2;  // Middle position for base
const int SHOULDER_HOME_ANGLE = (SHOULDER_MIN_ANGLE + SHOULDER_MAX_ANGLE) / 2;  // Middle position for shoulder
const int ELBOW_HOME_ANGLE = (ELBOW_MIN_ANGLE + ELBOW_MAX_ANGLE) / 2;  // Middle position for elbow
const int GRIPPER_HOME_ANGLE = GRIPPER_OPEN_ANGLE;  // Open gripper is considered the home position

// Define delay parameters
const int DELAY_MS = 50;            // Delay for general servo movements
const int GRIPPER_DELAY_MS = 200;   // Dedicated delay for gripper movements

void setup() {
  // Initialize servos
  baseServo.attach(BASE_SERVO_PIN);
  shoulderServo.attach(SHOULDER_SERVO_PIN);
  elbowServo.attach(ELBOW_SERVO_PIN);
  gripperServo.attach(GRIPPER_SERVO_PIN);

  Serial.begin(9600);
  Serial.println("Starting Arm Evaluation Test...");

  // Seed the random generator
  randomSeed(analogRead(0));
}

void loop() {
  // Initial testing of each servo one by one
  Serial.println("Initial Testing of Each Servo One by One...");

  // Test each servo individually
  Serial.println("Testing Base Servo...");
  moveServo(baseServo, BASE_MIN_ANGLE, BASE_MAX_ANGLE);

  Serial.println("Testing Shoulder Servo...");
  moveServo(shoulderServo, SHOULDER_MIN_ANGLE, SHOULDER_MAX_ANGLE);

  Serial.println("Testing Elbow Servo...");
  moveServo(elbowServo, ELBOW_MIN_ANGLE, ELBOW_MAX_ANGLE);

  Serial.println("Testing Gripper Servo...");
  testGripper(gripperServo);

  delay(3000); // Wait for 3 seconds before starting random movements

  backToHome();

  // Dynamic and random testing
  Serial.println("Starting Random and Combined Movements...");

  // Perform random movements
  performRandomMovements();

  delay(1000);  // Wait for 1 second before the next loop
}

// Function to move a servo from min to max angle and back
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

// Function to test gripper servo (open and close) with dedicated delay
void testGripper(Servo &servo) {
  servo.write(GRIPPER_OPEN_ANGLE);  // Open gripper
  delay(GRIPPER_DELAY_MS);
  servo.write(GRIPPER_CLOSE_ANGLE); // Close gripper
  delay(GRIPPER_DELAY_MS);
}

// Function to perform random movements
void performRandomMovements() {
  // Randomize and test the servos in random order
  int randomTest = random(1, 5); // Generate a random number between 1 and 4

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

  // Perform combined random movements
  Serial.println("Performing Combined Random Movement...");
  combinedRandomMovement();

  // Perform more complex movements
  Serial.println("Performing Complex Movement Sequences...");
  complexMovementSequence1();
  complexMovementSequence2();
  complexMovementSequence3();
  complexMovementSequence4();

  // Return to the home position
  backToHome();
}

// Function to move a servo from min to max angle with random movements
void randomMoveServo(Servo &servo, int minAngle, int maxAngle) {
  int randomAngle = random(minAngle, maxAngle);
  servo.write(randomAngle);
  delay(random(100, 500));  // Random delay between 100ms and 500ms
}

// Function to randomly test gripper servo (open and close) with dedicated delay
void randomGripper(Servo &servo) {
  int randomGripperAction = random(0, 2); // 0 for open, 1 for close
  if (randomGripperAction == 0) {
    servo.write(GRIPPER_OPEN_ANGLE); // Open gripper
  } else {
    servo.write(GRIPPER_CLOSE_ANGLE); // Close gripper
  }
  delay(GRIPPER_DELAY_MS);  // Use dedicated delay for smoother gripper movement
}

// Function to perform combined random movements
void combinedRandomMovement() {
  int basePos = random(BASE_MIN_ANGLE, BASE_MAX_ANGLE);
  int shoulderPos = random(SHOULDER_MIN_ANGLE, SHOULDER_MAX_ANGLE);
  int elbowPos = random(ELBOW_MIN_ANGLE, ELBOW_MAX_ANGLE);

  baseServo.write(basePos);
  shoulderServo.write(shoulderPos);
  elbowServo.write(elbowPos);

  delay(random(200, 700));  // Random delay for combined movements
}

// Function to perform complex movement sequences
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

// Function to return all servos to home position
void backToHome() {
  Serial.println("Returning to Home Position...");
  baseServo.write(BASE_HOME_ANGLE);
  shoulderServo.write(SHOULDER_HOME_ANGLE);
  elbowServo.write(ELBOW_HOME_ANGLE);
  gripperServo.write(GRIPPER_HOME_ANGLE);
  delay(500); // Delay to allow servos to reach home position
}
