#include <PID_v1.h>
#include <Arduino.h>

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
const int TURN_DURATION = 500;    // ms

// PID variables
double Setpoint, Input, Output;
double Kp = 2, Ki = 5, Kd = 1;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

// Exploration variables
unsigned long lastMoveChangeTime = 0;
const unsigned long MOVE_CHANGE_INTERVAL = 3000;  // 3 seconds
bool exploring = true;

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
void avoidObstacle();
void explore();

void setup() {
  // Initialize pins and serial communication
  pinMode(MOTOR1_IN1_PIN, OUTPUT);
  pinMode(MOTOR1_IN2_PIN, OUTPUT);
  pinMode(MOTOR2_IN3_PIN, OUTPUT);
  pinMode(MOTOR2_IN4_PIN, OUTPUT);
  pinMode(MOTOR1_EN_PIN, OUTPUT);
  pinMode(MOTOR2_EN_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(ECHO_PIN), echoInterrupt, CHANGE);
  Serial.begin(9600);
  Serial.println("Starting Exploration Mode with PID Control...");

  // Initialize PID
  Setpoint = OBSTACLE_DISTANCE;
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(0, MAX_SPEED - BASE_SPEED);
}

void loop() {
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

    Input = distance;
    myPID.Compute();

    if (distance > OBSTACLE_DISTANCE) {
      explore();  // Explore the environment when no obstacles are detected
    } else {
      avoidObstacle();  // Avoid obstacles
    }

    newDistanceAvailable = false;
  }
}

void echoInterrupt() {
  if (digitalRead(ECHO_PIN) == HIGH) {
    // Pulse started
    pulseStartTime = micros();
  } else {
    // Pulse ended
    if (pulseStartTime != 0) {
      pulseDuration = micros() - pulseStartTime;
      pulseStartTime = 0;
      newDistanceAvailable = true;
    }
  }
}

float getDistance() {
  noInterrupts();  // Temporarily disable interrupts to safely read the pulseDuration
  unsigned long duration = pulseDuration;
  interrupts();    // Re-enable interrupts after reading the value
  return (duration * 0.034) / 2;  // Calculate distance in cm
}

void moveForward(int leftSpeed, int rightSpeed) {
  analogWrite(MOTOR1_EN_PIN, leftSpeed);
  analogWrite(MOTOR2_EN_PIN, rightSpeed);
  digitalWrite(MOTOR1_IN1_PIN, HIGH);
  digitalWrite(MOTOR1_IN2_PIN, LOW);
  digitalWrite(MOTOR2_IN3_PIN, HIGH);
  digitalWrite(MOTOR2_IN4_PIN, LOW);
}

void turnRight() {
  analogWrite(MOTOR1_EN_PIN, MAX_SPEED);
  analogWrite(MOTOR2_EN_PIN, MAX_SPEED);
  digitalWrite(MOTOR1_IN1_PIN, HIGH);
  digitalWrite(MOTOR1_IN2_PIN, LOW);
  digitalWrite(MOTOR2_IN3_PIN, LOW);
  digitalWrite(MOTOR2_IN4_PIN, HIGH);
}

void turnLeft() {
  analogWrite(MOTOR1_EN_PIN, MAX_SPEED);
  analogWrite(MOTOR2_EN_PIN, MAX_SPEED);
  digitalWrite(MOTOR1_IN1_PIN, LOW);
  digitalWrite(MOTOR1_IN2_PIN, HIGH);
  digitalWrite(MOTOR2_IN3_PIN, HIGH);
  digitalWrite(MOTOR2_IN4_PIN, LOW);
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
  stopMotors();
  delay(100);
  turnRight();
  delay(TURN_DURATION);
  
  float newDistance = getDistance();
  if (newDistance <= OBSTACLE_DISTANCE) {
    turnRight();
    delay(TURN_DURATION);
  }
}

void explore() {
  // If enough time has passed, change the movement behavior randomly
  if (millis() - lastMoveChangeTime > MOVE_CHANGE_INTERVAL) {
    int randomMove = random(0, 3);  // Generate a random number between 0 and 2

    if (randomMove == 0) {
      // Move forward
      moveForward(BASE_SPEED + Output, BASE_SPEED + Output);
      Serial.println("Moving forward...");
    } else if (randomMove == 1) {
      // Turn left
      turnLeft();
      delay(TURN_DURATION);
      Serial.println("Turning left...");
    } else if (randomMove == 2) {
      // Turn right
      turnRight();
      delay(TURN_DURATION);
      Serial.println("Turning right...");
    }

    // Update the time for the next random movement change
    lastMoveChangeTime = millis();
  } else {
    // Continue moving forward if the interval hasn't passed
    moveForward(BASE_SPEED + Output, BASE_SPEED + Output);
  }
}
