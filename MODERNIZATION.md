# FDC1004 Arduino Library - Modernized Version

This is an improved and modernized version of the Arduino library for the FDC1004 capacitance sensor breakout board.

## What's New

### 🚀 Improved API Design

- **Type-safe enumerations** for channels, sample rates, and measurement slots
- **Comprehensive error handling** with specific error codes
- **Automatic CAPDAC adjustment** for extended measurement range
- **Modern C++ practices** with better const-correctness and parameter validation

### 📊 Enhanced Measurement Interface

```cpp
// New API interface
FDC1004 sensor(FDC1004_RATE_100HZ);
sensor.begin();

// Get detailed measurement with all metadata
fdc1004_capacitance_t measurement = sensor.getCapacitanceMeasurement(FDC1004_CHANNEL_0);
Serial.print("Capacitance: ");
Serial.print(measurement.capacitance_pf, 4);
Serial.println(" pF");

// Or use the simple interface
float capacitance = sensor.getCapacitancePicofarads(FDC1004_CHANNEL_0);
```

### 🔧 Better Configuration Management

```cpp
// Set sample rate
sensor.setSampleRate(FDC1004_RATE_400HZ);

// Manual CAPDAC control if needed
sensor.setCapdac(FDC1004_CHANNEL_0, 15);

// Check current settings
fdc1004_sample_rate_t rate = sensor.getSampleRate();
uint8_t capdac = sensor.getCapdac(FDC1004_CHANNEL_0);
```

### 🛡️ Robust Error Handling

```cpp
if (!sensor.begin()) {
    Serial.println("Failed to initialize sensor");
    return;
}

if (!sensor.isConnected()) {
    Serial.println("Sensor not responding");
    return;
}

fdc1004_capacitance_t result = sensor.getCapacitanceMeasurement(FDC1004_CHANNEL_0);
if (isnan(result.capacitance_pf)) {
    Serial.println("Measurement failed");
} else if (result.capdac_out_of_range) {
    Serial.println("CAPDAC adjustment needed");
}
```

## API Reference

### Data Types

#### `fdc1004_error_t`
Error codes returned by library functions:
- `FDC1004_SUCCESS` - Operation completed successfully
- `FDC1004_ERROR_INVALID_PARAMETER` - Invalid parameter provided
- `FDC1004_ERROR_MEASUREMENT_NOT_READY` - Measurement not completed yet
- `FDC1004_ERROR_I2C_COMMUNICATION` - I2C communication error
- `FDC1004_ERROR_DEVICE_NOT_FOUND` - Device not responding
- `FDC1004_ERROR_CAPDAC_OUT_OF_RANGE` - CAPDAC value adjustment needed

#### `fdc1004_sample_rate_t`
Available sample rates:
- `FDC1004_RATE_100HZ` - 100 Hz sample rate
- `FDC1004_RATE_200HZ` - 200 Hz sample rate  
- `FDC1004_RATE_400HZ` - 400 Hz sample rate

#### `fdc1004_channel_t`
Channel identifiers:
- `FDC1004_CHANNEL_0` through `FDC1004_CHANNEL_3`

#### `fdc1004_capacitance_t`
Processed measurement result:
```cpp
typedef struct {
    float capacitance_pf;       // Capacitance in picofarads
    bool capdac_out_of_range;   // True if CAPDAC needs adjustment
    uint8_t capdac_used;        // CAPDAC value used for measurement
} fdc1004_capacitance_t;
```

### Main Methods

#### Initialization
- `FDC1004(sample_rate, address)` - Constructor with sample rate and I2C address
- `bool begin()` - Initialize the sensor
- `bool isConnected()` - Check if device is responding

#### High-Level Measurements
- `fdc1004_capacitance_t getCapacitanceMeasurement(channel)` - Get detailed measurement
- `float getCapacitancePicofarads(channel)` - Get simple capacitance reading

#### Configuration
- `fdc1004_error_t setSampleRate(rate)` - Set measurement sample rate
- `fdc1004_sample_rate_t getSampleRate()` - Get current sample rate
- `fdc1004_error_t setCapdac(channel, value)` - Set CAPDAC for channel
- `uint8_t getCapdac(channel)` - Get current CAPDAC value

#### Low-Level Interface
- `configureMeasurementSingle()` - Configure a measurement slot
- `triggerSingleMeasurement()` - Trigger measurement
- `readMeasurement()` - Read measurement results
- `measureChannel()` - Complete measurement cycle

## Backward Compatibility

The library maintains full backward compatibility with the original API:

```cpp
// Legacy interface still works
FDC1004 FDC(FDC1004_100HZ);
int32_t capacitance = FDC.getCapacitance(0); // Returns femtofarads
```

## Key Improvements Over Original

### Code Organization
- **Separated constants** into logical groups with proper naming
- **Clear documentation** for all functions and parameters
- **Consistent naming conventions** throughout the codebase
- **Modular design** with separate concerns for I2C, measurement, and conversion

### Error Handling
- **Comprehensive error codes** instead of simple integer returns
- **Parameter validation** for all public functions
- **I2C communication error detection** and reporting
- **Graceful failure handling** with meaningful error messages

### Functionality
- **Automatic CAPDAC adjustment** to extend measurement range
- **Picofarad units** as the primary measurement unit (more intuitive)
- **Device presence detection** for better diagnostics
- **Configurable I2C address** support

### Performance
- **Optimized I2C operations** with proper error checking
- **Reduced magic numbers** and improved readability
- **Better memory usage** with const correctness
- **Faster development** with type-safe enumerations

## Hardware Information

### FDC1004 Features
- 4-channel capacitance-to-digital converter
- 24-bit resolution
- ±15% accuracy
- I2C interface
- Single-ended and differential measurements
- Programmable CAPDAC for offset adjustment

### Connection Diagram
```
Arduino Uno    FDC1004 Breakout
-----------    ----------------
5V         ->  VIN
GND        ->  GND  
A4 (SDA)   ->  SDA
A5 (SCL)   ->  SCL
```

## Examples

1. **Example1-FDC-streaming** - Updated to use new API with automatic CAPDAC adjustment
2. **Example2-New-API** - Demonstrates advanced new API features with multi-channel measurement
3. **Example3-Legacy-Compatibility** - Shows original API still works unchanged for backward compatibility

## License

MIT License - Same as original library

## Contributing

When contributing to this library, please maintain:
- Backward compatibility with the original API
- Comprehensive documentation for new features
- Type safety and parameter validation
- Consistent coding style and naming conventions
