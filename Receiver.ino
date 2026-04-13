#include <esp_now.h>
#include <WiFi.h>

#define RELAY_PIN 19
#define INPUT_PIN 32  // Local trigger on the receiver

typedef struct struct_message {
  bool toggle;
} struct_message;

struct_message incomingMessage;
bool relayState = false;

// ESP-NOW receive callback
void onDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  memcpy(&incomingMessage, data, sizeof(incomingMessage));
  if (incomingMessage.toggle) {
    relayState = !relayState;
    digitalWrite(RELAY_PIN, relayState ? LOW : HIGH);  // Active-LOW relay
    Serial.println("Toggled via ESP-NOW");
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);  // Relay OFF (active-LOW)

  pinMode(INPUT_PIN, INPUT_PULLDOWN);  // Local input on receiver

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW initialization failed");
    return;
  }
  esp_now_register_recv_cb(onDataRecv);
}

void loop() {
  static bool lastState = LOW;
  bool currentState = digitalRead(INPUT_PIN);

  // Detect rising edge from local input
  if (lastState == LOW && currentState == HIGH) {
    relayState = !relayState;
    digitalWrite(RELAY_PIN, relayState ? LOW : HIGH);
    Serial.println("Toggled via local input");
    delay(200);  // Debounce
  }

  lastState = currentState;
}
