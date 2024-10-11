#include "esp_camera.h"
#include <Wire.h>

#define I2C_SLAVE_ADDRESS 0x08 // Arduino I2C address
#define IMAGE_WIDTH 320
#define IMAGE_HEIGHT 240
#define TOLERANCE 20

// Function to initialize camera
void initCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = 5;
  config.pin_d1 = 18;
  config.pin_d2 = 19;
  config.pin_d3 = 21;
  config.pin_d4 = 36;
  config.pin_d5 = 39;
  config.pin_d6 = 34;
  config.pin_d7 = 35;
  config.pin_xclk = 0;
  config.pin_pclk = 22;
  config.pin_vsync = 25;
  config.pin_href = 23;
  config.pin_sscb_sda = 26;
  config.pin_sscb_scl = 27;
  config.pin_pwdn = 32;
  config.pin_reset = -1;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if(psramFound()){
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
}

// Function to detect red color and get its position
bool detectRed(camera_fb_t *fb, int &posX, int &posY) {
  int redX = 0, redY = 0, redPixels = 0;
  for (int y = 0; y < IMAGE_HEIGHT; y++) {
    for (int x = 0; x < IMAGE_WIDTH; x++) {
      int index = (y * IMAGE_WIDTH + x) * 3; // BGR pixel data
      uint8_t b = fb->buf[index];
      uint8_t g = fb->buf[index + 1];
      uint8_t r = fb->buf[index + 2];
      if (r > 150 && g < 100 && b < 100) {
        redX += x;
        redY += y;
        redPixels++;
      }
    }
  }

  if (redPixels > 0) {
    posX = redX / redPixels;
    posY = redY / redPixels;
    return true;
  }
  return false;
}

// Function to send position data via I2C
void sendPosition(int posX, int posY) {
  Wire.beginTransmission(I2C_SLAVE_ADDRESS);
  Wire.write(posX);  // Send X position
  Wire.write(posY);  // Send Y position
  Wire.endTransmission();
}

void setup() {
  Serial.begin(115200);
  Wire.begin();  // Start I2C communication as Master
  initCamera();
}

void loop() {
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  int posX, posY;
  if (detectRed(fb, posX, posY)) {
    sendPosition(posX, posY); // Send red object position to Arduino
  } else {
    sendPosition(0xFF, 0xFF); // Send "not found" signal
  }

  esp_camera_fb_return(fb);
  delay(100);
}
