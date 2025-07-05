#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "ESP32_NOW.h"
#include <WiFi.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C


const int buttonPin1 = 4;
const int buttonPin2 = 19;
const int buttonPin3 = 23;
int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;

int menueState = 0;  //counter to move thru the menue

int number;  //counter for Serial Print to show which recievery did recieve their message and which didn't

uint8_t broadcastAddress1[] = { 0xEC, 0x64, 0xC9, 0x5D, 0xAD, 0x48 };           // 3 rollo
uint8_t broadcastAddress2[] = { 0xC8, 0x2E, 0x18, 0xC3, 0x99, 0xCC };           // 1 rollo
uint8_t broadcastAddress3[] = { 0xC8, 0x2E, 0x18, 0x14, 0x73, 0xEC };
uint8_t broadcastAddress5[] = { 0xC8, 0x2E, 0x18, 0xC3, 0x80, 0xFC };           // 2 rollo
uint8_t broadcastAddress4[] = { 0xAC, 0x67, 0xB2, 0x09, 0xFF, 0x1C };           // 4 rollo




int devices[4];

typedef struct struct_message {
  int rotations;
  int speed;
  int maxSpeed;
  int acceleration;
  int status;
  int loops;
} struct_message;

struct_message myData;


int rolloState[5];  //1 for up 0 for down

const int ledPin = 2;


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
esp_now_peer_info_t peerInfo;



void menueDisplay(String mode, int anzahlmodi);






// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if (number == 5) {
    Serial.println("//////////////////////////////");
    number = 1;
  } else {
    number++;
  }
  char macStr[18];
  //Serial.print("Packet from: ");
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  //Serial.print(macStr);
  Serial.print(" Receiver");
  Serial.print(number);
  Serial.print(" send status:\t");
  if (status == ESP_NOW_SEND_SUCCESS) {
    Serial.println("Delivery Success");
    devices[number - 1] = 1;
    //Serial.println("1");

  } else {
    Serial.println("Delivery Fail");
    devices[number - 1] = 0;
    //Serial.println("0");
  }
}

void alle(bool direction){
  if(direction){
    myData.rotations = 20000;

  }else{
    myData.rotations = -20000;
  }
    esp_err_t result1 = esp_now_send(broadcastAddress1, (uint8_t *)&myData, sizeof(myData));
    esp_err_t result2 = esp_now_send(broadcastAddress2, (uint8_t *)&myData, sizeof(myData));
    esp_err_t result3 = esp_now_send(broadcastAddress3, (uint8_t *)&myData, sizeof(myData));
    esp_err_t result4 = esp_now_send(broadcastAddress4, (uint8_t *)&myData, sizeof(myData));
    esp_err_t result5 = esp_now_send(broadcastAddress5, (uint8_t *)&myData, sizeof(myData));
}

void rechts(bool direction){
  if(direction){
    myData.rotations = 20000;

  }else{
    myData.rotations = -20000;
  }
  esp_err_t result2 = esp_now_send(broadcastAddress2, (uint8_t *)&myData, sizeof(myData));
  esp_err_t result4 = esp_now_send(broadcastAddress4, (uint8_t *)&myData, sizeof(myData));


}

void front(bool direction){
  if(direction){
    myData.rotations = 20000;

  }else{
    myData.rotations = -20000;
  }
  esp_err_t result1 = esp_now_send(broadcastAddress1, (uint8_t *)&myData, sizeof(myData));
  esp_err_t result5 = esp_now_send(broadcastAddress5, (uint8_t *)&myData, sizeof(myData));
}


void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  memcpy(peerInfo.peer_addr, broadcastAddress1, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  memcpy(peerInfo.peer_addr, broadcastAddress2, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  memcpy(peerInfo.peer_addr, broadcastAddress3, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  memcpy(peerInfo.peer_addr, broadcastAddress4, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  memcpy(peerInfo.peer_addr, broadcastAddress5, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  for (int i = 0; i < 5; i++) {
    Serial.print(devices[i]);
  }

  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin1, INPUT_PULLDOWN);
  pinMode(buttonPin2, INPUT_PULLDOWN);
  pinMode(buttonPin3, INPUT_PULLDOWN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }


}

void loop() {
  // Set values to send
  myData.rotations = 20000;
  myData.speed = 300;
  myData.maxSpeed = 500;
  myData.acceleration = 300;
  myData.status = 1;
  myData.loops = 4;

  if (digitalRead(buttonPin1)) {
    menueState++;
    if (menueState == 3) {
      menueState = 0;  
    }
  }

  switch (menueState) {
    case 0:
      menueDisplay("alle Rollos", 3);
      break;  // Beendet den switch-Block
    case 1:
      menueDisplay("front", 3);
      break;  // Beendet den switch-Block
    case 2:
      menueDisplay("rechts", 3);
      break;  // Beendet den switch-Block
  }


  if (digitalRead(buttonPin3)) {
    if(menueState == 0){
      alle(true);
    }if(menueState == 1){
      front(true);
    }if(menueState == 2){
      rechts(true);
    }

  }

  if (digitalRead(buttonPin2)) {
    if(menueState == 0){
      alle(false);
    }if(menueState == 1){
      front(false);
    }if(menueState == 2){
      rechts(false);
    }
  }
  
  delay(200);
}

void menueDisplay(String mode, int anzahlmodi){
  display.clearDisplay();
  display.setTextSize(1.8);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(mode);


  int deltax = 128/anzahlmodi;
  int x1 = deltax*menueState;
  int x2 = deltax*(menueState+1);
  display.drawLine(x1, 25, x2, 25, WHITE);  // Linie von (10,10) nach (118,54)
  display.display();

}
