#include <WiFi.h>
#include <WebServer.h>
#include "ui_index.h"  // Include the UI file

// WiFi credentials
const char* ssid = "SSID";
const char* password = "PASSWORD";

WebServer server(80);

void handleRoot() {
    server.send(200, "text/html", html);
}

void handleCommand() {
    String cmd = server.arg("cmd");
    
    // Send the command through Serial
    Serial.println(cmd);
    
    server.send(200, "text/plain", "Command sent: " + cmd);
}

void setup() {
    Serial.begin(115200);
    
    // Connect to WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    
    Serial.println("Connected to WiFi");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    
    // Setup server routes
    server.on("/", handleRoot);
    server.on("/command", handleCommand);
    
    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
    server.handleClient();
}
