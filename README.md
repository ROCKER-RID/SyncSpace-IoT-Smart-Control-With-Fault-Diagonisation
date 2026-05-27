# SyncSpace: An Integrated Platform for Seamless Smart Living 🚀

SyncSpace is a web-managed, responsive IoT automation platform designed to bring remote visibility and smart control to appliance infrastructure. This repository hosts the public-facing edge firmware, dynamic peripheral control loops, centralized server modules, and local web interfaces running on the ESP32 microcontroller.

> 🔒 **Intellectual Property Note:** This repository showcases the IoT network connectivity, asynchronous web serving modules, MQTT message handling, and relay driver logic. The high-speed analog signal sampling, real-time wave analytics, and patent-pending automated fault-attribution algorithms executing on the co-processing hardware layer are intentionally excluded for patent protection.

---

## ✨ System Capabilities

- **Smart Appliance Infrastructure:** Multi-channel control lines managing independent local lighting paths and high-power load sockets.

- **Centralized MQTT Core:** Low-latency telemetry aggregation using an Eclipse Mosquitto broker setup for resilient machine-to-machine messaging.

- **Asynchronous Local Dashboard:** A lightweight web application served straight from the ESP32 and central server ecosystem, delivering real-time control toggles via WebSockets.

- **Live System Event Logging:** A running telemetry data grid logging physical switch clicks, client connection states, broker statuses, and Wi-Fi system resets.

- **Hardware Efficiency Optimization:** Embedded multiplexer and shift-register scanning arrays configured to manage extensive component layouts over minimal physical microchip pins.

---

## 🌐 System & Communication Architecture

SyncSpace features a hybrid communication architecture to ensure immediate hardware response and lightweight, continuous logging:

1. **Edge-to-Broker (MQTT):** The ESP32 node continuously publishes appliance statuses and receives commands over an isolated network using the **Eclipse Mosquitto** broker protocol.

2. **Broker-to-Client (WebSockets):** The application server bridges the data pipeline by broadcasting real-time system changes from the broker directly to consumer web browsers via persistent WebSocket channels.

---

## 🔌 Hardware Setup (IoT & Control Domain)

The hardware design focuses on keeping high-voltage mains safely isolated from the low-voltage control electronic circuits:

- **Main IoT Control Module:** ESP32-WROOM-32U.

- **General Appliance Control:** 5V SPDT Relays driven via PC817 optocouplers and 2N2222A NPN transistors.

- **Heavy Utility Socket Safety:** Robust T90 5V 30A isolation relay.

- **Fan Speed Circuitry:** BT136 TRIAC circuits isolated via MOC3021 optocouplers and reinforced with RC snubber circuits (0.1µF X2 capacitors + 47Ω resistors).

- **Pin Expansion Architecture:** 74HC4051 8-channel analog multiplexer arrays and 74HC595 8-bit shift register chains.

---

## 💻 Tech Stack & Embedded Libraries

The software relies on clean, native web structures and robust embedded networking modules without unnecessary external frameworks.

### ESP32 Firmware Environment

- **Framework:** Arduino C++ Core / PlatformIO.

- **Core Libraries:** `WiFi.h` (Network management), `PubSubClient.h` / `WiFiClient.h` (MQTT broker orchestration), `ESPAsyncWebServer.h`, `ArduinoJSON.h`, and `EEPROM.h` (Persistent memory allocation).

### Server Infrastructure & Responsive Web UI

- **MQTT Broker:** Eclipse Mosquitto.

- **Front-End Stack:** Vanilla HTML5, CSS3 grid formatting, and Native JavaScript.

- **Data Transports:** Asynchronous WebSockets delivering instant state syncing between software controls and physical hardware switches.

---

## 📱 User Manual: How to Deploy & Operate

This operational guide outlines the deployment pipeline for setting up the control node and initializing the live ecosystem dashboard interface.

### 1. Setting Up the Mosquitto MQTT Broker

- Install the Eclipse Mosquitto broker on your server or host computer.

- Ensure the service is configured using your included `mosquitto.conf` file inside the `SYNCSPACE_SERVER` directory.

- Ensure your network firewall permits communication between the server hosting Mosquitto and the ESP32 device node.

### 2. Initializing the Edge Controller Node

- Open your ESP32 source code and set the variable fields to match your environmental variables:

```cpp
const char* ssid     = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* mqtt_server = "YOUR_MOSQUITTO_BROKER_IP";
```

- Flash the firmware code to the ESP32 node. On boot-up, the device will connect to your Wi-Fi access point and authenticate automatically with your local Mosquitto server.

### 3. Running the Live Dashboard

- **Configure the Web Client:** Before opening the dashboard, ensure your web interface's JavaScript file connects to the right broker. Update the MQTT client initialization string with your broker's IP address:

```javascript
const mqttClient = mqtt.connect("mqtt://YOUR-MOSQUITTO-BROKER-IP");
```

- Access the web UI interface by pointing any device browser directly to the server location hosted on your network.

- **Appliance Toggles:** Interact with the custom panel buttons to immediately broadcast an MQTT payload down to the physical SPDT relays.

- **Telemetry Activity Grid:** The system tracking log framework will capture dynamic node changes, server connects, and operational telemetry updates live via the active WebSocket pipelines.

---

## 🛠️ Project Showroom

### Live Control Center Dashboard

<!-- Add dashboard screenshot here -->

### Telemetry & Event Logging Terminal

https://github.com/user-attachments/assets/be016d2d-ab07-4e29-a8ec-5308e1d7c816

### Assembled Control Hardware PCB

<!-- Add PCB image here -->

---

## 👥 Project Authors (Team SyncSpace)

| Name | Role |
|------|------|
| **Ridhin George** | Team Member |
| **Sharon Maliakal** | Team Member |
| **R A Swaroop** | Team Member |
| **Aaron A S** | Team Member |

**Institution:** Department of Computer Science and Engineering, Sahrdaya College of Engineering and Technology, Kodakara.

**Project Guide & Coordinator:** Ms. Rehna Baby Joseph (Assistant Professor).
