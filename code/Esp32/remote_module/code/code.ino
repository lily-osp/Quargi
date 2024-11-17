#include "ui_index.h"

// Set to true for ESP32, false for ESP8266 | led to true to enable IP LED
const bool useESP32 = false;
bool ledIndicatorEnabled = false;

#if defined(ESP32)
    #include <WiFi.h>
    #include <WebServer.h>
    #include <ESPmDNS.h>
    WebServer server(80);
    const int ledPin = 2;
#elif defined(ESP8266)
    #include <ESP8266WiFi.h>
    #include <ESP8266WebServer.h>
    #include <ESP8266mDNS.h>
    ESP8266WebServer server(80);
    const int ledPin = LED_BUILTIN;
#endif

// WiFi credentials
const char* ssid = "SSID";
const char* password = "password";

// Device hostname for mDNS
const char* hostname = "quargi-remote";

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

    // Connect to WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    Serial.println("IP Address: " + WiFi.localIP().toString());

    // Blink the last two IP segments if enabled
    blinkLastTwoIPSegmentsTwice();

    // Set up mDNS
    if (MDNS.begin(hostname)) {
        Serial.println("mDNS responder started");
        Serial.println("Access the device at: http://" + String(hostname) + ".local");
    } else {
        Serial.println("Error setting up mDNS responder!");
    }

    // Set up server routes
    server.on("/", handleRoot);
    server.on("/command", handleCommand);
    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
    server.handleClient();
}
