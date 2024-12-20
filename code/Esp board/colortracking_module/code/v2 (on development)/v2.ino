#include <EEPROM.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "esp_camera.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "main_ui.h"
#include "setup_ui.h"
#include <ESPmDNS.h>
#include <DNSServer.h>

// Constants for EEPROM
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
String Feedback = "";
String Command = "", cmd = "", P1 = "", P2 = "", P3 = "", P4 = "", P5 = "", P6 = "", P7 = "", P8 = "", P9 = "";
byte ReceiveState = 0, cmdState = 1, strState = 1, questionstate = 0, equalstate = 0, semicolonstate = 0;

// Coordinate variables
int x_coordinate = 0;
int y_coordinate = 0;
int x_widthMid = 320;
int y_heightMid = 240;

// Camera pins
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

DNSServer dnsServer;
WiFiServer server(80);
constexpr byte DNS_PORT = 53;

// EEPROM Helper Functions
void writeStringToEEPROM(int addr, const String &data) {
    for (int i = 0; i < data.length(); ++i) {
        EEPROM.write(addr + i, data[i]);
    }
    EEPROM.write(addr + data.length(), '\0');
    EEPROM.commit();
}

String readStringFromEEPROM(int addr, int maxLength) {
    String result = "";
    for (int i = 0; i < maxLength; ++i) {
        char c = EEPROM.read(addr + i);
        if (c == '\0') break;
        result += c;
    }
    return result;
}

void setupCamera() {
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;

    if (psramFound()) {
        config.frame_size = FRAMESIZE_UXGA;
        config.jpeg_quality = 10;
        config.fb_count = 2;
    } else {
        config.frame_size = FRAMESIZE_SVGA;
        config.jpeg_quality = 12;
        config.fb_count = 1;
    }

    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        delay(1000);
        ESP.restart();
    }

    sensor_t * s = esp_camera_sensor_get();
    s->set_framesize(s, FRAMESIZE_CIF);
}

void setupAccessPoint() {
    WiFi.softAP("ESP-CAM-Setup", "12345678");
    IPAddress apIP = WiFi.softAPIP();
    dnsServer.start(DNS_PORT, "*", apIP);
    Serial.println("AP Mode started. Connect to ESP-CAM-Setup");
}

bool connectToWiFi() {
    if (wifiSSID.length() == 0 || wifiPassword.length() == 0) return false;
    
    WiFi.begin(wifiSSID.c_str(), wifiPassword.c_str());
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        attempts++;
    }
    return WiFi.status() == WL_CONNECTED;
}

void handleRoot(WiFiClient &client) {
    if (WiFi.status() == WL_CONNECTED) {
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html; charset=utf-8");
        client.println("Connection: close");
        client.println();
        client.println(String((const char *)INDEX_HTML));
    } else {
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html; charset=utf-8");
        client.println("Connection: close");
        client.println();
        client.println(SETUP_UI);
    }
}

void handleSetup(WiFiClient &client, String request) {
    if (request.indexOf("ssid=") != -1 && request.indexOf("password=") != -1 && request.indexOf("mdns=") != -1) {
        // Parse parameters from request
        int ssidStart = request.indexOf("ssid=") + 5;
        int ssidEnd = request.indexOf("&", ssidStart);
        int passStart = request.indexOf("password=") + 9;
        int passEnd = request.indexOf("&", passStart);
        int mdnsStart = request.indexOf("mdns=") + 5;
        int mdnsEnd = request.indexOf(" ", mdnsStart);

        String newSSID = request.substring(ssidStart, ssidEnd);
        String newPass = request.substring(passStart, passEnd);
        String newMDNS = request.substring(mdnsStart, mdnsEnd);

        writeStringToEEPROM(WIFI_SSID_ADDR, newSSID);
        writeStringToEEPROM(WIFI_PASS_ADDR, newPass);
        writeStringToEEPROM(MDNS_NAME_ADDR, newMDNS);

        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/plain");
        client.println("Connection: close");
        client.println();
        client.println("Settings saved! Rebooting...");
        
        delay(1000);
        ESP.restart();
    }
}

void ExecuteCommand() {
    if (cmd == "resetwifi") {
        WiFi.begin(P1.c_str(), P2.c_str());
        long int StartTime = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - StartTime < 5000) {
            delay(100);
        }
        Feedback = "STAIP: " + WiFi.localIP().toString();
    }
    else if (cmd == "restart") {
        ESP.restart();
    }
    else if (cmd == "cm") {
        x_coordinate = P1.toInt();
        y_coordinate = P2.toInt();
        int x_offset = x_coordinate - x_widthMid;
        int y_offset = y_coordinate - y_heightMid;
        Feedback = "Position Data - X: " + String(x_coordinate) + ", Y: " + String(y_coordinate);
    }
    else if (cmd == "quality") {
        sensor_t * s = esp_camera_sensor_get();
        s->set_quality(s, P1.toInt());
    }
    else if (cmd == "contrast") {
        sensor_t * s = esp_camera_sensor_get();
        s->set_contrast(s, P1.toInt());
    }
    else if (cmd == "brightness") {
        sensor_t * s = esp_camera_sensor_get();
        s->set_brightness(s, P1.toInt());
    }
    
    if (Feedback == "") {
        Feedback = Command;
    }
}

void getCommand(char c) {
    if (c == '?') ReceiveState = 1;
    if ((c == ' ') || (c == '\r') || (c == '\n')) ReceiveState = 0;

    if (ReceiveState == 1) {
        Command = Command + String(c);
        if (c == '=') cmdState = 0;
        if (c == ';') strState++;
        if ((cmdState == 1) && ((c != '?') || (questionstate == 1))) cmd = cmd + String(c);
        if ((cmdState == 0) && (strState == 1) && ((c != '=') || (equalstate == 1))) P1 = P1 + String(c);
        if ((cmdState == 0) && (strState == 2) && (c != ';')) P2 = P2 + String(c);
        if ((cmdState == 0) && (strState == 3) && (c != ';')) P3 = P3 + String(c);
        if ((cmdState == 0) && (strState == 4) && (c != ';')) P4 = P4 + String(c);
        if ((cmdState == 0) && (strState == 5) && (c != ';')) P5 = P5 + String(c);
        if ((cmdState == 0) && (strState == 6) && (c != ';')) P6 = P6 + String(c);
        if ((cmdState == 0) && (strState == 7) && (c != ';')) P7 = P7 + String(c);
        if ((cmdState == 0) && (strState == 8) && (c != ';')) P8 = P8 + String(c);
        if ((cmdState == 0) && (strState >= 9) && ((c != ';') || (semicolonstate == 1))) P9 = P9 + String(c);
        if (c == '?') questionstate = 1;
        if (c == '=') equalstate = 1;
        if ((strState >= 9) && (c == ';')) semicolonstate = 1;
    }
}

void setup() {
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
    Serial.begin(115200);
    EEPROM.begin(EEPROM_SIZE);

    wifiSSID = readStringFromEEPROM(WIFI_SSID_ADDR, MAX_SSID_LENGTH);
    wifiPassword = readStringFromEEPROM(WIFI_PASS_ADDR, MAX_PASS_LENGTH);
    mdnsName = readStringFromEEPROM(MDNS_NAME_ADDR, MAX_MDNS_LENGTH);
    if (mdnsName.length() == 0) mdnsName = "quargi-camera";

    setupCamera();

    WiFi.mode(WIFI_AP_STA);
    if (!connectToWiFi()) {
        setupAccessPoint();
    } else {
        if (!MDNS.begin(mdnsName.c_str())) {
            Serial.println("Error starting mDNS");
        }
    }

    server.begin();
}

void loop() {
    dnsServer.processNextRequest();
    
    WiFiClient client = server.available();
    if (client) {
        String currentLine = "";
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                getCommand(c);

                if (c == '\n') {
                    if (currentLine.length() == 0) {
                        if (cmd == "colorDetect") {
                            camera_fb_t * fb = esp_camera_fb_get();
                            if (!fb) {
                                Serial.println("Camera capture failed");
                                ESP.restart();
                            }

                            client.println("HTTP/1.1 200 OK");
                            client.println("Access-Control-Allow-Origin: *");
                            client.println("Content-Type: image/jpeg");
                            client.println("Content-Length: " + String(fb->len));
                            client.println("Connection: close");
                            client.println();

                            client.write(fb->buf, fb->len);
                            esp_camera_fb_return(fb);
                        }
                        else if (currentLine.indexOf("/setup") != -1) {
                            handleSetup(client, currentLine);
                        }
                        else {
                            handleRoot(client);
                        }
                        break;
                    } else {
                        currentLine = "";
                    }
                } else if (c != '\r') {
                    currentLine += c;
                }

                if ((currentLine.indexOf("/?") != -1) && (currentLine.indexOf(" HTTP") != -1)) {
                    if (Command.indexOf("stop") != -1) {
                        client.println();
                        client.println();
                        client.stop();
                    }
                    currentLine = "";
                    ExecuteCommand();
                }
            }
        }
        client.stop();
    }
}