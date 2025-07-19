//////////////////////////////////////////////////////////////////////////////////////////
//
//    Arduino library for the FDC1004 capacitance sensor breakout board
//
//    Author: Ashwin Whitchurch
//    Copyright (c) 2018 ProtoCentral
//
//    Based on original code written by Benjamin Shaya
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
/////////////////////////////////////////////////////////////////////////////////////////

#include <Protocentral_FDC1004.h>

// =============================================================================
// Private Constants
// =============================================================================

static const uint8_t MEASUREMENT_CONFIG_REGISTERS[] = {
    FDC1004_REG_CONF_MEAS1, FDC1004_REG_CONF_MEAS2,
    FDC1004_REG_CONF_MEAS3, FDC1004_REG_CONF_MEAS4};

static const uint8_t MEASUREMENT_MSB_REGISTERS[] = {
    FDC1004_REG_MEAS1_MSB, FDC1004_REG_MEAS2_MSB,
    FDC1004_REG_MEAS3_MSB, FDC1004_REG_MEAS4_MSB};

static const uint8_t MEASUREMENT_LSB_REGISTERS[] = {
    FDC1004_REG_MEAS1_LSB, FDC1004_REG_MEAS2_LSB,
    FDC1004_REG_MEAS3_LSB, FDC1004_REG_MEAS4_LSB};

static const uint8_t SAMPLE_DELAYS_MS[] = {11, 11, 6, 3}; // Delays for 100Hz, 200Hz, 400Hz

// =============================================================================
// Constructors and Initialization
// =============================================================================

FDC1004::FDC1004(fdc1004_sample_rate_t rate, uint8_t address, TwoWire* wire)
    : _i2c_address(address), _sample_rate(rate), _device_initialized(false), _wire(wire)
{
    // Initialize CAPDAC values to zero
    for (int i = 0; i < 4; i++)
    {
        _capdac_values[i] = 0;
    }
}

FDC1004::FDC1004(TwoWire* wire, fdc1004_sample_rate_t rate, uint8_t address)
    : _i2c_address(address), _sample_rate(rate), _device_initialized(false), _wire(wire)
{
    // Initialize CAPDAC values to zero
    for (int i = 0; i < 4; i++)
    {
        _capdac_values[i] = 0;
    }
}

FDC1004::FDC1004(uint16_t rate)
    : _i2c_address(FDC1004_I2C_ADDRESS), _device_initialized(false), _wire(&Wire)
{
    // Legacy constructor - convert rate to new enum
    switch (rate)
    {
    case FDC1004_SAMPLE_RATE_100HZ:
        _sample_rate = FDC1004_RATE_100HZ;
        break;
    case FDC1004_SAMPLE_RATE_200HZ:
        _sample_rate = FDC1004_RATE_200HZ;
        break;
    case FDC1004_SAMPLE_RATE_400HZ:
        _sample_rate = FDC1004_RATE_400HZ;
        break;
    default:
        _sample_rate = FDC1004_RATE_100HZ;
        break;
    }

    // Initialize CAPDAC values to zero
    for (int i = 0; i < 4; i++)
    {
        _capdac_values[i] = 0;
    }
}

bool FDC1004::begin()
{
    // Initialize the TwoWire interface
    _wire->begin();
    
    // Check if device is responding
    if (!isConnected())
    {
        return false;
    }

    _device_initialized = true;
    return true;
}

bool FDC1004::isConnected()
{
    uint16_t device_id;
    fdc1004_error_t result = readRegister16(FDC1004_REG_DEVICE_ID, &device_id);
    return (result == FDC1004_SUCCESS);
}

// =============================================================================
// High-Level Measurement Functions
// =============================================================================

fdc1004_capacitance_t FDC1004::getCapacitanceMeasurement(fdc1004_channel_t channel)
{
    fdc1004_capacitance_t result;
    result.capacitance_pf = NAN;
    result.capdac_out_of_range = false;
    result.capdac_used = 0;

    if (!_device_initialized || !isValidChannel(channel))
    {
        return result;
    }

    fdc1004_raw_measurement_t raw_measurement;
    fdc1004_error_t error = getRawCapacitance(channel, &raw_measurement);

    if (error == FDC1004_SUCCESS)
    {
        result.capacitance_pf = convertToPicofarads(raw_measurement.value, raw_measurement.capdac);
        result.capdac_used = raw_measurement.capdac;

        // Check if CAPDAC adjustment is needed
        if (raw_measurement.value > FDC1004_UPPER_BOUND ||
            raw_measurement.value < FDC1004_LOWER_BOUND)
        {
            result.capdac_out_of_range = true;
            autoAdjustCapdac(channel, raw_measurement.value);
        }
    }

    return result;
}

float FDC1004::getCapacitancePicofarads(fdc1004_channel_t channel)
{
    fdc1004_capacitance_t measurement = getCapacitanceMeasurement(channel);
    return measurement.capacitance_pf;
}

int32_t FDC1004::getCapacitance(uint8_t channel)
{
    // Legacy function - returns femtofarads
    if (!isValidChannel(channel))
    {
        return 0x80000000; // Error value
    }

    fdc1004_raw_measurement_t value;
    fdc1004_error_t result = getRawCapacitance((fdc1004_channel_t)channel, &value);
    if (result != FDC1004_SUCCESS)
    {
        return 0x80000000; // Error value
    }

    // Convert to femtofarads (legacy format)
    int32_t capacitance = ((int32_t)FDC1004_ATTOFARADS_UPPER_WORD) * ((int32_t)value.value);
    capacitance /= 1000; // Convert attofarads to femtofarads
    capacitance += ((int32_t)FDC1004_FEMTOFARADS_CAPDAC) * ((int32_t)value.capdac);
    return capacitance;
}

// =============================================================================
// Configuration and Control
// =============================================================================

fdc1004_error_t FDC1004::setSampleRate(fdc1004_sample_rate_t rate)
{
    if (!isValidSampleRate(rate))
    {
        return FDC1004_ERROR_INVALID_PARAMETER;
    }

    _sample_rate = rate;
    return FDC1004_SUCCESS;
}

fdc1004_sample_rate_t FDC1004::getSampleRate() const
{
    return _sample_rate;
}

fdc1004_error_t FDC1004::setCapdac(fdc1004_channel_t channel, uint8_t capdac)
{
    if (!isValidChannel(channel) || !isValidCapdac(capdac))
    {
        return FDC1004_ERROR_INVALID_PARAMETER;
    }

    _capdac_values[channel] = capdac;
    return FDC1004_SUCCESS;
}

uint8_t FDC1004::getCapdac(fdc1004_channel_t channel) const
{
    if (!isValidChannel(channel))
    {
        return 0;
    }
    return _capdac_values[channel];
}

// =============================================================================
// Low-Level Hardware Interface (New Implementation)
// =============================================================================

fdc1004_error_t FDC1004::configureMeasurementSingle(fdc1004_measurement_t measurement,
                                                    fdc1004_channel_t channel,
                                                    uint8_t capdac)
{
    if (!isValidMeasurement(measurement) || !isValidChannel(channel) || !isValidCapdac(capdac))
    {
        return FDC1004_ERROR_INVALID_PARAMETER;
    }

    // Build 16-bit configuration
    uint16_t configuration_data = 0;
    configuration_data |= ((uint16_t)channel) << FDC1004_CONF_MEAS_CHA_SHIFT;            // CHA
    configuration_data |= FDC1004_CONF_MEAS_CHB_DISABLED << FDC1004_CONF_MEAS_CHB_SHIFT; // CHB disable
    configuration_data |= ((uint16_t)capdac) << FDC1004_CONF_MEAS_CAPDAC_SHIFT;          // CAPDAC value

    return writeRegister16(MEASUREMENT_CONFIG_REGISTERS[measurement], configuration_data);
}

fdc1004_error_t FDC1004::triggerSingleMeasurement(fdc1004_measurement_t measurement,
                                                  fdc1004_sample_rate_t rate)
{
    if (!isValidMeasurement(measurement) || !isValidSampleRate(rate))
    {
        return FDC1004_ERROR_INVALID_PARAMETER;
    }

    uint16_t trigger_data = 0;
    trigger_data |= ((uint16_t)rate) << FDC1004_FDC_CONF_RATE_SHIFT; // Sample rate
    trigger_data |= 0 << FDC1004_FDC_CONF_REPEAT_SHIFT;              // Repeat disabled
    trigger_data |= (1 << (7 - measurement));                        // Enable measurement

    return writeRegister16(FDC1004_REG_FDC_CONF, trigger_data);
}

fdc1004_error_t FDC1004::readMeasurement(fdc1004_measurement_t measurement, uint16_t *value)
{
    if (!isValidMeasurement(measurement) || value == nullptr)
    {
        return FDC1004_ERROR_INVALID_PARAMETER;
    }

    // Check if measurement is complete
    uint16_t fdc_register;
    fdc1004_error_t result = readRegister16(FDC1004_REG_FDC_CONF, &fdc_register);
    if (result != FDC1004_SUCCESS)
    {
        return result;
    }

    if (!(fdc_register & (1 << (3 - measurement))))
    {
        return FDC1004_ERROR_MEASUREMENT_NOT_READY;
    }

    // Read the measurement values
    uint16_t msb, lsb;
    result = readRegister16(MEASUREMENT_MSB_REGISTERS[measurement], &msb);
    if (result != FDC1004_SUCCESS)
    {
        return result;
    }

    result = readRegister16(MEASUREMENT_LSB_REGISTERS[measurement], &lsb);
    if (result != FDC1004_SUCCESS)
    {
        return result;
    }

    value[0] = msb;
    value[1] = lsb;
    return FDC1004_SUCCESS;
}

fdc1004_error_t FDC1004::measureChannel(fdc1004_channel_t channel, uint8_t capdac, uint16_t *value)
{
    if (!isValidChannel(channel) || !isValidCapdac(capdac) || value == nullptr)
    {
        return FDC1004_ERROR_INVALID_PARAMETER;
    }

    // Use measurement slot equal to channel number
    fdc1004_measurement_t measurement = (fdc1004_measurement_t)channel;

    fdc1004_error_t result = configureMeasurementSingle(measurement, channel, capdac);
    if (result != FDC1004_SUCCESS)
    {
        return result;
    }

    result = triggerSingleMeasurement(measurement, _sample_rate);
    if (result != FDC1004_SUCCESS)
    {
        return result;
    }

    delay(getMeasurementDelay());
    return readMeasurement(measurement, value);
}

fdc1004_error_t FDC1004::getRawCapacitance(fdc1004_channel_t channel, fdc1004_raw_measurement_t *value)
{
    if (!isValidChannel(channel) || value == nullptr)
    {
        return FDC1004_ERROR_INVALID_PARAMETER;
    }

    uint8_t capdac = _capdac_values[channel];
    uint16_t raw_measurement[2];

    fdc1004_error_t result = measureChannel(channel, capdac, raw_measurement);
    if (result != FDC1004_SUCCESS)
    {
        return result;
    }

    value->value = (int16_t)raw_measurement[0];
    value->capdac = capdac;

    return FDC1004_SUCCESS;
}

// =============================================================================
// Legacy Interface (for backward compatibility)
// =============================================================================

uint8_t FDC1004::configureMeasurementSingle(uint8_t measurement, uint8_t channel, uint8_t capdac)
{
    fdc1004_error_t result = configureMeasurementSingle((fdc1004_measurement_t)measurement,
                                                        (fdc1004_channel_t)channel,
                                                        capdac);
    return (result == FDC1004_SUCCESS) ? 0 : 1;
}

uint8_t FDC1004::triggerSingleMeasurement(uint8_t measurement, uint8_t rate)
{
    fdc1004_error_t result = triggerSingleMeasurement((fdc1004_measurement_t)measurement,
                                                      (fdc1004_sample_rate_t)rate);
    return (result == FDC1004_SUCCESS) ? 0 : 1;
}

uint8_t FDC1004::readMeasurement(uint8_t measurement, uint16_t *value)
{
    fdc1004_error_t result = readMeasurement((fdc1004_measurement_t)measurement, value);
    switch (result)
    {
    case FDC1004_SUCCESS:
        return 0;
    case FDC1004_ERROR_MEASUREMENT_NOT_READY:
        return 2;
    default:
        return 1;
    }
}

uint8_t FDC1004::measureChannel(uint8_t channel, uint8_t capdac, uint16_t *value)
{
    fdc1004_error_t result = measureChannel((fdc1004_channel_t)channel, capdac, value);
    return (result == FDC1004_SUCCESS) ? 0 : 1;
}

uint8_t FDC1004::getRawCapacitance(uint8_t channel, fdc1004_raw_measurement_t *value)
{
    fdc1004_error_t result = getRawCapacitance((fdc1004_channel_t)channel, value);
    return (result == FDC1004_SUCCESS) ? 0 : 1;
}

uint16_t FDC1004::read16(uint8_t reg)
{
    uint16_t value;
    readRegister16(reg, &value);
    return value;
}

// =============================================================================
// Private Methods - I2C Communication
// =============================================================================

fdc1004_error_t FDC1004::writeRegister16(uint8_t reg, uint16_t data)
{
    _wire->beginTransmission(_i2c_address);
    _wire->write(reg);
    _wire->write((uint8_t)(data >> 8)); // MSB first
    _wire->write((uint8_t)(data));      // LSB second

    uint8_t error = _wire->endTransmission();
    return (error == 0) ? FDC1004_SUCCESS : FDC1004_ERROR_I2C_COMMUNICATION;
}

fdc1004_error_t FDC1004::readRegister16(uint8_t reg, uint16_t *data)
{
    if (data == nullptr)
    {
        return FDC1004_ERROR_INVALID_PARAMETER;
    }

    _wire->beginTransmission(_i2c_address);
    _wire->write(reg);
    uint8_t error = _wire->endTransmission();

    if (error != 0)
    {
        return FDC1004_ERROR_I2C_COMMUNICATION;
    }

    uint8_t bytes_received = _wire->requestFrom(_i2c_address, (uint8_t)2);
    if (bytes_received != 2)
    {
        return FDC1004_ERROR_I2C_COMMUNICATION;
    }

    *data = ((uint16_t)_wire->read() << 8) | _wire->read();
    return FDC1004_SUCCESS;
}

void FDC1004::write16(uint8_t reg, uint16_t data)
{
    // Legacy function - ignore error handling
    writeRegister16(reg, data);
}

// =============================================================================
// Private Methods - Utility Functions
// =============================================================================

uint8_t FDC1004::getMeasurementDelay() const
{
    switch (_sample_rate)
    {
    case FDC1004_RATE_100HZ:
        return SAMPLE_DELAYS_MS[0];
    case FDC1004_RATE_200HZ:
        return SAMPLE_DELAYS_MS[1];
    case FDC1004_RATE_400HZ:
        return SAMPLE_DELAYS_MS[2];
    default:
        return SAMPLE_DELAYS_MS[0]; // Default to 100Hz delay
    }
}

float FDC1004::convertToPicofarads(int16_t raw_value, uint8_t capdac) const
{
    // Convert from raw measurement to picofarads
    float capacitance_af = (float)FDC1004_ATTOFARADS_UPPER_WORD * (float)raw_value;  // attofarads
    float capacitance_pf = capacitance_af / 1000000.0f;                              // Convert to picofarads
    capacitance_pf += ((float)FDC1004_FEMTOFARADS_CAPDAC * (float)capdac) / 1000.0f; // Add CAPDAC offset

    return capacitance_pf;
}

bool FDC1004::isValidChannel(uint8_t channel) const
{
    return (channel <= FDC1004_CHANNEL_MAX);
}

bool FDC1004::isValidMeasurement(uint8_t measurement) const
{
    return (measurement <= FDC1004_MEASUREMENT_MAX);
}

bool FDC1004::isValidCapdac(uint8_t capdac) const
{
    return (capdac <= FDC1004_CAPDAC_MAX);
}

bool FDC1004::isValidSampleRate(uint8_t rate) const
{
    return (rate == FDC1004_SAMPLE_RATE_100HZ ||
            rate == FDC1004_SAMPLE_RATE_200HZ ||
            rate == FDC1004_SAMPLE_RATE_400HZ);
}

bool FDC1004::autoAdjustCapdac(fdc1004_channel_t channel, int16_t raw_value)
{
    uint8_t current_capdac = _capdac_values[channel];

    if (raw_value > FDC1004_UPPER_BOUND && current_capdac < FDC1004_CAPDAC_MAX)
    {
        _capdac_values[channel] = current_capdac + 1;
        return true;
    }
    else if (raw_value < FDC1004_LOWER_BOUND && current_capdac > 0)
    {
        _capdac_values[channel] = current_capdac - 1;
        return true;
    }

    return false;
}
