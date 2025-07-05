#include "ESP32_NOW.h"
#include <WiFi.h>

/* Definitions */
int LedPin = 2;
int enablePin = 21;
int STEP_PIN = 17;
int DIR_PIN = 16;

typedef struct struct_message {
  int rotations;
  int speed;
  int maxSpeed;
  int acceleration;
  int status;
  int loops;
} struct_message;

struct_message myData;


void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));

  digitalWrite(LedPin, HIGH);
  runMotor(myData.rotations);
  digitalWrite(LedPin, LOW);
}

void runMotor(int steps) {
  if (steps > 0) {
    digitalWrite(DIR_PIN, HIGH);
  } else {
    digitalWrite(DIR_PIN, LOW);
    steps = -steps;
  }

  for (int i = 0; i < steps; i++) {
    digitalWrite(STEP_PIN, HIGH);  // Pulse the step pin
    delayMicroseconds(300);        // Wait for 500 microseconds
    digitalWrite(STEP_PIN, LOW);   // End of pulse
    delayMicroseconds(300);        // Wait for 500 microseconds
  }
}


/* Main */
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  pinMode(LedPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  
  digitalWrite(enablePin, LOW);

  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

void loop() {
  runMotor(100);
}