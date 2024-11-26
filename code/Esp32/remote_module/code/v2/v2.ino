#include <EEPROM.h>
#include "setup_ui.h"      // Includes SETUP_UI for configuration UI
#include "main_ui.h"   // Includes the command/remote UI

// Platform-Specific Includes
#if defined(ESP32)
    #include <WiFi.h>
    #include <WebServer.h>
    #include <ESPmDNS.h>
    #include <DNSServer.h>
    WebServer server(80);
#elif defined(ESP8266)
    #include <ESP8266WiFi.h>
    #include <ESP8266WebServer.h>
    #include <ESP8266mDNS.h>
    #include <DNSServer.h>
    ESP8266WebServer server(80);
#endif

DNSServer dnsServer;

// Constants
constexpr bool useESP32 = false; // Set to true for ESP32, false for ESP8266
constexpr byte DNS_PORT = 53;
constexpr size_t EEPROM_SIZE = 512;
constexpr int WIFI_SSID_ADDR = 0;
constexpr int WIFI_PASS_ADDR = 64;
constexpr int MDNS_NAME_ADDR = 128;
constexpr size_t MAX_SSID_LENGTH = 32;
constexpr size_t MAX_PASS_LENGTH = 64;
constexpr size_t MAX_MDNS_LENGTH = 32;

// Variables
String wifiSSID = "";
String wifiPassword = "";
String mdnsName = "";

// Function Declarations
void saveSettingsToEEPROM();
void loadSettingsFromEEPROM();
void setupAccessPoint();
bool connectToWiFi();
void setupMDNS();
void handleRoot();
void handleCommandUI();
void handleSetup();
void setupHTTPRoutes();

// Helper Functions for String Handling
void writeStringToEEPROM(int addr, const String &data) {
    for (int i = 0; i < data.length(); ++i) {
        EEPROM.write(addr + i, data[i]);
    }
    EEPROM.write(addr + data.length(), '\0'); // Null-terminate the string
}

String readStringFromEEPROM(int addr, int maxLength) {
    String result = "";
    for (int i = 0; i < maxLength; ++i) {
        char c = EEPROM.read(addr + i);
        if (c == '\0') break; // Stop at the null terminator
        result += c;
    }
    return result;
}

// Functions: EEPROM Handling
void saveSettingsToEEPROM() {
    writeStringToEEPROM(WIFI_SSID_ADDR, wifiSSID);
    writeStringToEEPROM(WIFI_PASS_ADDR, wifiPassword);
    writeStringToEEPROM(MDNS_NAME_ADDR, mdnsName);
    EEPROM.commit();
    Serial.println("Settings saved to EEPROM.");
}

void loadSettingsFromEEPROM() {
    wifiSSID = readStringFromEEPROM(WIFI_SSID_ADDR, MAX_SSID_LENGTH);
    wifiPassword = readStringFromEEPROM(WIFI_PASS_ADDR, MAX_PASS_LENGTH);
    mdnsName = readStringFromEEPROM(MDNS_NAME_ADDR, MAX_MDNS_LENGTH);

    Serial.println("Settings loaded from EEPROM:");
    Serial.println("SSID: " + wifiSSID);
    Serial.println("Password: " + wifiPassword);
    Serial.println("mDNS: " + mdnsName);
}

// Functions: HTTP Handlers
void handleRoot() {
    server.send(200, "text/html", SETUP_UI); // Configuration UI
}

void handleCommandUI() {
    server.send(200, "text/html", htmlContent); // Command/Remote UI
}

void handleSetup() {
    if (server.hasArg("ssid") && server.hasArg("password") && server.hasArg("mdns")) {
        wifiSSID = server.arg("ssid");
        wifiPassword = server.arg("password");
        mdnsName = server.arg("mdns");

        Serial.println("Received settings:");
        Serial.printf("SSID: %s\nPassword: %s\nmDNS: %s\n", wifiSSID.c_str(), wifiPassword.c_str(), mdnsName.c_str());

        saveSettingsToEEPROM();
        server.send(200, "text/plain", "Settings saved! Rebooting...");
        delay(2000);
        ESP.restart();
    } else {
        server.send(400, "text/plain", "Invalid input!");
    }
}

// Functions: Access Point and Network
void setupAccessPoint() {
    const char* apSSID = "ConfigAP";
    const char* apPassword = "12345678";

    WiFi.softAP(apSSID, apPassword);
    IPAddress apIP = WiFi.softAPIP();
    dnsServer.start(DNS_PORT, "*", apIP); // Captive portal redirects all domains to AP IP

    Serial.printf("AP Mode. Connect to: %s\nIP Address: %s\n", apSSID, apIP.toString().c_str());

    setupHTTPRoutes();
    server.begin();
    Serial.println("HTTP server started in AP mode.");
}

bool connectToWiFi() {
    WiFi.begin(wifiSSID.c_str(), wifiPassword.c_str());
    Serial.print("Connecting to WiFi");
    for (int attempts = 0; attempts < 10; attempts++) {
        if (WiFi.status() == WL_CONNECTED) {
            Serial.printf("\nConnected! IP Address: %s\n", WiFi.localIP().toString().c_str());
            return true;
        }
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nFailed to connect to WiFi.");
    return false;
}

void setupMDNS() {
    if (MDNS.begin(mdnsName.c_str())) {
        Serial.printf("mDNS responder started. Access the device at: http://%s.local\n", mdnsName.c_str());
    } else {
        Serial.println("Error setting up mDNS responder!");
    }
}

// Functions: HTTP Route Setup
void setupHTTPRoutes() {
    server.on("/", handleRoot);
    server.on("/setup", HTTP_POST, handleSetup);
    server.on("/command", [](void) {
        String cmd = server.arg("cmd");
        Serial.printf("Command received: %s\n", cmd.c_str());
        server.send(200, "text/plain", "Command sent: " + cmd);
    });
}

// Main Functions
void setup() {
    Serial.begin(115200);

    // Initialize EEPROM
    EEPROM.begin(EEPROM_SIZE);

    // Load settings
    loadSettingsFromEEPROM();

    // Determine mode (AP or Station)
    if (wifiSSID.isEmpty() || wifiPassword.isEmpty() || mdnsName.isEmpty() || !connectToWiFi()) {
        setupAccessPoint(); // Fallback to AP mode for configuration
    } else {
        setupMDNS();
        setupHTTPRoutes();
        server.begin();
        Serial.println("HTTP server started in Station mode.");
    }
}

void loop() {
    dnsServer.processNextRequest(); // Handle captive portal
    server.handleClient();          // Handle HTTP server
}
