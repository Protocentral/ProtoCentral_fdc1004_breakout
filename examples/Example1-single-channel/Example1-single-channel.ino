//////////////////////////////////////////////////////////////////////////////////////////
//
//    Basic Single channel demo for the FDC1004 capacitance sensor breakout board
//
//    Author: Ashwin Whitchurch 
//    Copyright (c) 2018-2025 Protocentral Electronics
//
//    This example measures capacitance on CHANNEL0 with
//    automatic CAPDAC adjustment.
//
//    Arduino connections:
//
//    Arduino   FDC1004 board
//    -------   -------------
//    5V     -> Vin
//    GND    -> GND
//    A4     -> SDA
//    A5     -> SCL
//
//    For boards with multiple I2C interfaces (ESP32, Arduino Mega, etc.):
//    You can use Wire1 or other I2C interfaces by modifying the constructor.
//    See the constructor examples below.
//
//    This software is licensed under the MIT License(http://opensource.org/licenses/MIT).
//
//   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
//   NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//   For information on how to use, visit https://github.com/protocentral/ProtoCentral_fdc1004_breakout
//
/////////////////////////////////////////////////////////////////////////////////////////

#include <Wire.h>
#include <Protocentral_FDC1004.h>

// Constructor Options - Choose one of the following:

// Option 1: Default Wire interface (backwards compatible)
FDC1004 capacitanceSensor(FDC1004_RATE_100HZ);

// Option 2: Specify custom I2C interface (for boards with multiple I2C ports)
// FDC1004 capacitanceSensor(&Wire1, FDC1004_RATE_100HZ);

// Option 3: Alternative syntax with TwoWire as third parameter
// FDC1004 capacitanceSensor(FDC1004_RATE_100HZ, FDC1004_I2C_ADDRESS, &Wire1);

// Option 4: Custom I2C address with default Wire
// FDC1004 capacitanceSensor(FDC1004_RATE_100HZ, 0x51); // Different address

void setup() {
    Serial.begin(115200);
    
    // Initialize I2C interface
    // Note: If using Wire1 or custom I2C interface, initialize that instead
    // Example for ESP32 with custom pins: Wire1.begin(21, 22);
    Wire.begin();
    
    Serial.println("FDC1004 Capacitance Sensor");
    Serial.println("==========================");
    Serial.println("Using default Wire interface");
    Serial.println();
    
    // Initialize the sensor with error checking
    if (capacitanceSensor.begin()) {
        Serial.println("✓ FDC1004 sensor initialized successfully");
    } else {
        Serial.println("✗ Failed to initialize FDC1004 sensor");
        Serial.println("Check wiring and restart Arduino");
        while (1) {
            delay(1000); 
        }
    }
    
    // Verify device connection
    if (capacitanceSensor.isConnected()) {
        Serial.println("✓ FDC1004 device is responding");
    } else {
        Serial.println("✗ FDC1004 device not responding");
        Serial.println("Check I2C connections");
    }
    
    Serial.println();
    Serial.println("Starting continuous measurements on Channel 0...");
    Serial.println("Time(ms)\tCapacitance(pF)\tCAPDAC\tStatus");
    Serial.println("----------------------------------------------");
}

void loop() {
    static unsigned long lastMeasurement = 0;
    
    // Take measurement every 250ms
    if (millis() - lastMeasurement >= 250) {
        
        fdc1004_capacitance_t measurement = capacitanceSensor.getCapacitanceMeasurement(FDC1004_CHANNEL_0);
        
        // Print timestamp
        Serial.print(millis());
        Serial.print("\t\t");
        
        // Check if measurement was successful
        if (!isnan(measurement.capacitance_pf)) {
            // Print capacitance value with 4 decimal places
            Serial.print(measurement.capacitance_pf, 4);
            Serial.print("\t\t");
            
            // Print CAPDAC value being used
            Serial.print(measurement.capdac_used);
            Serial.print("\t");
            
            // Print status information
            if (measurement.capdac_out_of_range) {
                Serial.println("CAPDAC ADJUST");
            } else {
                Serial.println("OK");
            }
            
        } else {
            // Handle measurement error
            Serial.println("ERROR\t\t-\tMEASUREMENT FAILED");
        }
        
        lastMeasurement = millis();
    }
}
