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
const int DELAY_MS = 500;  // Delay for movement transitions

void setup() {
  // Initialize servos
  baseServo.attach(BASE_SERVO_PIN);
  shoulderServo.attach(SHOULDER_SERVO_PIN);
  elbowServo.attach(ELBOW_SERVO_PIN);
  gripperServo.attach(GRIPPER_SERVO_PIN);

  Serial.begin(9600);
  Serial.println("Starting Arm Evaluation Test...");
}

void loop() {
  // Test base servo movement
  Serial.println("Testing Base Servo...");
  moveServo(baseServo, BASE_MIN_ANGLE, BASE_MAX_ANGLE);

  // Test shoulder servo movement
  Serial.println("Testing Shoulder Servo...");
  moveServo(shoulderServo, SHOULDER_MIN_ANGLE, SHOULDER_MAX_ANGLE);

  // Test elbow servo movement
  Serial.println("Testing Elbow Servo...");
  moveServo(elbowServo, ELBOW_MIN_ANGLE, ELBOW_MAX_ANGLE);

  // Test gripper servo movement
  Serial.println("Testing Gripper Servo...");
  testGripper(gripperServo);

  // Repeat the test with a delay
  delay(3000);  // Wait for 3 seconds before the next loop
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

// Function to test gripper servo (open and close)
void testGripper(Servo &servo) {
  // Open gripper
  servo.write(GRIPPER_OPEN_ANGLE);
  delay(DELAY_MS);
  // Close gripper
  servo.write(GRIPPER_CLOSE_ANGLE);
  delay(DELAY_MS);
}
