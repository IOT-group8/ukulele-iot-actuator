# Ukulele IoT Actuator

This project controls the ukulele's fret pressing and string striking mechanisms using an ESP32. It receives commands via MQTT and actuates the corresponding hardware components.

## Features

- **MQTT Communication**: Listens for control commands to manage the ukulele's actuators.
- **Fret Control**: Manages solenoids to press specific frets.
- **String Striking**: Controls a servo motor to strum strings.

## Getting Started

### Prerequisites

- **Hardware**: ESP32 development board, solenoids, servo motor.
- **Software**: [ESP-IDF v5.4](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/index.html), CMake, Python 3.8 or newer.

### Installation

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/IOT-group8/ukulele-iot-actuator.git
   cd ukulele-iot-actuator
   ```
