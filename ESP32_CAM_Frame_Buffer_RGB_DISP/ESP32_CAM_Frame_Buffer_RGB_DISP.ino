#include "esp_camera.h"
#include <WiFi.h>

// Camera module pin configuration
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

void setup() {
  Serial.begin(500000);
  // Initialize the camera
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
  config.pixel_format = PIXFORMAT_RGB565; // RGB565 format

  // Init with high specs to pre-allocate larger buffers

    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 10;
    config.fb_count = 1;
  

  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  Serial.println("booting...");
  delay(300);
  Serial.println("booting...");
  delay(300);
  Serial.println("booting...");
  delay(300);
}

void loop() {
  // Capture a frame
  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  // Extract and display RGB values of the central line
  int height = fb->height;
  int width = fb->width;
  
for (int x = 0; x <width; x++){
  for (int y = 0; y<height; y++){
    uint16_t pixel = ((uint16_t *)fb->buf)[y * fb->width + x];
    //pixel = reverse16(pixel);
    //print16BitBinary(pixel);
    uint8_t r = (pixel >> 10) & 0x1F;
    uint8_t g = (pixel >> 5) & 0x1F;
    uint8_t b = pixel & 0x1F;

    // Convert 5-bit color to 8-bit color
    // r = (r * 255) / 31;
    // g = (g * 255) / 31;
    // b = (b * 255) / 31;

    Serial.printf("%3d,%3d,%3d-", r, g, b);
    
    
  }
  Serial.print("|");
}
Serial.println("");
Serial.println("");
Serial.println("Frame Capture Complete");

  // Return the frame buffer back to the driver for reuse
  esp_camera_fb_return(fb);

  delay(3000); // Capture and display every 10 seconds
}

uint16_t reverse16(uint16_t num) {
    uint16_t reversedNum = 0;
    for (int i = 0; i < 16; i++) {
        if (num & (1 << i)) {
            reversedNum |= (1 << (15 - i));
        }
    }
    return reversedNum;
}

void print16BitBinary(uint16_t pixel) {
    for (int i = 15; i >= 0; i--) {
        Serial.print(bitRead(pixel, i));
    }
    Serial.print("-");
}
