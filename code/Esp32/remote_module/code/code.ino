#include "ui_index.h"

// Set this to true for ESP32, or false for ESP8266
const bool useESP32 = false;

#if defined(ESP32)
    #include <WiFi.h>
    #include <WebServer.h>
    WebServer server(80);
    const int ledPin = 2; // Built-in LED on ESP32
#elif defined(ESP8266)
    #include <ESP8266WiFi.h>
    #include <ESP8266WebServer.h>
    ESP8266WebServer server(80);
    const int ledPin = LED_BUILTIN; // Built-in LED on ESP8266
#endif

// WiFi credentials
const char* ssid = "SSID";
const char* password = "password";

// Blink delay time in milliseconds
const int blinkDelay = 200;
const int segmentPause = 1000; // Pause between segments
const int repeatPause = 2500;  // Pause before repeating sequence

void handleRoot() {
    server.send(200, "text/html", htmlContent);
}

void handleCommand() {
    String cmd = server.arg("cmd");

    // Send the command through Serial
    Serial.println(cmd);

    server.send(200, "text/plain", "Command sent: " + cmd);
}

// Function to blink the LED a specified number of times
void blinkLED(int times) {
    for (int i = 0; i < times; i++) {
        digitalWrite(ledPin, LOW);  // Turn LED on
        delay(blinkDelay);
        digitalWrite(ledPin, HIGH); // Turn LED off
        delay(blinkDelay);
    }
    delay(segmentPause); // Pause between IP segments
}

// Function to blink the last two segments of the IP address
void blinkLastTwoIPSegmentsTwice() {
    IPAddress ip = WiFi.localIP();
    int thirdSegment = ip[2];
    int fourthSegment = ip[3];

    for (int j = 0; j < 2; j++) { // Repeat the blink sequence twice
        blinkLED(thirdSegment);    // Blink for the third segment
        blinkLED(fourthSegment);   // Blink for the fourth segment
        delay(repeatPause);        // Pause between repeat sequences
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH); // Turn LED off initially

    // Connect to WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
    }

    Serial.print("IP : ");
    Serial.println(WiFi.localIP());

    // Blink the last two IP address segments twice to indicate connection
    blinkLastTwoIPSegmentsTwice();

    // Setup server routes
    server.on("/", handleRoot);
    server.on("/command", handleCommand);

    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
    server.handleClient();
}
