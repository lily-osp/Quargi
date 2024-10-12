#include <Wire.h>
#include <Adafruit_TCS34725.h>

// Include libraries for sensors
#define TRIG_PIN 12  // Pin for the ultrasonic sensor trigger
#define ECHO_PIN 13  // Pin for the ultrasonic sensor echo

// Initialize color sensor
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

// Variables for color sensor readings
uint16_t r, g, b, c;

// Function to initialize the ultrasonic sensor
void setup() {
  Serial.begin(9600);
  Serial.println("Starting Sensor Evaluation Test...");

  // Initialize ultrasonic sensor pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Initialize color sensor
  if (tcs.begin()) {
    Serial.println("Color sensor (TCS34725) initialized successfully.");
  } else {
    Serial.println("Color sensor (TCS34725) not found. Check wiring!");
    while (1);
  }
}

void loop() {
  // Test Ultrasonic Sensor
  Serial.println("Testing Ultrasonic Sensor...");
  float distance = readUltrasonic();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Test Color Sensor
  Serial.println("Testing Color Sensor...");
  readColorSensor();

  delay(2000);  // Wait for 2 seconds before the next test cycle
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

// Function to read and print color sensor data
void readColorSensor() {
  tcs.getRawData(&r, &g, &b, &c);
  Serial.print("Red: "); Serial.print(r); Serial.print(" ");
  Serial.print("Green: "); Serial.print(g); Serial.print(" ");
  Serial.print("Blue: "); Serial.print(b); Serial.print(" ");
  Serial.print("Clear: "); Serial.println(c);

  // Detect target color (Red)
  if (r > g && r > b && r > 1000) {  // Arbitrary threshold for red detection
    Serial.println("Red color detected.");
  } else {
    Serial.println("No red color detected.");
  }
}
