#include "ui_index.h"

// Set to true for ESP32, false for ESP8266 | led to true to enable ip led
const bool useESP32 = false;
bool ledIndicatorEnabled = false;

#if defined(ESP32)
    #include <WiFi.h>
    #include <WebServer.h>
    WebServer server(80);
    const int ledPin = 2;
#elif defined(ESP8266)
    #include <ESP8266WiFi.h>
    #include <ESP8266WebServer.h>
    ESP8266WebServer server(80);
    const int ledPin = LED_BUILTIN;
#endif

// WiFi credentials
const char* ssid = "SSID";
const char* password = "password";

const int blinkDelay = 200;
const int segmentPause = 1000;
const int repeatPause = 2000;

void handleRoot() {
    server.send(200, "text/html", htmlContent);
}

void handleCommand() {
    String cmd = server.arg("cmd");
    Serial.println(cmd);
    server.send(200, "text/plain", "Command sent: " + cmd);
}

void blinkLED(int times) {
    for (int i = 0; i < times; i++) {
        digitalWrite(ledPin, LOW);
        delay(blinkDelay);
        digitalWrite(ledPin, HIGH);
        delay(blinkDelay);
    }
    delay(segmentPause);
}

void blinkLastTwoIPSegmentsTwice() {
    if (!ledIndicatorEnabled) return;
    
    IPAddress ip = WiFi.localIP();
    for (int j = 0; j < 2; j++) {
        blinkLED(ip[2]);
        blinkLED(ip[3]);
        delay(repeatPause);
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
    }

    Serial.println("IP : " + WiFi.localIP().toString());
    blinkLastTwoIPSegmentsTwice();

    server.on("/", handleRoot);
    server.on("/command", handleCommand);
    server.begin();
}

void loop() {
    server.handleClient();
}