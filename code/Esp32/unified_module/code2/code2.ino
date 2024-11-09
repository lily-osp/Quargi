#include <WiFi.h>
#include <WebServer.h>
#include "esp_camera.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "ui_index.h"

// WiFi credentials
const char* ssid = "SSID";
const char* password = "password";

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

WebServer server(80);
bool isCameraInitialized = false;

bool initCamera() {
    if (isCameraInitialized) return true;

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
        return false;
    }

    // Flash LED setup
    pinMode(4, OUTPUT);
    digitalWrite(4, HIGH);

    isCameraInitialized = true;
    Serial.println("Camera initialized successfully");
    return true;
}

void handleRoot() {
    server.send(200, "text/html", html);
}

void handleCommand() {
    String cmd = server.arg("cmd");
    String P1 = server.arg("P1");
    String P2 = server.arg("P2");

    if (cmd == "resetwifi") {
        WiFi.begin(P1.c_str(), P2.c_str());
        long int startTime = millis();
        while (WiFi.status() != WL_CONNECTED && (millis() - startTime < 5000)) {
            delay(100);
        }
        server.send(200, "text/plain", "STAIP: " + WiFi.localIP().toString());
    }
    else if (cmd == "restart") {
        server.send(200, "text/plain", "Restarting...");
        delay(1000);
        ESP.restart();
    }
    else if (cmd == "startstream") {
        if (!isCameraInitialized) {
            if (initCamera()) {
                server.send(200, "text/plain", "Camera initialized and stream started");
            } else {
                server.send(500, "text/plain", "Failed to initialize camera");
            }
        } else {
            server.send(200, "text/plain", "Stream already active");
        }
    }
    else if (cmd == "stopstream") {
        if (isCameraInitialized) {
            esp_camera_deinit();
            isCameraInitialized = false;
            server.send(200, "text/plain", "Camera stopped");
        } else {
            server.send(200, "text/plain", "Camera already stopped");
        }
    }
    else if (cmd == "cm") {
        if (!isCameraInitialized) {
            server.send(400, "text/plain", "Camera not initialized");
            return;
        }
        x_coordinate = P1.toInt();
        y_coordinate = P2.toInt();
        int x_offset = x_coordinate - x_widthMid;
        int y_offset = y_coordinate - y_heightMid;

        String response = "Position Data - X: " + String(x_coordinate) + ", Y: " + String(y_coordinate);
        Serial.println("Received Coordinates:");
        Serial.printf("X Position: %d (Offset: %d)\n", x_coordinate, x_offset);
        Serial.printf("Y Position: %d (Offset: %d)\n", y_coordinate, y_offset);

        server.send(200, "text/plain", response);
    }
    else if (cmd == "quality" || cmd == "contrast" || cmd == "brightness") {
        if (!isCameraInitialized) {
            server.send(400, "text/plain", "Camera not initialized");
            return;
        }
        sensor_t* s = esp_camera_sensor_get();
        int val = P1.toInt();
        if (cmd == "quality") s->set_quality(s, val);
        else if (cmd == "contrast") s->set_contrast(s, val);
        else if (cmd == "brightness") s->set_brightness(s, val);
        server.send(200, "text/plain", "Camera setting updated");
    }
    else {
        server.send(200, "text/plain", "Command sent: " + cmd);
    }
}

void handleCapture() {
    if (!isCameraInitialized) {
        server.send(400, "text/plain", "Camera not initialized. Please start stream first.");
        return;
    }

    camera_fb_t* fb = esp_camera_fb_get();
    if (!fb) {
        server.send(500, "text/plain", "Camera capture failed");
        return;
    }

    server.setContentLength(fb->len);
    server.sendHeader("Content-Type", "image/jpeg");
    server.sendHeader("Content-Disposition", "inline; filename=capture.jpg");
    server.sendHeader("Access-Control-Allow-Origin", "*");
    WiFiClient client = server.client();
    client.write(fb->buf, fb->len);

    esp_camera_fb_return(fb);
}

void setup() {
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
    Serial.begin(115200);

    // Connect to WiFi
    WiFi.mode(WIFI_AP_STA);
    WiFi.begin(ssid, password);
    long int startTime = millis();
    while (WiFi.status() != WL_CONNECTED && (millis() - startTime < 10000)) {
        delay(1000);
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.printf("ESP IP Address: http://%s\n", WiFi.localIP().toString().c_str());
    }

    // Setup server routes
    server.on("/", handleRoot);
    server.on("/command", handleCommand);
    server.on("/capture", handleCapture);

    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
    server.handleClient();
}
