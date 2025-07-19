//////////////////////////////////////////////////////////////////////////////////////////
//
//    Simple Multi-Channel demo for the FDC1004 capacitance sensor breakout board
//
//    This example demonstrates basic multi-channel measurement with:
//    - Sequential measurement of all 4 channels
//    - Automatic CAPDAC adjustment
//    - Simple tabular output format
//    - Error handling and status reporting
//
//    Author: Ashwin Whitchurch
//    Copyright (c) 2018-2025 Protocentral Electronics
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

// Configuration
const uint8_t NUM_CHANNELS = 4;
const uint16_t MEASUREMENT_DELAY_MS = 500; // Delay between complete channel scans

void setup()
{
    Serial.begin(115200);
    Wire.begin();

    Serial.println("FDC1004 Multi-Channel Capacitance Measurement");
    Serial.println("============================================");

    // Initialize the sensor
    if (capacitanceSensor.begin())
    {
        Serial.println("✓ FDC1004 sensor initialized successfully");
    }
    else
    {
        Serial.println("✗ Failed to initialize FDC1004 sensor");
        Serial.println("Check wiring and restart Arduino");
        while (1)
            delay(1000);
    }

    // Verify connection
    if (capacitanceSensor.isConnected())
    {
        Serial.println("✓ FDC1004 device is responding");
    }
    else
    {
        Serial.println("✗ FDC1004 device not responding");
        while (1)
            delay(1000);
    }

    Serial.println();
    Serial.println("Starting multi-channel measurements...");
    Serial.println("Time(s)\tCH0(pF)\tCH1(pF)\tCH2(pF)\tCH3(pF)\tStatus");
    Serial.println("----------------------------------------------------------");
}

void loop()
{
    static unsigned long lastMeasurement = 0;
    static unsigned long startTime = millis();

    if (millis() - lastMeasurement >= MEASUREMENT_DELAY_MS)
    {
        performMultiChannelScan();
        lastMeasurement = millis();
    }
}

void performMultiChannelScan()
{
    float channelValues[NUM_CHANNELS];
    bool channelValid[NUM_CHANNELS];
    uint8_t capdacValues[NUM_CHANNELS];
    bool capdacAdjusted[NUM_CHANNELS];

    // Measure all channels
    for (uint8_t channel = 0; channel < NUM_CHANNELS; channel++)
    {
        fdc1004_capacitance_t measurement = capacitanceSensor.getCapacitanceMeasurement((fdc1004_channel_t)channel);

        if (!isnan(measurement.capacitance_pf))
        {
            channelValues[channel] = measurement.capacitance_pf;
            channelValid[channel] = true;
            capdacValues[channel] = measurement.capdac_used;
            capdacAdjusted[channel] = measurement.capdac_out_of_range;
        }
        else
        {
            channelValues[channel] = 0.0;
            channelValid[channel] = false;
            capdacValues[channel] = 0;
            capdacAdjusted[channel] = false;
        }

        // Small delay between channels for stability
        delay(10);
    }

    // Print results
    printMeasurementResults(channelValues, channelValid, capdacValues, capdacAdjusted);
}

void printMeasurementResults(float *values, bool *valid, uint8_t *capdac, bool *adjusted)
{
    static unsigned long startTime = millis();
    float timeSeconds = (millis() - startTime) / 1000.0;

    // Print timestamp
    Serial.print(timeSeconds, 1);
    Serial.print("\t");

    // Print channel values
    for (uint8_t channel = 0; channel < NUM_CHANNELS; channel++)
    {
        if (valid[channel])
        {
            Serial.print(values[channel], 2);
        }
        else
        {
            Serial.print("ERROR");
        }
        Serial.print("\t");
    }

    // Print status information
    Serial.print("[");
    for (uint8_t channel = 0; channel < NUM_CHANNELS; channel++)
    {
        if (valid[channel])
        {
            if (adjusted[channel])
            {
                Serial.print("A"); // CAPDAC adjusted
            }
            else
            {
                Serial.print("O"); // OK
            }
        }
        else
        {
            Serial.print("E"); // Error
        }
    }
    Serial.print("]");

    // Print CAPDAC values for channels that were adjusted
    bool anyAdjusted = false;
    for (uint8_t channel = 0; channel < NUM_CHANNELS; channel++)
    {
        if (valid[channel] && adjusted[channel])
        {
            if (!anyAdjusted)
            {
                Serial.print(" CAPDAC:");
                anyAdjusted = true;
            }
            Serial.print(" CH");
            Serial.print(channel);
            Serial.print("=");
            Serial.print(capdac[channel]);
        }
    }

    Serial.println();
}

void printChannelDetails()
{
    Serial.println("\nDetailed Channel Information:");
    Serial.println("-----------------------------");

    for (uint8_t channel = 0; channel < NUM_CHANNELS; channel++)
    {
        Serial.print("Channel ");
        Serial.print(channel);
        Serial.print(": ");

        fdc1004_capacitance_t measurement = capacitanceSensor.getCapacitanceMeasurement((fdc1004_channel_t)channel);

        if (!isnan(measurement.capacitance_pf))
        {
            Serial.print("Capacitance = ");
            Serial.print(measurement.capacitance_pf, 4);
            Serial.print(" pF, CAPDAC = ");
            Serial.print(measurement.capdac_used);

            if (measurement.capdac_out_of_range)
            {
                Serial.println(" (CAPDAC adjusted)");
            }
            else
            {
                Serial.println(" (OK)");
            }
        }
        else
        {
            Serial.println("Measurement failed");
        }

        delay(50);
    }
    Serial.println();
}
