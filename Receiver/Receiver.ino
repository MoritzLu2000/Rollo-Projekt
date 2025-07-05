#include "ESP32_NOW.h"
#include <WiFi.h>

#define STEP_PIN 17  // Pin connected to the STEP input on the driver
#define DIR_PIN 16   // Pin connected to the DIR input on the driver
#define ENABLE_PIN 21
#define LED_PIN 2


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
  digitalWrite(ENABLE_PIN, LOW);

  digitalWrite(LED_PIN, HIGH);
  moveStepper(myData.rotations*15);
  digitalWrite(LED_PIN, LOW);
}


main



void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);

  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(ENABLE_PIN, LOW);
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}


void loop() {
}

// Function to move the stepper motor a specified number of steps
void moveStepper(int steps) {
  if (steps > 0) {
    digitalWrite(DIR_PIN, HIGH);
  } else {
    digitalWrite(DIR_PIN, LOW);
    steps = -steps;
  }

  for (int i = 0; i < steps; i++) {
    digitalWrite(STEP_PIN, HIGH);  // Pulse the step pin
    delayMicroseconds(85);        // Wait for 500 microseconds
    digitalWrite(STEP_PIN, LOW);   // End of pulse
    delayMicroseconds(85);        // Wait for 500 microseconds
  }
}
