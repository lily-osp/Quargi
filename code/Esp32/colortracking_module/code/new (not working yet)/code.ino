/* Importing libraries */
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <esp_camera.h>
#include <soc/soc.h>
#include <soc/rtc_cntl_reg.h>
#include <ESPmDNS.h>
#include "ui_index.h"

const char* ssid = "ssid"; /* Replace your SSID */
const char* password = "password"; /* Replace your Password */
const char* mdns_name = "esp32-camera"; // mDNS hostname

String Feedback = "";
String Command = "", cmd = "", P1 = "", P2 = "", P3 = "", P4 = "", P5 = "", P6 = "", P7 = "", P8 = "", P9 = "";
byte ReceiveState = 0, cmdState = 1, strState = 1, questionstate = 0, equalstate = 0, semicolonstate = 0;

// Initial coordinate values
int x_coordinate = 0;
int y_coordinate = 0;
int x_widthMid = 320;
int y_heightMid = 240;

/* AI-Thinker */
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

WiFiServer server(80);

void ExecuteCommand() {
  if (cmd != "colorDetect") {
    // Debugging output for other commands
    Serial.println("Command executed: " + cmd);
  }

  if (cmd == "resetwifi") {
    WiFi.begin(P1.c_str(), P2.c_str());
    Serial.print("Connecting to ");
    Serial.println(P1);
    long int StartTime = millis();
    while (WiFi.status() != WL_CONNECTED) {
      delay(100);
      if ((StartTime + 5000) < millis()) break;
    }
    Feedback = "STAIP: " + WiFi.localIP().toString();
  } else if (cmd == "restart") {
    ESP.restart();
  } else if (cmd == "cm") {
    x_coordinate = P1.toInt();
    y_coordinate = P2.toInt();

    // Calculate offsets from the center
    int x_offset = x_coordinate - x_widthMid;
    int y_offset = y_coordinate - y_heightMid;

    Serial.println("Coordinates received:");
    Serial.printf("X: %d (Offset: %d), Y: %d (Offset: %d)\n", x_coordinate, x_offset, y_coordinate, y_offset);

    if (x_offset > 0) Serial.println("Right of center");
    else if (x_offset < 0) Serial.println("Left of center");
    else Serial.println("Centered horizontally");

    if (y_offset > 0) Serial.println("Below center");
    else if (y_offset < 0) Serial.println("Above center");
    else Serial.println("Centered vertically");

    Feedback = "Position Data - X: " + String(x_coordinate) + ", Y: " + String(y_coordinate);
  } else if (cmd == "quality") {
    sensor_t* s = esp_camera_sensor_get();
    s->set_quality(s, P1.toInt());
  } else if (cmd == "contrast") {
    sensor_t* s = esp_camera_sensor_get();
    s->set_contrast(s, P1.toInt());
  } else if (cmd == "brightness") {
    sensor_t* s = esp_camera_sensor_get();
    s->set_brightness(s, P1.toInt());
  } else {
    Feedback = "Command not defined.";
  }

  if (Feedback == "") Feedback = Command;
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  Serial.begin(115200);
  Serial.println("\nStarting...");

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

  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("Camera initialization failed.");
    delay(100);
    ESP.restart();
  }

  sensor_t* s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_CIF);

  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);

  long StartTime = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    if ((StartTime + 10000) < millis()) break;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi connected!");
    Serial.print("IP Address: http://");
    Serial.println(WiFi.localIP());
  }

  if (!MDNS.begin(mdns_name)) {
    Serial.println("Error starting mDNS.");
  } else {
    Serial.printf("mDNS started: http://%s.local\n", mdns_name);
  }

  server.begin();
}

void loop() {
  Feedback = ""; Command = ""; cmd = ""; P1 = ""; P2 = ""; P3 = ""; P4 = ""; P5 = ""; P6 = ""; P7 = ""; P8 = ""; P9 = "";
  ReceiveState = 0, cmdState = 1, strState = 1, questionstate = 0, equalstate = 0, semicolonstate = 0;

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
              camera_fb_t* fb = esp_camera_fb_get();
              if (!fb) {
                Serial.println("Camera capture failed.");
                delay(100);
                ESP.restart();
              }

              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: image/jpeg");
              client.println("Connection: close");
              client.println();

              client.write(fb->buf, fb->len);
              esp_camera_fb_return(fb);
              break;
            }
          }
        }
        currentLine += c;
      }
    }
    delay(1);
    client.stop();
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