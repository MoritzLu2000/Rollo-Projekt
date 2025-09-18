#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

#define STEP_PIN 17
#define DIR_PIN 16
#define ENABLE_PIN 21
#define LED_PIN 2

#define stepsdone
#define steps

bool state = false;


struct struct_message { int rotations, speed, maxSpeed, acceleration, status, loops; };
struct_message myData;

//Zeiger zum hardware timer
hw_timer_t* periodicTimer = nullptr;
timerAttachInterrupt(periodicTimer, &onISR, true);




// Prototypes
void moveStepper(int steps);
void OnDataRecv(const uint8_t* mac, const uint8_t* incomingData, int len);
void startPeriodicUs(uint32_t us);
void stopPeriodic();
void destroyTimer();




void IRAM_ATTR onISR() { 
  if(state){
    digitalWrite(STEP_PIN, HIGH);  
    state = false;

  }else{
    digitalWrite(STEP_PIN, LOW);   
    state = true;
  }
  stepsdone++;
  if(stestepsdone == steps){
    stopPeriodic();
  }
}

void startPeriodicUs(uint32_t us) {
  if (periodicTimer == nullptr) {                 // noch keiner angelegt?
    periodicTimer = timerBegin(1, 80, true);      // Timer 1, 1 µs Takt
    timerAttachInterrupt(periodicTimer, &onISR, true);
  }
  timerWrite(periodicTimer, 0);
  timerAlarmWrite(periodicTimer, us, true);       // auto-reload
  timerAlarmEnable(periodicTimer);
}

void stopPeriodic() {
  if (periodicTimer) timerAlarmDisable(periodicTimer);
}

void destroyTimer() {
  if (periodicTimer) { timerEnd(periodicTimer); periodicTimer = nullptr; }
}







// Implementierung
void moveStepper(int steps) {
  if (steps > 0) digitalWrite(DIR_PIN, HIGH);
  else { digitalWrite(DIR_PIN, LOW); steps = -steps; }
  for (int i = 0; i < steps; i++) {
    digitalWrite(STEP_PIN, HIGH);  
    delayMicroseconds(85);
    digitalWrite(STEP_PIN, LOW);   
    delayMicroseconds(85);
  }
}


void OnDataRecv(const uint8_t* mac, const uint8_t* incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  digitalWrite(ENABLE_PIN, LOW);
  digitalWrite(LED_PIN, HIGH);
  //::moveStepper(myData.rotations * 15);   // global eindeutig
  steps = myData.rotations * 15;
  // set direction with dir pin
  if (steps > 0){
    digitalWrite(DIR_PIN, HIGH);
  }else{ 
    digitalWrite(DIR_PIN, LOW); 
    steps = -steps; 
  }
  // start timer
  startPeriodicUs(85);


  digitalWrite(LED_PIN, LOW);
}

void setup() {
  Serial.begin(9600);
  pinMode(STEP_PIN, OUTPUT); pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT); pinMode(LED_PIN, OUTPUT);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) { Serial.println("ESP-NOW init error"); return; }
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {}
