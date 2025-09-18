#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

#define STEP_PIN    17
#define DIR_PIN     16
#define ENABLE_PIN  21
#define LED_PIN     2

bool state = false;

// Globale Variablen für Step-Zähler
volatile int stepsDone = 0;
volatile int stepsTarget = 0;

// Zeiger zum Hardware-Timer
hw_timer_t* periodicTimer = nullptr;

// Prototypes
void IRAM_ATTR onISR();
void startPeriodicUs(uint32_t us);
void stopPeriodic();
void destroyTimer();
void OnDataRecv(const uint8_t* mac, const uint8_t* incomingData, int len);

struct struct_message {
  int rotations, speed, maxSpeed, acceleration, status, loops;
};
struct_message myData;

// === ISR ===
void IRAM_ATTR onISR() { 
  if (state) {
    digitalWrite(STEP_PIN, HIGH);  
    state = false;
  } else {
    digitalWrite(STEP_PIN, LOW);   
    state = true;
  }

  stepsDone++;
  if (stepsDone >= stepsTarget) {
    stopPeriodic();
    stepsDone = 0;
  }
}

void startPeriodicUs(uint32_t us) {
  if (periodicTimer == nullptr) {
    periodicTimer = timerBegin(1, 80, true);   // Timer 1, 1 µs Takt
    timerAttachInterrupt(periodicTimer, &onISR, true);
  }
  timerWrite(periodicTimer, 0);
  timerAlarmWrite(periodicTimer, us, true);    // auto-reload
  timerAlarmEnable(periodicTimer);
}

void stopPeriodic() {
  if (periodicTimer) timerAlarmDisable(periodicTimer);
}

void destroyTimer() {
  if (periodicTimer) {
    timerEnd(periodicTimer);
    periodicTimer = nullptr;
  }
}

void OnDataRecv(const uint8_t* mac, const uint8_t* incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  digitalWrite(ENABLE_PIN, LOW);
  digitalWrite(LED_PIN, HIGH);

  stepsTarget = myData.rotations * 15;
  stepsDone = 0;

  // Richtung einstellen
  if (stepsTarget > 0) {
    digitalWrite(DIR_PIN, HIGH);
  } else {
    digitalWrite(DIR_PIN, LOW);
    stepsTarget = -stepsTarget;
  }

  // Timer starten
  startPeriodicUs(85);
  digitalWrite(LED_PIN, LOW);
}

void setup() {
  Serial.begin(9600);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init error");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {}
