# MustMonitor

Qt-based monitoring application for solar inverters and battery systems via Modbus TCP protocol.

This project was created for fun and heavily involved AI agents. Please don't take it too seriously, but some feedback is appreciated!

## Features

- Real-time monitoring of battery state, voltage, current, and power
- Modbus TCP communication with configurable registers
- Clean QML-based dashboard UI with charts and metrics
- Configurable via JSON file
- Cross-platform (macOS, Linux, Windows)

## Requirements

- Qt 6.x
- CMake 3.16+
- libmodbus
- C++17 compiler

## Installation

### macOS
```bash
brew install qt6 libmodbus cmake
```

### Linux (Ubuntu/Debian)
```bash
sudo apt install qt6-base-dev libmodbus-dev cmake build-essential
```

## Build

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

The executable will be in `build/MustMonitor/`.

## Configuration

Edit `config.json` to configure Modbus connection and registers:

```json
{
    "modbus": {
        "host": "192.168.1.43",
        "port": 502,
        "deviceId": 4,
        "pollInterval": 5000
    },
    "registers": [
        {
            "address": 113,
            "name": "Battery State, %",
            "dataType": "UInt16",
            "scale": 1,
            "isSigned": false
        }
    ]
}
```

## Usage

Run with default config (searches for `config.json` in current directory):
```bash
./build/MustMonitor/MustMonitor
```

Run with custom config:
```bash
./build/MustMonitor/MustMonitor /path/to/config.json
```

## Dashboard

The dashboard displays:
- **Battery State of Charge** - Donut chart showing percentage
- **Load Power** - Donut chart showing percentage
- **Invertor Voltage & Current** - Numeric values with labels
- **Grid Voltage & Current** - Numeric values with labels
- **Grid Frequency & Invertor Frequency** - Numeric values with labels

All labels are automatically loaded from register names in `config.json`.

## Register Configuration

Each register in `config.json` requires:
- `address` - Modbus register address
- `name` - Display name (shown in UI)
- `dataType` - Data type (UInt16, Int16)
- `scale` - Scale factor to apply to raw value
- `isSigned` - Whether to interpret as signed integer

## Project Structure

```
MustMonitor/
├── src/              # C++ source files
├── components/       # QML UI components
├── docs/            # Documentation
├── config.json      # Configuration file
└── CMakeLists.txt   # Build configuration
```

## License

MIT
