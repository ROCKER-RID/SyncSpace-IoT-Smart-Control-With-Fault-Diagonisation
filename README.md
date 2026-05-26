# SyncSpace: An Integrated Platform for Seamless Smart Living 🚀

SyncSpace is a web-managed, responsive IoT automation platform designed to bring remote visibility and smart control to appliance infrastructure. This repository hosts the public-facing edge firmware, dynamic peripheral control loops, and local web interfaces running on the ESP32 microcontroller.

> 🔒 **Intellectual Property Note:** This repository showcases the IoT network connectivity, asynchronous web serving modules, and relay driver logic. The high-speed analog signal sampling, real-time wave analytics, and patent-pending automated fault-attribution algorithms executing on the co-processing hardware layer are intentionally excluded for patent protection.

---

## ✨ System Capabilities
* **Smart Appliance Infrastructure:** Multi-channel control lines managing independent local lighting paths and high-power load sockets.
* **Asynchronous Local Dashboard:** A lightweight web application served straight from the ESP32 flash chip, delivering real-time control toggles via WebSockets.
* **Live System Event Logging:** A running telemetry data grid logging physical switch clicks, client connection states, and Wi-Fi system resets.
* **Hardware Efficiency Optimization:** Embedded multiplexer and shift-register scanning arrays configured to manage extensive component layouts over minimal physical microchip pins.

---

## 🔌 Hardware Setup (IoT & Control Domain)
The hardware design focuses on keeping high-voltage mains safely isolated from the low-voltage control electronic circuits:

* **Main IoT Control Module:** ESP32-WROOM-32U.
* **General Appliance Control:** 5V SPDT Relays driven via PC817 optocouplers and 2N2222A NPN transistors.
* **Heavy Utility Socket Safety:** Robust T90 5V 30A isolation relay.
* **Fan Speed Circuitry:** BT136 TRIAC circuits isolated via MOC3021 optocouplers and reinforced with RC snubber circuits (0.1µF X2 capacitors + 47Ω resistors).
* **Pin Expansion Architecture:** 74HC4051 8-channel analog multiplexer arrays and 74HC595 8-bit shift register chains.

---

## 💻 Tech Stack & Embedded Libraries
The software relies on clean, native web structures and robust embedded networking modules without unnecessary external frameworks.

### ESP32 Firmware Environment
* **Framework:** Arduino C++ Core / PlatformIO.
* **Core Libraries:** `WiFi.h` (Network management), `ESPAsyncWebServer.h` (Async server engine), `ArduinoJSON.h` (State updates serialization), and `EEPROM.h` (Persistent memory allocation).

### Responsive Web UI
* **Front-End Stack:** Vanilla HTML5, CSS3 grid formatting, and Native JavaScript.
* **Data Layer:** Bi-directional WebSockets delivering instant state syncing between software controls and physical buttons.

---

## 🛠️ Project Showroom
*(Drag and drop your project pictures from your computer right here inside the GitHub editor!)*

### Live Control Center Dashboard
<img width="1280" height="720" alt="switch controls" src="https://github.com/user-attachments/assets/9d41882b-1dcf-4618-8b00-d4587f5e9714" />


### Telemetry & Event Logging Terminal

https://github.com/user-attachments/assets/be016d2d-ab07-4e29-a8ec-5308e1d7c816

### Assembled Control Hardware PCB
<img width="1280" height="963" alt="pcbimage1" src="https://github.com/user-attachments/assets/28379fff-7fbb-4f30-8082-12e6315293ac" />


---

## 👥 Project Authors (Team SyncSpace)
* **Ridhin George** (SHR23CS161)
* **Sharon Maliakal** (SHR23CS168)
* **R A Swaroop** (SHR23CS159)
* **Aaron A S** (LSHR23CS190)

**Institution:** Department of Computer Science and Engineering, Sahrdaya College of Engineering and Technology, Kodakara.  
**Project Guide & Coordinator:** Ms. Rehna Baby Joseph (Assistant Professor).
