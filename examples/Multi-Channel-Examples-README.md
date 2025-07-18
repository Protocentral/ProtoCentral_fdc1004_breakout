# FDC1004 Multi-Channel Examples

This directory contains examples demonstrating multi-channel capacitance measurement using the FDC1004 sensor.

## Examples Overview

### Example4-MultiChannel-Acquisition
**Advanced multi-channel demo with statistics and real-time monitoring**

Features:
- Simultaneous measurement of all 4 channels
- Three display modes (Real-time, Statistics, Raw Data)
- Statistical analysis (min, max, average)
- Automatic sensor detection
- Individual CAPDAC management per channel
- Interactive display mode switching

**Use Case**: Complex applications requiring detailed analysis, data logging, or monitoring multiple sensors simultaneously.

### Example5-Simple-MultiChannel
**Basic multi-channel measurement demo**

Features:
- Sequential measurement of all 4 channels
- Simple tabular output format
- Basic error handling and status reporting
- Automatic CAPDAC adjustment
- Lightweight and easy to understand

**Use Case**: Simple applications that need to read multiple channels without complex analysis.

## Hardware Setup

### Basic Connections
```
Arduino   FDC1004 board
-------   -------------
5V     -> Vin
GND    -> GND
A4     -> SDA
A5     -> SCL
```

### Channel Configuration
Connect your capacitive sensors to the FDC1004 channels:
- **Channel 0**: Primary sensor (e.g., liquid level detection)
- **Channel 1**: Secondary sensor (e.g., proximity detection)
- **Channel 2**: Reference sensor (e.g., temperature compensation)
- **Channel 3**: Auxiliary sensor (e.g., touch detection)

## Usage Tips

### Sensor Types
The FDC1004 can work with various capacitive sensors:
- **Liquid level sensors**: Detect fluid levels in tanks
- **Proximity sensors**: Detect nearby objects
- **Touch sensors**: Detect human touch
- **Humidity sensors**: Measure environmental humidity
- **Custom capacitive sensors**: Any sensor that changes capacitance

### CAPDAC Management
The FDC1004 has a built-in CAPDAC (Capacitive Digital-to-Analog Converter) for each channel:
- **Range**: 0-31 (5-bit)
- **Resolution**: ~3.125 pF per step
- **Purpose**: Extends measurement range by compensating for baseline capacitance
- **Auto-adjustment**: Library automatically adjusts CAPDAC when readings are out of range

### Sample Rate Selection
Choose sample rate based on your application:
- **100 Hz**: Good for most applications, lower power consumption
- **200 Hz**: Balanced performance for multi-channel applications
- **400 Hz**: Fast response time, higher power consumption

### Multi-Channel Considerations
1. **Measurement Timing**: Allow small delays between channel measurements for stability
2. **Channel Isolation**: Ensure proper electrical isolation between sensors
3. **Common Mode Rejection**: Use differential measurements if available
4. **Calibration**: Consider calibrating each channel independently

## Code Examples

### Basic Multi-Channel Reading
```cpp
FDC1004 sensor(FDC1004_RATE_100HZ);

void loop() {
    for (uint8_t channel = 0; channel < 4; channel++) {
        fdc1004_capacitance_t measurement = sensor.getCapacitanceMeasurement((fdc1004_channel_t)channel);
        
        if (!isnan(measurement.capacitance_pf)) {
            Serial.print("CH");
            Serial.print(channel);
            Serial.print(": ");
            Serial.print(measurement.capacitance_pf, 2);
            Serial.println(" pF");
        }
        
        delay(10); // Small delay between channels
    }
}
```

### Advanced Error Handling
```cpp
fdc1004_capacitance_t measurement = sensor.getCapacitanceMeasurement(FDC1004_CHANNEL_0);

if (!isnan(measurement.capacitance_pf)) {
    // Valid measurement
    float capacitance = measurement.capacitance_pf;
    uint8_t capdac = measurement.capdac_used;
    
    if (measurement.capdac_out_of_range) {
        // CAPDAC was automatically adjusted
        Serial.println("CAPDAC adjusted for better range");
    }
} else {
    // Measurement failed
    Serial.println("Measurement error");
}
```

## Troubleshooting

### Common Issues
1. **No readings**: Check I2C connections and power supply
2. **Erratic readings**: Ensure proper grounding and shielding
3. **Limited range**: Check CAPDAC settings and sensor placement
4. **Slow response**: Increase sample rate or reduce measurement intervals

### Debug Steps
1. Use the sensor detection function to verify connected channels
2. Check CAPDAC adjustment messages for range issues
3. Monitor I2C communication for hardware problems
4. Verify sensor wiring and connections

## Application Examples

### Liquid Level Monitoring
```cpp
// Use multiple channels for tank level monitoring
// Channel 0: Main tank
// Channel 1: Reserve tank
// Channel 2: Reference (empty measurement)
// Channel 3: Overflow detection
```

### Multi-Touch Interface
```cpp
// Use channels for touch detection
// Channel 0-3: Touch pads or buttons
// Detect simultaneous touches across multiple channels
```

### Environmental Monitoring
```cpp
// Channel 0: Humidity sensor
// Channel 1: Proximity sensor
// Channel 2: Temperature compensation
// Channel 3: Spare/calibration
```

## Performance Optimization

### For High-Speed Applications
- Use 400 Hz sample rate
- Minimize delays between measurements
- Use interrupt-driven measurement triggering

### For Low-Power Applications
- Use 100 Hz sample rate
- Implement duty cycling
- Use sleep modes between measurements

### For High-Precision Applications
- Use statistical averaging
- Implement temperature compensation
- Calibrate each channel independently

## License

This example code is licensed under the MIT License. See the main library LICENSE file for details.
