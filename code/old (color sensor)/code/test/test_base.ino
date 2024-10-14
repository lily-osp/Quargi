// Define motor control pins
const int MOTOR1_IN1_PIN = 3;  // Motor 1 control pin 1
const int MOTOR1_IN2_PIN = 4;  // Motor 1 control pin 2
const int MOTOR2_IN3_PIN = 5;  // Motor 2 control pin 3
const int MOTOR2_IN4_PIN = 6;  // Motor 2 control pin 4
const int MOTOR1_EN_PIN = 9;   // Motor 1 enable pin (speed control)
const int MOTOR2_EN_PIN = 10;  // Motor 2 enable pin (speed control)

// Define ultrasonic sensor pins
#define TRIG_PIN 12  // Pin for the ultrasonic sensor trigger
#define ECHO_PIN 2  // Pin for the ultrasonic sensor echo

// Define movement parameters
const int MOTOR_SPEED = 200;  // Speed of motors (0 to 255)
const int MOVE_DURATION = 2000;  // Duration for each movement (milliseconds)
const int OBSTACLE_DISTANCE = 15; // Distance threshold to detect obstacles (in cm)
const int TURN_DELAY = 1000; // Delay for turning (in milliseconds)

void setup() {
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
  Serial.println("Starting Base and Ultrasonic Sensor Combined Test...");
}

void loop() {
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
    delay(2000);  // Wait for 2 seconds before the next loop
  } else {
    // If obstacle detected, stop and turn
    Serial.println("Obstacle detected. Stopping and turning...");
    stopMotors();
    delay(TURN_DELAY);  // Wait before turning
    turnRight();       // Turn to avoid obstacle
    delay(TURN_DELAY);
  }
}

// Function to read distance from the ultrasonic sensor
float readUltrasonic() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = (duration * 0.034) / 2;  // Convert to centimeters
  return distance;
}

// Function to move forward
void moveForward() {
  analogWrite(MOTOR1_EN_PIN, MOTOR_SPEED);  // Set motor speed
  analogWrite(MOTOR2_EN_PIN, MOTOR_SPEED);
  digitalWrite(MOTOR1_IN1_PIN, HIGH);
  digitalWrite(MOTOR1_IN2_PIN, LOW);
  digitalWrite(MOTOR2_IN3_PIN, HIGH);
  digitalWrite(MOTOR2_IN4_PIN, LOW);
}

// Function to move backward
void moveBackward() {
  analogWrite(MOTOR1_EN_PIN, MOTOR_SPEED);  // Set motor speed
  analogWrite(MOTOR2_EN_PIN, MOTOR_SPEED);
  digitalWrite(MOTOR1_IN1_PIN, LOW);
  digitalWrite(MOTOR1_IN2_PIN, HIGH);
  digitalWrite(MOTOR2_IN3_PIN, LOW);
  digitalWrite(MOTOR2_IN4_PIN, HIGH);
}

// Function to turn left
void turnLeft() {
  analogWrite(MOTOR1_EN_PIN, MOTOR_SPEED);  // Set motor speed
  analogWrite(MOTOR2_EN_PIN, MOTOR_SPEED);
  digitalWrite(MOTOR1_IN1_PIN, LOW);
  digitalWrite(MOTOR1_IN2_PIN, HIGH);
  digitalWrite(MOTOR2_IN3_PIN, HIGH);
  digitalWrite(MOTOR2_IN4_PIN, LOW);
}

// Function to turn right
void turnRight() {
  analogWrite(MOTOR1_EN_PIN, MOTOR_SPEED);  // Set motor speed
  analogWrite(MOTOR2_EN_PIN, MOTOR_SPEED);
  digitalWrite(MOTOR1_IN1_PIN, HIGH);
  digitalWrite(MOTOR1_IN2_PIN, LOW);
  digitalWrite(MOTOR2_IN3_PIN, LOW);
  digitalWrite(MOTOR2_IN4_PIN, HIGH);
}

// Function to stop motors
void stopMotors() {
  analogWrite(MOTOR1_EN_PIN, 0);  // Stop motors
  analogWrite(MOTOR2_EN_PIN, 0);
  digitalWrite(MOTOR1_IN1_PIN, LOW);
  digitalWrite(MOTOR1_IN2_PIN, LOW);
  digitalWrite(MOTOR2_IN3_PIN, LOW);
  digitalWrite(MOTOR2_IN4_PIN, LOW);
}
