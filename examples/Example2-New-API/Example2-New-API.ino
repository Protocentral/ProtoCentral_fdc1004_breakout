//////////////////////////////////////////////////////////////////////////////////////////
//
//    New API demo for the FDC1004 capacitance sensor breakout board
//
//    This example demonstrates the new, improved API with better error handling,
//    automatic CAPDAC adjustment, and cleaner interface.
//
//    Author: Refactored by GitHub Copilot
//    Copyright (c) 2018 ProtoCentral
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
//    This software is licensed under the MIT License(http://opensource.org/licenses/MIT).
//
/////////////////////////////////////////////////////////////////////////////////////////

#include <Wire.h>
#include <Protocentral_FDC1004.h>

// Create FDC1004 instance with 100Hz sample rate
FDC1004 capacitanceSensor(FDC1004_RATE_100HZ);

void setup() {
    Serial.begin(115200);
    Wire.begin();
    
    Serial.println("FDC1004 New API Demo");
    Serial.println("====================");
    
    // Initialize the sensor
    if (capacitanceSensor.begin()) {
        Serial.println("✓ FDC1004 sensor initialized successfully");
    } else {
        Serial.println("✗ Failed to initialize FDC1004 sensor");
        while (1) delay(1000); // Stop execution
    }
    
    // Check if device is connected
    if (capacitanceSensor.isConnected()) {
        Serial.println("✓ FDC1004 device is responding");
    } else {
        Serial.println("✗ FDC1004 device not responding");
    }
    
    Serial.println();
    Serial.println("Starting measurements...");
    Serial.println("Time(ms)\tChannel\tCapacitance(pF)\tCAPDAC\tStatus");
    Serial.println("------------------------------------------------------");
}

void loop() {
    static unsigned long lastMeasurement = 0;
    static uint8_t currentChannel = 0;
    
    // Take measurement every 500ms, cycling through channels
    if (millis() - lastMeasurement >= 500) {
        // Get detailed measurement with all information
        fdc1004_capacitance_t measurement = capacitanceSensor.getCapacitanceMeasurement((fdc1004_channel_t)currentChannel);
        
        // Print timestamp
        Serial.print(millis());
        Serial.print("\t\t");
        
        // Print channel
        Serial.print(currentChannel);
        Serial.print("\t");
        
        if (!isnan(measurement.capacitance_pf)) {
            // Print capacitance value
            Serial.print(measurement.capacitance_pf, 4);
            Serial.print("\t\t");
            
            // Print CAPDAC value used
            Serial.print(measurement.capdac_used);
            Serial.print("\t");
            
            // Print status
            if (measurement.capdac_out_of_range) {
                Serial.println("CAPDAC ADJ");
            } else {
                Serial.println("OK");
            }
        } else {
            Serial.println("ERROR\t\t-\tFAILED");
        }
        
        // Move to next channel (0-3)
        currentChannel = (currentChannel + 1) % 4;
        lastMeasurement = millis();
    }
    
    // You can also use the simple interface for basic measurements:
    // float capacitance = capacitanceSensor.getCapacitancePicofarads(FDC1004_CHANNEL_0);
    
    // Or the legacy interface for backward compatibility:
    // int32_t capacitance_ff = capacitanceSensor.getCapacitance(0); // Returns femtofarads
}

void printSensorInfo() {
    Serial.println("\nSensor Configuration:");
    Serial.print("Sample Rate: ");
    switch (capacitanceSensor.getSampleRate()) {
        case FDC1004_RATE_100HZ:
            Serial.println("100 Hz");
            break;
        case FDC1004_RATE_200HZ:
            Serial.println("200 Hz");
            break;
        case FDC1004_RATE_400HZ:
            Serial.println("400 Hz");
            break;
    }
    
    Serial.println("CAPDAC Values:");
    for (int i = 0; i < 4; i++) {
        Serial.print("  Channel ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(capacitanceSensor.getCapdac((fdc1004_channel_t)i));
    }
}
