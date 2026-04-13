#include <esp_now.h>
#include <WiFi.h>
#include "esp_wifi.h"

#define BUTTON_PIN 32

uint8_t receiverMAC[] = {0x3C, 0x8A, 0x1F, 0x5D, 0xCA, 0xBC};

typedef struct struct_message {
  bool toggle;
} struct_message;

struct_message msg;
bool lastButtonState = HIGH;

void onDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
  Serial.print("Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW initialization failed");
    return;
  }

  esp_now_register_send_cb(onDataSent);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 1;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  Serial.println("Sender ready.");
}

void loop() {
  bool buttonState = digitalRead(BUTTON_PIN);

  if (lastButtonState == HIGH && buttonState == LOW) {
    Serial.println("Button pressed - sending toggle");
    msg.toggle = true;
    esp_now_send(receiverMAC, (uint8_t *)&msg, sizeof(msg));
    delay(300);
  }

  lastButtonState = buttonState;
}
