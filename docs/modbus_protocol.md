# Modbus TCP Register Mapping

## Connection Settings
- **Protocol**: Modbus TCP
- **Default Port**: 502
- **Server Address**: Configure in main.cpp (default: 192.168.1.100)
- **Polling Interval**: 1 second
- **Timeout**: 1000ms
- **Retries**: 3

## Register Map (Holding Registers)

| Register | Address | Parameter | Unit | Scale Factor | Data Type |
|----------|---------|-----------|------|--------------|-----------|
| 0 | 0x0000 | Battery Voltage | V | 0.1 | uint16 |
| 1 | 0x0001 | Battery Current | A | 0.1 | int16 |
| 2 | 0x0002 | Battery SOC | % | 1.0 | uint16 |
| 3 | 0x0003 | Battery Temperature | °C | 0.1 | int16 |
| 4 | 0x0004 | Load Power | W | 1.0 | uint16 |
| 5 | 0x0005 | PV Voltage | V | 0.1 | uint16 |

## Configuration

### Change Server Address
Edit `main.cpp`:
```cpp
modbusClient.connectToServer("YOUR_IP_ADDRESS", 502);
```

### Adjust Register Mapping
Edit `modbusclient.cpp` in the `readRegisters()` method:
```cpp
// Change starting address and count
QModbusDataUnit readUnit(QModbusDataUnit::HoldingRegisters, START_ADDRESS, COUNT);
```

### Adjust Scale Factors
Edit `modbusclient.cpp` in the `onReadReady()` method:
```cpp
double batteryVoltage = unit.value(0) * YOUR_SCALE_FACTOR;
```

### Change Polling Interval
Edit `modbusclient.cpp` in the constructor:
```cpp
m_pollTimer->setInterval(YOUR_INTERVAL_MS); // milliseconds
```

## Register Types Supported
- **Holding Registers** (Read/Write) - Currently used
- **Input Registers** (Read-only)
- **Coils** (Read/Write bits)
- **Discrete Inputs** (Read-only bits)

To change register type, modify in `modbusclient.cpp`:
```cpp
QModbusDataUnit readUnit(QModbusDataUnit::InputRegisters, 0, 10);
```

## Troubleshooting

### Connection Issues
- Verify IP address and port
- Check firewall settings
- Ensure Modbus server is running
- Check network connectivity

### Data Issues
- Verify register addresses match your device
- Adjust scale factors based on device documentation
- Check data types (signed vs unsigned)
- Verify byte order (endianness) if needed

## Example Devices
This implementation is compatible with:
- Must Solar inverters
- Growatt inverters
- Victron Energy devices
- Generic Modbus TCP devices

Adjust register mapping according to your specific device's documentation.
