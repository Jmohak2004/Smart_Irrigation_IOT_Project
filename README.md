# Smart Irrigation System Using ESP32 and Blynk IoT

An IoT-based smart irrigation system built on the **ESP32** microcontroller that automatically controls a water pump based on real-time soil moisture and temperature readings. Remote monitoring and manual override are provided through the **Blynk** IoT platform. The system is prototyped and validated using the **Wokwi** browser-based simulator.

## How It Works

The ESP32 continuously reads two sensors every 2 seconds:

| Condition | Pump Action |
|---|---|
| Soil moisture < 40% | **ON** (auto) |
| Temperature > 50 °C | **ON** (auto) |
| Both within normal range | **OFF** (auto) |
| Manual override via Blynk app | Follows user command |

When neither trigger condition is met, the pump stays off. A manual override from the Blynk mobile app takes precedence over automatic control; pressing the auto-mode button restores rule-based operation.

## Hardware / Simulation Components

| Component | GPIO | Role |
|---|---|---|
| ESP32 DevKit C v4 | — | MCU, WiFi, ADC |
| DHT22 (AM2302) | 15 | Temperature & humidity sensor |
| Capacitive Soil Moisture Sensor (potentiometer in sim) | 34 | Soil water content (analog) |
| Single-channel Relay Module | 5 | Switches the water pump |
| Water Pump / LED (in sim) | Relay NO | Actuator / visual indicator |

## Circuit Connections

```
DHT22  VCC  → ESP32 3.3V
DHT22  GND  → ESP32 GND
DHT22  DATA → ESP32 GPIO 15  (10 kΩ pull-up to 3.3V)

Soil Sensor / Pot  VCC  → ESP32 3.3V
Soil Sensor / Pot  GND  → ESP32 GND
Soil Sensor / Pot  AOUT → ESP32 GPIO 34

Relay  VCC → ESP32 VIN (5V)
Relay  GND → ESP32 GND
Relay  IN  → ESP32 GPIO 5
```

## Blynk Virtual Pin Map

| Pin | Direction | Purpose |
|---|---|---|
| V0 | App → Device | Manual pump ON/OFF toggle |
| V1 | Device → App | Temperature (°C) |
| V2 | Device → App | Humidity (%) |
| V3 | Device → App | Soil moisture (%) |
| V4 | App → Device | Switch back to auto mode |

## Project Structure

```
├── src/
│   └── main.cpp          # ESP32 firmware (Arduino/PlatformIO)
├── diagram.json           # Wokwi circuit diagram
├── wokwi.toml             # Wokwi simulator config
├── platformio.ini         # PlatformIO build configuration
├── libraries.txt          # Required Arduino libraries
└── main.py                # IEEE-format PDF report generator
```

## Prerequisites

- [PlatformIO](https://platformio.org/) (CLI or VS Code extension)
- [Wokwi](https://wokwi.com/) account for simulation (or the VS Code Wokwi extension)
- A [Blynk](https://blynk.io/) account with a template configured for the virtual pins listed above

## Getting Started

### 1. Clone the repository

```bash
git clone <repo-url>
cd IOT
```

### 2. Configure Blynk credentials

Open `src/main.cpp` and replace the placeholder values with your own Blynk template ID, template name, and auth token:

```cpp
#define BLYNK_TEMPLATE_ID   "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "YOUR_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN     "YOUR_AUTH_TOKEN"
```

### 3. Build and upload (physical board)

```bash
pio run --target upload
```

### 4. Run in Wokwi simulator

Open the project in VS Code with the Wokwi extension installed, or upload the files to [wokwi.com](https://wokwi.com/). The `diagram.json` and `wokwi.toml` files configure the virtual circuit automatically.

### 5. Generate the PDF report (optional)

```bash
pip install reportlab
python main.py
```

This produces an IEEE-formatted project report (`New3.pdf`).

## Dependencies

Defined in `platformio.ini`:

- [Blynk](https://github.com/blynkkk/blynk-library) — cloud IoT connectivity
- [DHT sensor library](https://github.com/adafruit/DHT-sensor-library) — DHT22 driver
- [Adafruit Unified Sensor](https://github.com/adafruit/Adafruit_Sensor) — sensor abstraction layer

## Authors

- **Khyati Shah** — 16010423048
- **Mohak Jaiswal** — 16010423054

Department of Information & Technology — Academic Year 2025-26

## License

This project was developed as a mini-project for academic purposes.
