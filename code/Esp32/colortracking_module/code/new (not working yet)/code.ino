/* Importing libraries */
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "esp_camera.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "ui_index.h"

const char* ssid = "WARKOP QBAR"; /* Replace your SSID */
const char* password = "nutrisari"; /* Replace your Password */

String Feedback = "";
String Command = "", cmd = "", P1 = "", P2 = "", P3 = "", P4 = "", P5 = "", P6 = "", P7 = "", P8 = "", P9 = "";
byte ReceiveState = 0, cmdState = 1, strState = 1, questionstate = 0, equalstate = 0, semicolonstate = 0;

// initial coordinate val
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
bool isCameraInitialized = false;

void initCamera() {
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
    config.jpeg_quality = 10;  //0-63 lower number means higher quality
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;  //0-63 lower number means higher quality
    config.fb_count = 1;
  }

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    isCameraInitialized = false;
    return;
  }

  //drop down frame size for higher initial frame rate
  sensor_t * s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_CIF);  //UXGA|SXGA|XGA|SVGA|VGA|CIF|QVGA|HQVGA|QQVGA
  isCameraInitialized = true;
}

void ExecuteCommand() {
  if (cmd != "colorDetect") {
    // Optional: Debugging output for other commands
    // Serial.println("cmd= "+cmd+" ,P1= "+P1+" ,P2= "+P2+" ,P3= "+P3+" ,P4= "+P4+" ,P5= "+P5+" ,P6= "+P6+" ,P7= "+P7+" ,P8= "+P8+" ,P9= "+P9);
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
    Serial.println("");
    Serial.println("STAIP: " + WiFi.localIP().toString());
    Feedback = "STAIP: " + WiFi.localIP().toString();
  }
  else if (cmd == "restart") {
    ESP.restart();
  }
  else if (cmd == "initcamera") {
    initCamera();
    Feedback = isCameraInitialized ? "Camera initialized successfully" : "Camera initialization failed";
  }
  else if (cmd == "cm") {
    x_coordinate = P1.toInt();
    y_coordinate = P2.toInt();
    int x_offset = x_coordinate - x_widthMid;
    int y_offset = y_coordinate - y_heightMid;
    
    String positionInfo = "Position Data - X: " + String(x_coordinate) + ", Y: " + String(y_coordinate);
    positionInfo += "\nOffsets - X: " + String(x_offset) + ", Y: " + String(y_offset);
    
    if (x_offset > 0) positionInfo += "\nObject is to the right of center";
    else if (x_offset < 0) positionInfo += "\nObject is to the left of center";
    else positionInfo += "\nObject is centered horizontally";
    
    if (y_offset > 0) positionInfo += "\nObject is below center";
    else if (y_offset < 0) positionInfo += "\nObject is above center";
    else positionInfo += "\nObject is centered vertically";
    
    Serial.println(positionInfo);
    Feedback = positionInfo;
  }
  else if (cmd == "quality" || cmd == "contrast" || cmd == "brightness") {
    if (!isCameraInitialized) {
      Feedback = "Camera not initialized. Please initialize camera first.";
      return;
    }
    sensor_t * s = esp_camera_sensor_get();
    int val = P1.toInt();
    
    if (cmd == "quality") s->set_quality(s, val);
    else if (cmd == "contrast") s->set_contrast(s, val);
    else if (cmd == "brightness") s->set_brightness(s, val);
    
    Feedback = cmd + " set to " + String(val);
  }
  else {
    Feedback = "Command is not defined.";
  }

  if (Feedback == "") {
    Feedback = Command;
  }
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  // Initialize WiFi first
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  long int StartTime = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
    if ((StartTime + 10000) < millis()) {
      Serial.println("\nFailed to connect to WiFi");
      break;
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("\nESP IP Address: http://");
    Serial.println(WiFi.localIP());
  }
  
  // Start the server
  server.begin();
  Serial.println("Server started");
  
  // Try to initialize camera (optional)
  initCamera();
  if (isCameraInitialized) {
    Serial.println("Camera initialized successfully");
  } else {
    Serial.println("Camera initialization skipped or failed - server will still work");
  }
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
              if (!isCameraInitialized) {
                client.println("HTTP/1.1 503 Service Unavailable");
                client.println("Content-Type: text/plain");
                client.println("Access-Control-Allow-Origin: *");
                client.println("Connection: close");
                client.println();
                client.println("Camera not initialized");
              } else {
                camera_fb_t * fb = NULL;
                fb = esp_camera_fb_get();
                if (!fb) {
                  client.println("HTTP/1.1 503 Service Unavailable");
                  client.println("Content-Type: text/plain");
                  client.println("Connection: close");
                  client.println();
                  client.println("Camera capture failed");
                } else {
                  client.println("HTTP/1.1 200 OK");
                  client.println("Access-Control-Allow-Origin: *");
                  client.println("Access-Control-Allow-Headers: Origin, X-Requested-With, Content-Type, Accept");
                  client.println("Access-Control-Allow-Methods: GET,POST,PUT,DELETE,OPTIONS");
                  client.println("Content-Type: image/jpeg");
                  client.println("Content-Disposition: form-data; name=\"imageFile\"; filename=\"picture.jpg\"");
                  client.println("Content-Length: " + String(fb->len));
                  client.println("Connection: close");
                  client.println();

                  uint8_t *fbBuf = fb->buf;
                  size_t fbLen = fb->len;
                  for (size_t n = 0; n < fbLen; n = n + 1024) {
                    if (n + 1024 < fbLen) {
                      client.write(fbBuf, 1024);
                      fbBuf += 1024;
                    }
                    else if (fbLen % 1024 > 0) {
                      size_t remainder = fbLen % 1024;
                      client.write(fbBuf, remainder);
                    }
                  }
                  esp_camera_fb_return(fb);
                }
              }
            }
            else {
              client.println("HTTP/1.1 200 OK");
              client.println("Access-Control-Allow-Headers: Origin, X-Requested-With, Content-Type, Accept");
              client.println("Access-Control-Allow-Methods: GET,POST,PUT,DELETE,OPTIONS");
              client.println("Content-Type: text/html; charset=utf-8");
              client.println("Access-Control-Allow-Origin: *");
              client.println("Connection: close");
              client.println();
              
              String Data = "";
              if (cmd != "") {
                Data = Feedback;
              } else {
                Data = String(html);  // Updated to use 'html' from the new UI header
              }
              
              int Index;
              for (Index = 0; Index < Data.length(); Index += 1000) {
                client.print(Data.substring(Index, Index + 1000));
              }
              client.println();
            }
            Feedback = "";
            break;
          } else {
            currentLine = "";
          }
        }
        else if (c != '\r') {
          currentLine += c;
        }
        if ((currentLine.indexOf("/?") != -1) && (currentLine.indexOf(" HTTP") != -1)) {
          if (Command.indexOf("stop") != -1) {
            client.println();
            client.println();
            client.stop();
          }
          currentLine = "";
          Feedback = "";
          ExecuteCommand();
        }
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