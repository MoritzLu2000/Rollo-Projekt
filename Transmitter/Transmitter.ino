#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <esp_now.h>
#include <WiFi.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

const int buttonPin1 = 4;
const int buttonPin2 = 19;
const int buttonPin3 = 23;

int menueState = 0;
int number = 0;

uint8_t broadcastAddress1[] = { 0xEC, 0x64, 0xC9, 0x5D, 0xAD, 0x48 };  // 3 rollo
uint8_t broadcastAddress2[] = { 0xC8, 0x2E, 0x18, 0xC3, 0x99, 0xCC };  // 1 rollo
uint8_t broadcastAddress3[] = { 0xC8, 0x2E, 0x18, 0x14, 0x73, 0xEC };
uint8_t broadcastAddress5[] = { 0xC8, 0x2E, 0x18, 0xC3, 0x80, 0xFC };  // 2 rollo
uint8_t broadcastAddress4[] = { 0xAC, 0x67, 0xB2, 0x09, 0xFF, 0x1C };  // 4 rollo

int devices[5];
int rolloState[5];  // 1 for up, 0 for down

const int ledPin = 2;

typedef struct struct_message {
  int rotations;
  int speed;
  int maxSpeed;
  int acceleration;
  int status;
  int loops;
} struct_message;

struct_message myData;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
esp_now_peer_info_t peerInfo;

void menueDisplay(String mode, int anzahlmodi);

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  number++;
  if (number > 5) number = 1;

  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           mac_addr[0], mac_addr[1], mac_addr[2],
           mac_addr[3], mac_addr[4], mac_addr[5]);

  Serial.print("Receiver ");
  Serial.print(number);
  Serial.print(" send status:\t");

  if (status == ESP_NOW_SEND_SUCCESS) {
    Serial.println("Delivery Success");
    devices[number - 1] = 1;
  } else {
    Serial.println("Delivery Fail");
    devices[number - 1] = 0;
  }
}

void alle(bool direction) {
  myData.rotations = direction ? 20000 : -20000;

  esp_now_send(broadcastAddress1, (uint8_t *)&myData, sizeof(myData));
  esp_now_send(broadcastAddress2, (uint8_t *)&myData, sizeof(myData));
  esp_now_send(broadcastAddress3, (uint8_t *)&myData, sizeof(myData));
  esp_now_send(broadcastAddress4, (uint8_t *)&myData, sizeof(myData));
  esp_now_send(broadcastAddress5, (uint8_t *)&myData, sizeof(myData));
}

void rechts(bool direction) {
  myData.rotations = direction ? 20000 : -20000;

  esp_now_send(broadcastAddress2, (uint8_t *)&myData, sizeof(myData));
  esp_now_send(broadcastAddress4, (uint8_t *)&myData, sizeof(myData));
}

void front(bool direction) {
  myData.rotations = direction ? 20000 : -20000;

  esp_now_send(broadcastAddress1, (uint8_t *)&myData, sizeof(myData));
  esp_now_send(broadcastAddress5, (uint8_t *)&myData, sizeof(myData));
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  display.clearDisplay();
  display.display();

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Peers hinzufügen
  memcpy(peerInfo.peer_addr, broadcastAddress1, 6);
  esp_now_add_peer(&peerInfo);
  memcpy(peerInfo.peer_addr, broadcastAddress2, 6);
  esp_now_add_peer(&peerInfo);
  memcpy(peerInfo.peer_addr, broadcastAddress3, 6);
  esp_now_add_peer(&peerInfo);
  memcpy(peerInfo.peer_addr, broadcastAddress4, 6);
  esp_now_add_peer(&peerInfo);
  memcpy(peerInfo.peer_addr, broadcastAddress5, 6);
  esp_now_add_peer(&peerInfo);

  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin1, INPUT_PULLDOWN);
  pinMode(buttonPin2, INPUT_PULLDOWN);
  pinMode(buttonPin3, INPUT_PULLDOWN);
}

void loop() {
  myData.speed = 300;
  myData.maxSpeed = 500;
  myData.acceleration = 300;
  myData.status = 1;
  myData.loops = 4;

  if (digitalRead(buttonPin1)) {
    menueState++;
    if (menueState == 3) menueState = 0;
  }

  switch (menueState) {
    case 0: menueDisplay("alle Rollos", 3); break;
    case 1: menueDisplay("front", 3); break;
    case 2: menueDisplay("rechts", 3); break;
  }

  if (digitalRead(buttonPin3)) {
    if (menueState == 0) alle(true);
    if (menueState == 1) front(true);
    if (menueState == 2) rechts(true);
  }

  if (digitalRead(buttonPin2)) {
    if (menueState == 0) alle(false);
    if (menueState == 1) front(false);
    if (menueState == 2) rechts(false);
  }

  delay(200);
}

void menueDisplay(String mode, int anzahlmodi) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(mode);

  int deltax = 128 / anzahlmodi;
  int x1 = deltax * menueState;
  int x2 = deltax * (menueState + 1);
  display.drawLine(x1, 25, x2, 25, WHITE);
  display.display();
}
