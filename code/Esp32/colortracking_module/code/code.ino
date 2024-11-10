#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "esp_camera.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "ui_index.h"

const char* ssid = "ssid"; // Replace with your SSID
const char* password = "password"; // Replace with your Password

String Feedback, Command, cmd, P1, P2, P3, P4, P5, P6, P7, P8, P9;
byte ReceiveState = 0, cmdState = 1, strState = 1, questionstate = 0, equalstate = 0, semicolonstate = 0;

// Coordinates for tracking
int x_coordinate = 0, y_coordinate = 0;
const int x_widthMid = 320, y_heightMid = 240;

// Camera pin configuration for AI-Thinker
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

WiFiServer server(80);

void ExecuteCommand() {
    if (cmd == "resetwifi") {
        WiFi.begin(P1.c_str(), P2.c_str());
        long int startTime = millis();
        while (WiFi.status() != WL_CONNECTED && (millis() - startTime < 5000)) {
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
        Serial.println("Received Coordinates:");
        Serial.printf("X Position: %d (Offset: %d)\n", x_coordinate, x_offset);
        Serial.printf("Y Position: %d (Offset: %d)\n", y_coordinate, y_offset);

        if (x_offset > 0) Serial.println("Object is to the right of center.");
        else if (x_offset < 0) Serial.println("Object is to the left of center.");
        else Serial.println("Object is centered horizontally.");

        if (y_offset > 0) Serial.println("Object is below center.");
        else if (y_offset < 0) Serial.println("Object is above center.");
        else Serial.println("Object is centered vertically.");
    }
    else if (cmd == "quality" || cmd == "contrast" || cmd == "brightness") {
        sensor_t* s = esp_camera_sensor_get();
        int val = P1.toInt();
        if (cmd == "quality") s->set_quality(s, val);
        else if (cmd == "contrast") s->set_contrast(s, val);
        else if (cmd == "brightness") s->set_brightness(s, val);
    }
    else {
        Feedback = "Command is not defined.";
    }
}

void setup() {
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
    Serial.begin(115200);
    Serial.setDebugOutput(true);

    // Camera initialization
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
    config.frame_size = psramFound() ? FRAMESIZE_UXGA : FRAMESIZE_SVGA;
    config.jpeg_quality = psramFound() ? 10 : 12;
    config.fb_count = psramFound() ? 2 : 1;

    if (esp_camera_init(&config) != ESP_OK) {
        Serial.println("Camera initialization failed!");
        ESP.restart();
    }

    // Flash LED setup
    pinMode(4, OUTPUT);
    digitalWrite(4, HIGH); //HIGH : LOw = on off

    // WiFi configuration
    WiFi.mode(WIFI_AP_STA);
    WiFi.begin(ssid, password);
    long int startTime = millis();
    while (WiFi.status() != WL_CONNECTED && (millis() - startTime < 10000)) {
        delay(100);
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.printf("ESP IP Address: http://%s\n", WiFi.localIP().toString().c_str());
    }
    server.begin();
}

void loop() {
    Feedback = ""; Command = ""; cmd = ""; P1 = ""; P2 = ""; P3 = ""; P4 = ""; P5 = ""; P6 = ""; P7 = ""; P8 = ""; P9 = "";
    ReceiveState = cmdState = strState = questionstate = equalstate = semicolonstate = 0;

    WiFiClient client = server.available();
    if (client) {
        String currentLine = "";
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                getCommand(c);
                if (c == '\n' && currentLine.length() == 0) {
                    handleClientRequest(client);
                    break;
                } else if (c != '\r') {
                    currentLine += c;
                }
                if (currentLine.indexOf("/?") != -1 && currentLine.indexOf(" HTTP") != -1) {
                    ExecuteCommand();
                }
            }
        }
        delay(1);
        client.stop();
    }
}

void handleClientRequest(WiFiClient& client) {
    if (cmd == "colorDetect") {
        camera_fb_t* fb = esp_camera_fb_get();
        if (!fb) {
            Serial.println("Camera capture failed");
            ESP.restart();
        }
        sendImage(client, fb);
        esp_camera_fb_return(fb);
    }
    else {
        sendResponse(client, cmd.isEmpty() ? INDEX_HTML : Feedback);
    }
} 

void sendImage(WiFiClient& client, camera_fb_t* fb) {
    client.println("HTTP/1.1 200 OK");
    client.println("Access-Control-Allow-Origin: *");
    client.println("Content-Type: image/jpeg");
    client.println("Content-Disposition: form-data; name=\"imageFile\"; filename=\"picture.jpg\"");
    client.println("Content-Length: " + String(fb->len));
    client.println("Connection: close");
    client.println();

    uint8_t* fbBuf = fb->buf;
    size_t fbLen = fb->len;
    for (size_t n = 0; n < fbLen; n += 1024) {
        size_t chunkSize = (n + 1024 < fbLen) ? 1024 : fbLen % 1024;
        client.write(fbBuf, chunkSize);
        fbBuf += chunkSize;
    }
}

void sendResponse(WiFiClient& client, const String& data) {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html; charset=utf-8");
    client.println("Access-Control-Allow-Origin: *");
    client.println("Connection: close");
    client.println();

    for (size_t i = 0; i < data.length(); i += 1000) {
        client.print(data.substring(i, i + 1000));
    }
}

void getCommand(char c) {
    if (c == '?') ReceiveState = 1;
    if (ReceiveState == 1) {
        Command += c;
        if (c == '=') cmdState = 0;
        if (c == ';') strState++;
        if (cmdState == 1 && c != '?') cmd += c;
        if (cmdState == 0 && strState == 1 && c != '=') P1 += c;
        if (cmdState == 0 && strState == 2) P2 += c;
        if (cmdState == 0 && strState == 3) P3 += c;
        if (cmdState == 0 && strState == 4) P4 += c;
        if (cmdState == 0 && strState == 5) P5 += c;
        if (cmdState == 0 && strState == 6) P6 += c;
        if (cmdState == 0 && strState == 7) P7 += c;
        if (cmdState == 0 && strState == 8) P8 += c;
        if (cmdState == 0 && strState == 9) P9 += c;
    }
}
