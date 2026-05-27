#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

// ---------------- WIFI ----------------
const char* ssid = "YOUR-WIFI-SSID";
const char* password = "YOUR-WIFI-PASSWORD";

// ---------------- MQTT ----------------
const char* mqtt_server = "YOUR-MQTT-BROKER-ID"; // your broker IP

WiFiClient espClient;
PubSubClient client(espClient);

// ---------------- PINS ----------------
const int dataPin = 27;
const int latchPin = 26;
const int clockPin = 23;

const int triacPins[3] = {4, 16, 17};
const int zcdPin = 14;

const int S0_PIN = 19;
const int S1_PIN = 18;
const int S2_PIN = 5;
const int COM_PIN = 34;

// ---------------- STATES ----------------
byte relayState = 0;
int fanSpeed[3] = {0, 0, 0};
int lastSwitchState[8] = {0};

// ---------------- TRIAC CONTROL ----------------
volatile unsigned long zeroCrossTime = 0;
volatile bool zcFlag = false;
volatile bool fireTriac[3] = {false};

const int HALF_PERIOD = 10000;

// ---------------- TIMERS ----------------
unsigned long lastSwitchCheck = 0;
const int SWITCH_INTERVAL = 50;

// ---------------- WIFI ----------------
void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

// ---------------- MQTT ----------------
void callback(char* topic, byte* payload, unsigned int length) {
  String msg = "";
  for (int i = 0; i < length; i++) msg += (char)payload[i];

  String t = String(topic);

  // Relay control
  for (int i = 0; i < 5; i++) {
    if (t == "syncspace/relay/" + String(i) + "/cmd") {
      if (msg == "ON") relayState |= (1 << i);
      else relayState &= ~(1 << i);
    }
  }

  // Fan control
  for (int i = 0; i < 3; i++) {
    if (t == "syncspace/fan/" + String(i) + "/cmd") {
      fanSpeed[i] = constrain(msg.toInt(), 0, 100);
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32_SYNC")) {
      for (int i = 0; i < 5; i++)
        client.subscribe(("syncspace/relay/" + String(i) + "/cmd").c_str());

      for (int i = 0; i < 3; i++)
        client.subscribe(("syncspace/fan/" + String(i) + "/cmd").c_str());

    } else {
      delay(2000);
    }
  }
}

// ---------------- SHIFT REGISTER ----------------
void updateShiftRegister() {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, relayState);
  digitalWrite(latchPin, HIGH);
}

// ---------------- ZCD ISR ----------------
void IRAM_ATTR zeroCrossISR() {
  zeroCrossTime = micros();
  zcFlag = true;

  for (int i = 0; i < 3; i++) {
    fireTriac[i] = true;
  }
}

// ---------------- TRIAC HANDLER ----------------
void handleTriacs() {
  for (int i = 0; i < 3; i++) {
    if (!fireTriac[i]) continue;

    fireTriac[i] = false;

    int speed = fanSpeed[i];

    if (speed <= 0) {
      digitalWrite(triacPins[i], LOW);
      continue;
    }

    if (speed >= 99) {
      digitalWrite(triacPins[i], HIGH);
      continue;
    }

    int delayTime = map(speed, 1, 98, HALF_PERIOD - 500, 200);

    // Wait relative to zero cross
    while (micros() - zeroCrossTime < delayTime);

    digitalWrite(triacPins[i], HIGH);
    delayMicroseconds(100);
    digitalWrite(triacPins[i], LOW);
  }
}

// ---------------- SWITCHES ----------------
void selectChannel(int ch) {
  digitalWrite(S0_PIN, bitRead(ch, 0));
  digitalWrite(S1_PIN, bitRead(ch, 1));
  digitalWrite(S2_PIN, bitRead(ch, 2));
}

void checkSwitches() {
  for (int ch = 0; ch < 8; ch++) {
    selectChannel(ch);
    delayMicroseconds(50);

    int val = analogRead(COM_PIN);
    bool state = (val < 1000);

    if (state != lastSwitchState[ch]) {
      lastSwitchState[ch] = state;

      if (state) {
        if (ch < 5) {
          // toggle locally
          relayState ^= (1 << ch);

          // publish
          client.publish(("syncspace/relay/" + String(ch) + "/state").c_str(),
                         (relayState & (1 << ch)) ? "ON" : "OFF");
        } else {
          int fan = ch - 5;

          if (fanSpeed[fan] == 0) fanSpeed[fan] = 33;
          else if (fanSpeed[fan] == 33) fanSpeed[fan] = 66;
          else if (fanSpeed[fan] == 66) fanSpeed[fan] = 100;
          else fanSpeed[fan] = 0;

          client.publish(("syncspace/fan/" + String(fan) + "/state").c_str(),
                         String(fanSpeed[fan]).c_str());
        }
      }
    }
  }
}

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(115200);

  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);

  for (int i = 0; i < 3; i++) {
    pinMode(triacPins[i], OUTPUT);
    digitalWrite(triacPins[i], LOW);
  }

  pinMode(zcdPin, INPUT_PULLUP);

  pinMode(S0_PIN, OUTPUT);
  pinMode(S1_PIN, OUTPUT);
  pinMode(S2_PIN, OUTPUT);
  pinMode(COM_PIN, INPUT);

  setup_wifi();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  attachInterrupt(digitalPinToInterrupt(zcdPin), zeroCrossISR, FALLING);
}

// ---------------- LOOP ----------------
void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  unsigned long now = millis();

  if (now - lastSwitchCheck > SWITCH_INTERVAL) {
    lastSwitchCheck = now;
    checkSwitches();
    updateShiftRegister();
  }

  handleTriacs();

  // heartbeat
  static unsigned long lastHB = 0;
  if (now - lastHB > 5000) {
    client.publish("syncspace/esp32/status", "ONLINE");
    lastHB = now;
  }
}