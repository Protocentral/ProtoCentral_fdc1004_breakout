//////////////////////////////////////////////////////////////////////////////////////////
//
//    Multi-Channel Acquisition demo for the FDC1004 capacitance sensor breakout board
//
//    This example demonstrates simultaneous measurement of all 4 channels with:
//    - Simultaneous data acquisition from all channels
//    - Individual CAPDAC management per channel
//    - Data logging with timestamps
//    - Statistical analysis (min, max, average)
//    - Real-time monitoring with status indicators
//
//    Author: GitHub Copilot
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
//    Connect capacitive sensors to channels 0-3 as needed:
//    - Channel 0: Primary sensor (e.g., liquid level)
//    - Channel 1: Secondary sensor (e.g., proximity)
//    - Channel 2: Reference sensor (e.g., temperature compensation)
//    - Channel 3: Auxiliary sensor (e.g., touch detection)
//
//    This software is licensed under the MIT License(http://opensource.org/licenses/MIT).
//
/////////////////////////////////////////////////////////////////////////////////////////

#include <Wire.h>
#include <Protocentral_FDC1004.h>

// Create FDC1004 instance with 200Hz sample rate for faster multi-channel acquisition
FDC1004 capacitanceSensor(FDC1004_RATE_200HZ);

// Configuration parameters
const uint8_t NUM_CHANNELS = 4;
const uint16_t MEASUREMENT_INTERVAL_MS = 250; // 4 Hz update rate per channel
const uint16_t STATISTICS_INTERVAL_MS = 5000; // Statistics every 5 seconds
const uint16_t SERIAL_BAUD_RATE = 115200;

// Data structure for channel information
struct ChannelData
{
    float current_capacitance;
    float min_capacitance;
    float max_capacitance;
    float sum_capacitance;
    uint16_t measurement_count;
    uint8_t capdac_value;
    bool sensor_connected;
    bool capdac_adjusted;
    const char *sensor_name;
};

// Channel configuration
ChannelData channels[NUM_CHANNELS] = {
    {0.0, 999999.0, -999999.0, 0.0, 0, 0, false, false, "Primary"},
    {0.0, 999999.0, -999999.0, 0.0, 0, 0, false, false, "Secondary"},
    {0.0, 999999.0, -999999.0, 0.0, 0, 0, false, false, "Reference"},
    {0.0, 999999.0, -999999.0, 0.0, 0, 0, false, false, "Auxiliary"}};

// Timing variables
unsigned long lastMeasurementTime = 0;
unsigned long lastStatisticsTime = 0;
unsigned long startTime = 0;

// Display modes
enum DisplayMode
{
    DISPLAY_REALTIME,
    DISPLAY_STATISTICS,
    DISPLAY_RAW_DATA
};

DisplayMode currentDisplayMode = DISPLAY_REALTIME;
uint8_t displayModeCounter = 0;

void setup()
{
    Serial.begin(SERIAL_BAUD_RATE);
    Wire.begin();

    // Wait for Serial to initialize
    delay(1000);

    printHeader();

    // Initialize the sensor
    Serial.println("Initializing FDC1004 sensor...");
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

    // Verify device connection
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

    // Configure sensor settings
    Serial.printf("✓ Sample rate: %s\n", getSampleRateString(capacitanceSensor.getSampleRate()));

    // Detect connected sensors
    Serial.println("\nDetecting connected sensors...");
    detectConnectedSensors();

    // Initialize timing
    startTime = millis();
    lastMeasurementTime = startTime;
    lastStatisticsTime = startTime;

    Serial.println("\n" + String('=', 80));
    Serial.println("Multi-Channel Acquisition Started");
    Serial.println("Press any key to cycle through display modes");
    Serial.println(String('=', 80));

    printColumnHeaders();
}

void loop()
{
    unsigned long currentTime = millis();

    // Check for serial input to change display mode
    if (Serial.available() > 0)
    {
        Serial.read(); // Clear the input
        cycleDisplayMode();
    }

    // Take measurements at regular intervals
    if (currentTime - lastMeasurementTime >= MEASUREMENT_INTERVAL_MS)
    {
        performMultiChannelMeasurement();
        lastMeasurementTime = currentTime;
    }

    // Print statistics at regular intervals
    if (currentTime - lastStatisticsTime >= STATISTICS_INTERVAL_MS)
    {
        if (currentDisplayMode == DISPLAY_STATISTICS)
        {
            printStatistics();
        }
        lastStatisticsTime = currentTime;
    }
}

void printHeader()
{
    Serial.println("\n" + String('=', 80));
    Serial.println("FDC1004 Multi-Channel Capacitance Acquisition");
    Serial.println("Advanced Demo with Statistics and Real-time Monitoring");
    Serial.println(String('=', 80));
}

void detectConnectedSensors()
{
    for (uint8_t channel = 0; channel < NUM_CHANNELS; channel++)
    {
        // Take a quick measurement to see if sensor is connected
        fdc1004_capacitance_t measurement = capacitanceSensor.getCapacitanceMeasurement((fdc1004_channel_t)channel);

        if (!isnan(measurement.capacitance_pf))
        {
            // Consider sensor connected if we get a reasonable reading
            if (measurement.capacitance_pf > -1000.0 && measurement.capacitance_pf < 1000.0)
            {
                channels[channel].sensor_connected = true;
                channels[channel].capdac_value = measurement.capdac_used;
                Serial.printf("✓ Channel %d (%s): Connected (%.2f pF, CAPDAC: %d)\n",
                              channel, channels[channel].sensor_name,
                              measurement.capacitance_pf, measurement.capdac_used);
            }
            else
            {
                Serial.printf("? Channel %d (%s): Unusual reading (%.2f pF)\n",
                              channel, channels[channel].sensor_name, measurement.capacitance_pf);
            }
        }
        else
        {
            Serial.printf("✗ Channel %d (%s): No response\n", channel, channels[channel].sensor_name);
        }

        delay(100); // Small delay between channel checks
    }
}

void performMultiChannelMeasurement()
{
    unsigned long measurementTime = millis();

    // Measure all connected channels
    for (uint8_t channel = 0; channel < NUM_CHANNELS; channel++)
    {
        if (channels[channel].sensor_connected)
        {
            measureChannel(channel);
        }
    }

    // Display results based on current mode
    switch (currentDisplayMode)
    {
    case DISPLAY_REALTIME:
        printRealtimeData(measurementTime);
        break;
    case DISPLAY_RAW_DATA:
        printRawData(measurementTime);
        break;
    case DISPLAY_STATISTICS:
        // Statistics are printed separately on a timer
        break;
    }
}

void measureChannel(uint8_t channel)
{
    fdc1004_capacitance_t measurement = capacitanceSensor.getCapacitanceMeasurement((fdc1004_channel_t)channel);

    if (!isnan(measurement.capacitance_pf))
    {
        // Update current measurement
        channels[channel].current_capacitance = measurement.capacitance_pf;
        channels[channel].capdac_value = measurement.capdac_used;
        channels[channel].capdac_adjusted = measurement.capdac_out_of_range;

        // Update statistics
        updateStatistics(channel, measurement.capacitance_pf);
    }
}

void updateStatistics(uint8_t channel, float capacitance)
{
    ChannelData *ch = &channels[channel];

    // Update min/max
    if (capacitance < ch->min_capacitance)
    {
        ch->min_capacitance = capacitance;
    }
    if (capacitance > ch->max_capacitance)
    {
        ch->max_capacitance = capacitance;
    }

    // Update running average
    ch->sum_capacitance += capacitance;
    ch->measurement_count++;
}

void printColumnHeaders()
{
    switch (currentDisplayMode)
    {
    case DISPLAY_REALTIME:
        Serial.println("\nTime(s)\tCH0(pF)\tCH1(pF)\tCH2(pF)\tCH3(pF)\tStatus");
        Serial.println(String('-', 70));
        break;
    case DISPLAY_RAW_DATA:
        Serial.println("\nTime(s)\tCH\tCapacitance(pF)\tCAPDAC\tStatus");
        Serial.println(String('-', 50));
        break;
    case DISPLAY_STATISTICS:
        Serial.println("\nStatistics Display Mode - Updates every 5 seconds");
        Serial.println(String('-', 50));
        break;
    }
}

void printRealtimeData(unsigned long timestamp)
{
    float timeSeconds = (timestamp - startTime) / 1000.0;

    Serial.printf("%6.1f\t", timeSeconds);

    for (uint8_t channel = 0; channel < NUM_CHANNELS; channel++)
    {
        if (channels[channel].sensor_connected)
        {
            Serial.printf("%7.2f\t", channels[channel].current_capacitance);
        }
        else
        {
            Serial.print("   --  \t");
        }
    }

    // Print status indicators
    Serial.print("[");
    for (uint8_t channel = 0; channel < NUM_CHANNELS; channel++)
    {
        if (channels[channel].sensor_connected)
        {
            if (channels[channel].capdac_adjusted)
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
            Serial.print("-"); // Disconnected
        }
    }
    Serial.println("]");
}

void printRawData(unsigned long timestamp)
{
    float timeSeconds = (timestamp - startTime) / 1000.0;

    for (uint8_t channel = 0; channel < NUM_CHANNELS; channel++)
    {
        if (channels[channel].sensor_connected)
        {
            Serial.printf("%6.1f\t%d\t%10.4f\t%6d\t",
                          timeSeconds, channel,
                          channels[channel].current_capacitance,
                          channels[channel].capdac_value);

            if (channels[channel].capdac_adjusted)
            {
                Serial.println("CAPDAC_ADJ");
            }
            else
            {
                Serial.println("OK");
            }
        }
    }
}

void printStatistics()
{
    Serial.println("\n" + String('=', 60));
    Serial.println("CHANNEL STATISTICS");
    Serial.println(String('=', 60));

    for (uint8_t channel = 0; channel < NUM_CHANNELS; channel++)
    {
        if (channels[channel].sensor_connected && channels[channel].measurement_count > 0)
        {
            float average = channels[channel].sum_capacitance / channels[channel].measurement_count;
            float range = channels[channel].max_capacitance - channels[channel].min_capacitance;

            Serial.printf("Channel %d (%s):\n", channel, channels[channel].sensor_name);
            Serial.printf("  Current:    %8.4f pF\n", channels[channel].current_capacitance);
            Serial.printf("  Average:    %8.4f pF\n", average);
            Serial.printf("  Range:      %8.4f pF (%.4f to %.4f)\n",
                          range, channels[channel].min_capacitance, channels[channel].max_capacitance);
            Serial.printf("  CAPDAC:     %8d\n", channels[channel].capdac_value);
            Serial.printf("  Samples:    %8d\n", channels[channel].measurement_count);
            Serial.println();
        }
    }

    Serial.println(String('=', 60));
}

void cycleDisplayMode()
{
    displayModeCounter = (displayModeCounter + 1) % 3;

    switch (displayModeCounter)
    {
    case 0:
        currentDisplayMode = DISPLAY_REALTIME;
        Serial.println("\n>> Switched to REALTIME display mode");
        break;
    case 1:
        currentDisplayMode = DISPLAY_STATISTICS;
        Serial.println("\n>> Switched to STATISTICS display mode");
        break;
    case 2:
        currentDisplayMode = DISPLAY_RAW_DATA;
        Serial.println("\n>> Switched to RAW DATA display mode");
        break;
    }

    printColumnHeaders();
}

const char *getSampleRateString(fdc1004_sample_rate_t rate)
{
    switch (rate)
    {
    case FDC1004_RATE_100HZ:
        return "100 Hz";
    case FDC1004_RATE_200HZ:
        return "200 Hz";
    case FDC1004_RATE_400HZ:
        return "400 Hz";
    default:
        return "Unknown";
    }
}

// Optional: Reset statistics function
void resetStatistics()
{
    for (uint8_t channel = 0; channel < NUM_CHANNELS; channel++)
    {
        channels[channel].min_capacitance = 999999.0;
        channels[channel].max_capacitance = -999999.0;
        channels[channel].sum_capacitance = 0.0;
        channels[channel].measurement_count = 0;
    }
    Serial.println("Statistics reset.");
}
