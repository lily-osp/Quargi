#include "ui_index.h" 

// Set this to true for ESP32, or false for ESP8266
const bool useESP32 = false;

#if defined(ESP32)
    #include <WiFi.h>
    #include <WebServer.h>
    WebServer server(80);
#elif defined(ESP8266)
    #include <ESP8266WiFi.h>
    #include <ESP8266WebServer.h>
    ESP8266WebServer server(80);
#endif

// WiFi credentials
const char* ssid = "SSID";
const char* password = "password";

void handleRoot() {
    server.send(200, "text/html", htmlContent);
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
    }
    
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
