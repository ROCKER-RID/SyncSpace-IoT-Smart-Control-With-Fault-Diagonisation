# SyncSpace: Embedded Smart Control System with Real-Time Fault Detection and Predictive Maintenance Framework 🔌⚡

SyncSpace is a fault-aware embedded power distribution system that introduces **utility-level observability** into institutional electrical infrastructure. It identifies the exact load responsible for electrical failures, enabling precise fault attribution and selective isolation — without requiring major modifications to existing MCB/RCCB setups.

> 🔒 **Intellectual Property Note:** This repository showcases the IoT network connectivity, asynchronous web serving modules, MQTT message handling, and relay driver logic. The high-speed analog signal sampling, real-time waveform analytics, and **patent-pending automated fault-attribution algorithms** executing on the co-processing hardware layer are intentionally excluded for IP protection.

---
---

## 🚀 Project Evolution

### V1 — Smart IoT Automation Platform

The first-generation SyncSpace architecture focused on smart appliance automation and real-time IoT infrastructure management using an ESP32-centric embedded ecosystem.

#### V1 Core Features
- MQTT-based communication
- WebSocket live synchronization
- ESP32-based appliance control
- Relay & TRIAC switching
- Real-time telemetry logging
- Browser-based smart dashboard
- Multiplexer & shift-register hardware optimization

#### V1 Architecture
- Single-controller ESP32 architecture
- MQTT broker communication pipeline
- Web-managed smart appliance ecosystem

---

### V2 — Fault-Aware Embedded Power Infrastructure

The second-generation architecture evolved SyncSpace into a utility-aware embedded electrical infrastructure platform capable of real-time fault attribution and predictive maintenance.

#### Major Innovations Introduced in V2
- Dual-microcontroller architecture (ESP32 + STM32)
- Dedicated co-processing subsystem
- High-speed waveform acquisition
- Real-time fault attribution
- Predictive maintenance framework
- Statistical anomaly detection
- Selective fault isolation
- Utility-level electrical observability

#### V2 Architectural Evolution
- ESP32 → IoT dashboard, MQTT & telemetry bridge
- STM32 → Real-time signal analysis & fault analytics engine

---
## 📋 Abstract

Electrical power distribution systems in institutional environments lack visibility into fault occurrences, making it difficult to identify the exact source of electrical failures. Conventional protection systems such as MCBs and RCCBs isolate entire circuits without indicating the specific load responsible, resulting in unnecessary shutdowns, increased downtime, and inefficient maintenance. SyncSpace addresses this by proposing a fault-aware power distribution system that introduces utility-level observability — enabling precise fault identification, faster diagnosis, and selective isolation. Designed to work alongside existing infrastructure without major modifications, the system enhances efficiency, reduces downtime, and supports more reliable, data-driven maintenance practices.

---

## ✨ Key Features & Innovations

- **Utility-Level Fault Attribution:** Accurately identifies the specific electrical load or utility responsible for a fault — not just that a fault occurred, but *which* device caused it.

- **Selective Fault Isolation:** Instead of shutting down the entire circuit, the system isolates only the affected utility, allowing all other connected devices to continue operating normally.

- **Non-Intrusive Design:** Works alongside existing MCB/RCCB infrastructure without rewiring or replacing protection devices — fully retrofit-compatible.

- **Predictive Maintenance Framework:** Continuous baseline learning followed by statistical anomaly detection for early fault prediction.

- **Scalable Modular Architecture:** Easily expandable — add more sensing channels or utilities without changes to the core system.

- **Real-Time Web Dashboard:** Live fault log, relay control, fan speed control, and WebSocket-based status sync accessible from any browser on the local network.

---

## 🏗️ System Architecture

The system is organized into functional hardware modules:

| Module | Function |
|--------|----------|
| **Input / Sensing** | Measures real-time current from each connected load non-intrusively |
| **Analog Front-End (AFE)** | Conditions raw sensor signals into usable voltage levels; provides AC zero-cross timing reference |
| **Multiplexing** | Sequentially scans all utility channels, reducing required ADC pins |
| **Co-Processing Unit** | *(Patent-pending)* Real-time signal analysis, fault detection logic, and baseline comparison |
| **Fault Isolation** | Selectively disconnects only the faulty load via relay/TRIAC switching |
| **Output Interface** | Real-time visual fault and status indication via status LEDs |
| **IoT Gateway** | ESP32-based remote dashboard, MQTT telemetry, and WebSocket alert bridge |

### Communication Flow

```
Fault Detection Unit
       │
       ▼
Co-Processing Unit  ◄─── (Patent-pending core — excluded from this repo)
       │
       ├──► Fault Detected → Selective Relay/TRIAC Isolation + LED Alert
       │
       └──► Status Normal → ESP32 IoT Gateway → WebSocket Dashboard
```

---

## ⚙️ System Operational Flow

1. **System Initialization** — Hardware peripherals initialize; multiplexer set to Channel 1.
2. **Zero-Cross Sync** — System synchronizes sampling with the AC waveform via ZCD.
3. **Burst Sampling** — ADC samples collected within one AC cycle (~20ms) per channel.
4. **Signal Processing** — RMS, peak, and Crest Factor computed. *(Core algorithm excluded for IP protection.)*
5. **Baseline Decision:**
   - **Learning Mode:** Stores running statistics to build a per-channel reference baseline.
   - **Monitoring Mode:** Compares live values against the stored baseline.
6. **Anomaly Detection** — Significant deviation from baseline flags a fault on the active channel.
7. **Fault Alert & Isolation** — Status LED triggered, alert dispatched via ESP32, faulty channel identified.
8. **Channel Rotation** — Multiplexer advances to the next channel; loop repeats continuously.

---

## 🔌 Hardware Stack

### IoT Gateway
- **ESP32-WROOM-32U** (External antenna, 8MB) — Web dashboard, MQTT/WebSocket bridge, UART interface to co-processor

### Power Control
- **BT136 TRIAC** (600V, 4A) — Fan speed phase control × 3
- **MOC3021 Optocouplers** — TRIAC isolation × 3
- **NT73 5V 10A SPDT Relays** — Light/appliance control × 3
- **NT90 T90 5V 30A Relay** — Heavy-duty power socket control × 1
- **PC817 Optocouplers** — Relay isolation × 5
- **2N2222A NPN Transistors** — Relay/TRIAC drivers × 8
- **RC Snubber Circuits** — 0.1µF X2 310VAC capacitors + 47Ω 1W resistors (per TRIAC)

### Logic & Expansion
- **74HC4051** 8-channel Analog MUX  (channel selection + switch input scanning)
- **74HC595** 8-bit Shift Register  (LED indicator expansion)

### Power Supply
- **HLK-10M05** AC-DC Module — 100–240VAC → 5VDC / 10W (mains-isolated)
- **LM2596** Buck Converter — 5V → 3.3V regulated supply for ESP32

---

## 💻 Software Stack

### ESP32 Firmware
- **Framework:** Arduino C++ Core / PlatformIO
- **Core Libraries:** `WiFi.h`, `PubSubClient.h`, `ESPAsyncWebServer.h`, `ArduinoJSON.h`, `EEPROM.h`
- **Communication:** MQTT (Eclipse Mosquitto broker), WebSocket (real-time dashboard sync), HTTP

### Dashboard & Frontend
- **MQTT Broker:** Eclipse Mosquitto (locally hosted)
- **Frontend:** Vanilla HTML5, CSS3, Native JavaScript
- **Transport:** WebSockets for real-time relay state sync and fault event streaming

---

## 📱 Deployment Guide

### 1. Set Up the Mosquitto MQTT Broker

- Install Eclipse Mosquitto on your local server or host PC.
- Configure the broker using the `mosquitto.conf` file in the `SYNCSPACE_SERVER` directory.
- Ensure your network firewall allows communication between the Mosquitto host and the ESP32 node.

### 2. Configure & Flash the ESP32

- Set your environment credentials in the firmware:

```cpp
const char* ssid        = "YOUR_WIFI_SSID";
const char* password    = "YOUR_WIFI_PASSWORD";
const char* mqtt_server = "YOUR_MOSQUITTO_BROKER_IP";
```

- Flash via PlatformIO or Arduino IDE. On boot, the ESP32 connects to Wi-Fi, authenticates with the Mosquitto broker, and begins bridging fault data to the dashboard.

### 3. Launch the Dashboard

- Update the MQTT client IP in the web interface JS file:

```javascript
const mqttClient = mqtt.connect("mqtt://YOUR-MOSQUITTO-BROKER-IP");
```

- Open any browser on the local network and navigate to the server IP.
- **Switch Controls:** Toggle individual relays to control lighting and appliances.
- **Fan Controls:** Adjust TRIAC phase angle per fan for speed control.
- **Fault Log:** Live event log displays relay states, connection status, and fault attribution alerts.

---

## 🛠️ Project Showroom

### Live Fault Monitoring & Control Dashboard

<img width="1280" height="720" alt="SyncSpace Dashboard"
src="https://github.com/user-attachments/assets/9d41882b-1dcf-4618-8b00-d4587f5e9714" />

### SYNCSPACE Control Panel & Telemetry Terminal

https://github.com/user-attachments/assets/be016d2d-ab07-4e29-a8ec-5308e1d7c816

### Circuit Schematic
### This is designed for 7 utility setup

[SYNCSPACE_AUTOMATION_HARDWARE.pdf](https://github.com/user-attachments/files/28308854/SYNCSPACE_AUTOMATION_HARDWARE.pdf)


### Final Assembled PCB Hardware

<img width="2210" height="1657" alt="Assembled SYNCSPACE Hardware" src="https://github.com/user-attachments/assets/0c04c827-8e1d-4dbc-af64-df88e15eb020" />
<img width="462" height="342" alt="syncspace utility connections" src="https://github.com/user-attachments/assets/c78e830e-cbb4-4f0d-b139-4b24999f8cfa" />
<img width="761" height="655" alt="pcbimagewithprogrammercontrol" src="https://github.com/user-attachments/assets/ee4922df-2cb9-402e-939a-e61639ac4d83" />



---

## 📐 PCB Overview

The hardware spans three interconnected PCBs:

- **PCB 1 — ESP32 Controller Board:** ESP32-WROOM-32U, 74HC4051 MUX, 74HC595 shift register, 3.3V LM2596 buck converter, FPC connectors.
- **PCB 2 — ESP32 Programmer Board:** RESET/BOOT control, UART bridge for programming.
- **PCB 3 — Power Control Board:** T73 relay module (3 lights), T90 relay module (1 power socket), BT136 TRIAC module (3 fans with snubbers), ZCD circuit, HLK-10M05 AC-DC supply.

Main PCB dimensions: **145.4mm × 104.6mm**. ESP32 sub-board: **81.2mm × 36.6mm**.

---

## 🏆 Recognition

SyncSpace won **both the District and State levels** of Kerala's **Young Innovators Programme (YIP)** — a state government initiative under KSUM recognizing student-led innovations with real-world impact.

---

## 👥 Team SyncSpace

| Name | Roll Number |
|------|-------------|
| **Ridhin George** | Team Leader |
| **Sharon Maliakal** | Team Member |
| **R A Swaroop** | Team Member |
| **Aaron A S** | Team Member |

**Institution:** Department of Computer Science and Engineering, Sahrdaya College of Engineering and Technology, Kodakara, Thrissur — 680684.

**Project Guide & Coordinator:** Ms. Rehna Baby Joseph (Assistant Professor, Dept. of CSE).

**Submitted to:** APJ Abdul Kalam Technological University (KTU) in partial fulfillment of the requirements for the award of the Degree of Bachelor of Technology in Computer Science and Engineering — March 2026.
