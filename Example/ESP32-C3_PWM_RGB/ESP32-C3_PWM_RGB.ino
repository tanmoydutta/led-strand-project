#include <Arduino.h>

// Pin definitions
const int redPin   = 3;
const int greenPin = 4;
const int bluePin  = 5;

// PWM configuration
const uint32_t pwmFreq       = 5000;  // 5 kHz
const uint8_t  pwmResolution = 8;     // 8-bit (0-255)

// HSV→RGB conversion (for smooth fade)
void hsvToRgb(float h, float s, float v, uint8_t &r, uint8_t &g, uint8_t &b) {
  float c = v * s;
  float x = c * (1 - fabs(fmod(h / 60.0, 2) - 1));
  float m = v - c;
  float r1, g1, b1;

  if (h < 60)       { r1 = c; g1 = x; b1 = 0; }
  else if (h < 120) { r1 = x; g1 = c; b1 = 0; }
  else if (h < 180) { r1 = 0; g1 = c; b1 = x; }
  else if (h < 240) { r1 = 0; g1 = x; b1 = c; }
  else if (h < 300) { r1 = x; g1 = 0; b1 = c; }
  else              { r1 = c; g1 = 0; b1 = x; }

  r = (uint8_t)((r1 + m) * 255);
  g = (uint8_t)((g1 + m) * 255);
  b = (uint8_t)((b1 + m) * 255);
}

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  Serial.println("ESP32-C3 RGB Fade (new LEDC API)");

  // Attach each pin to LEDC with new API
  if (!ledcAttach(redPin,   pwmFreq, pwmResolution))   Serial.println("Error attaching redPin");
  if (!ledcAttach(greenPin, pwmFreq, pwmResolution))   Serial.println("Error attaching greenPin");
  if (!ledcAttach(bluePin,  pwmFreq, pwmResolution))   Serial.println("Error attaching bluePin");
}

void loop() {
  static float hue = 0.0;
  uint8_t r, g, b;

  hsvToRgb(hue, 1.0, 1.0, r, g, b);

  // Write PWM duty via new API
  ledcWrite(redPin,   r);
  ledcWrite(greenPin, g);
  ledcWrite(bluePin,  b);

  hue += 1.0;
  if (hue >= 360.0) hue = 0.0;

  delay(15); // adjust to change fade speed
}
