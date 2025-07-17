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

#ifndef _FDC1004
#define _FDC1004

#include "Arduino.h"
#include "Wire.h"

//Constants and limits for FDC1004
#define FDC1004_100HZ (0x01)
#define FDC1004_200HZ (0x02)
#define FDC1004_400HZ (0x03)
#define FDC1004_IS_RATE(x) (x == FDC1004_100HZ || \
                            x == FDC1004_200HZ || \
                            x == FDC1004_400HZ)

#define FDC1004_CAPDAC_MAX (0x1F)

#define FDC1004_CHANNEL_MAX (0x03)
#define FDC1004_IS_CHANNEL(x) (x >= 0 && x <= FDC1004_CHANNEL_MAX)

#define FDC1004_MEAS_MAX (0x03)
#define FDC1004_IS_MEAS(x) (x >= 0 && x <= FDC1004_MEAS_MAX)

#define FDC_REGISTER (0x0C)

#define ATTOFARADS_UPPER_WORD (457) //number of attofarads for each 8th most lsb (lsb of the upper 16 bit half-word)
#define FEMTOFARADS_CAPDAC (3028) //number of femtofarads for each lsb of the capdac

// =============================================================================
// Data Types and Enumerations
// =============================================================================

/**
 * @brief Error codes returned by FDC1004 functions
 */
typedef enum {
    FDC1004_SUCCESS = 0,                    ///< Operation completed successfully
    FDC1004_ERROR_INVALID_PARAMETER,       ///< Invalid parameter provided
    FDC1004_ERROR_MEASUREMENT_NOT_READY,   ///< Measurement not completed yet
    FDC1004_ERROR_I2C_COMMUNICATION,       ///< I2C communication error
    FDC1004_ERROR_DEVICE_NOT_FOUND,        ///< Device not responding
    FDC1004_ERROR_CAPDAC_OUT_OF_RANGE      ///< CAPDAC value adjustment needed
} fdc1004_error_t;

/**
 * @brief Sample rate options for measurements
 */
typedef enum {
    FDC1004_RATE_100HZ = FDC1004_SAMPLE_RATE_100HZ,    ///< 100 Hz sample rate
    FDC1004_RATE_200HZ = FDC1004_SAMPLE_RATE_200HZ,    ///< 200 Hz sample rate
    FDC1004_RATE_400HZ = FDC1004_SAMPLE_RATE_400HZ     ///< 400 Hz sample rate
} fdc1004_sample_rate_t;

/**
 * @brief Channel identifiers
 */
typedef enum {
    FDC1004_CHANNEL_0 = 0,
    FDC1004_CHANNEL_1 = 1,
    FDC1004_CHANNEL_2 = 2,
    FDC1004_CHANNEL_3 = 3
} fdc1004_channel_t;

/**
 * @brief Measurement slot identifiers
 */
typedef enum {
    FDC1004_MEASUREMENT_1 = 0,
    FDC1004_MEASUREMENT_2 = 1,
    FDC1004_MEASUREMENT_3 = 2,
    FDC1004_MEASUREMENT_4 = 3
} fdc1004_measurement_t;

/**
 * @brief Raw measurement data structure
 */
typedef struct {
    int16_t value;      ///< Raw capacitance measurement value
    uint8_t capdac;     ///< CAPDAC offset used for this measurement
} fdc1004_raw_measurement_t;

/**
 * @brief Processed capacitance measurement
 */
typedef struct {
    float capacitance_pf;       ///< Capacitance in picofarads
    bool capdac_out_of_range;   ///< True if CAPDAC needs adjustment
    uint8_t capdac_used;        ///< CAPDAC value used for measurement
} fdc1004_capacitance_t;

// =============================================================================
// FDC1004 Class Declaration
// =============================================================================

/**
 * @brief Main class for interfacing with the FDC1004 capacitance sensor
 * 
 * This class provides a high-level interface for the Texas Instruments FDC1004
 * 4-channel capacitance-to-digital converter. It supports single-ended measurements
 * with automatic CAPDAC adjustment for extended range.
 * 
 * Example usage:
 * @code
 * FDC1004 sensor(FDC1004_RATE_100HZ);
 * if (sensor.begin()) {
 *     float capacitance = sensor.getCapacitancePicofarads(FDC1004_CHANNEL_0);
 *     Serial.println(capacitance);
 * }
 * @endcode
 */
class FDC1004 {
public:
    // =========================================================================
    // Constructors and Initialization
    // =========================================================================
    
    /**
     * @brief Constructor with sample rate specification
     * @param rate Sample rate for measurements (default: 100Hz)
     * @param address I2C address (default: 0x50)
     */
    FDC1004(fdc1004_sample_rate_t rate = FDC1004_RATE_100HZ, uint8_t address = FDC1004_I2C_ADDRESS);
    
    /**
     * @brief Legacy constructor for backward compatibility
     * @param rate Sample rate as uint16_t
     */
    FDC1004(uint16_t rate);
    
    /**
     * @brief Initialize the FDC1004 sensor
     * @return true if initialization successful, false otherwise
     */
    bool begin();
    
    /**
     * @brief Check if device is connected and responding
     * @return true if device is responding, false otherwise
     */
    bool isConnected();
    
    // =========================================================================
    // High-Level Measurement Functions
    // =========================================================================
    
    /**
     * @brief Get capacitance measurement in picofarads with automatic CAPDAC adjustment
     * @param channel Channel to measure (0-3)
     * @return Capacitance measurement structure
     */
    fdc1004_capacitance_t getCapacitanceMeasurement(fdc1004_channel_t channel);
    
    /**
     * @brief Get capacitance in picofarads (simplified interface)
     * @param channel Channel to measure (0-3)
     * @return Capacitance in picofarads, or NaN on error
     */
    float getCapacitancePicofarads(fdc1004_channel_t channel);
    
    /**
     * @brief Legacy function for backward compatibility
     * @param channel Channel to measure (default: 1)
     * @return Capacitance in femtofarads, or error code on failure
     */
    int32_t getCapacitance(uint8_t channel = 1);
    
    // =========================================================================
    // Configuration and Control
    // =========================================================================
    
    /**
     * @brief Set the sample rate for measurements
     * @param rate New sample rate
     * @return Error code
     */
    fdc1004_error_t setSampleRate(fdc1004_sample_rate_t rate);
    
    /**
     * @brief Get current sample rate
     * @return Current sample rate
     */
    fdc1004_sample_rate_t getSampleRate() const;
    
    /**
     * @brief Set CAPDAC value for a specific channel
     * @param channel Channel number (0-3)
     * @param capdac CAPDAC value (0-31)
     * @return Error code
     */
    fdc1004_error_t setCapdac(fdc1004_channel_t channel, uint8_t capdac);
    
    /**
     * @brief Get current CAPDAC value for a channel
     * @param channel Channel number (0-3)
     * @return Current CAPDAC value
     */
    uint8_t getCapdac(fdc1004_channel_t channel) const;
    
    // =========================================================================
    // Low-Level Hardware Interface
    // =========================================================================
    
    /**
     * @brief Configure a single-ended measurement
     * @param measurement Measurement slot (0-3)
     * @param channel Input channel (0-3)
     * @param capdac CAPDAC offset value (0-31)
     * @return Error code
     */
    fdc1004_error_t configureMeasurementSingle(fdc1004_measurement_t measurement, 
                                               fdc1004_channel_t channel, 
                                               uint8_t capdac);
    
    /**
     * @brief Trigger a single measurement
     * @param measurement Measurement slot to trigger
     * @param rate Sample rate for this measurement
     * @return Error code
     */
    fdc1004_error_t triggerSingleMeasurement(fdc1004_measurement_t measurement, 
                                             fdc1004_sample_rate_t rate);
    
    /**
     * @brief Read measurement result
     * @param measurement Measurement slot to read
     * @param value Pointer to store 24-bit measurement result
     * @return Error code
     */
    fdc1004_error_t readMeasurement(fdc1004_measurement_t measurement, uint16_t* value);
    
    /**
     * @brief Perform complete measurement cycle for a channel
     * @param channel Channel to measure
     * @param capdac CAPDAC value to use
     * @param value Pointer to store measurement result
     * @return Error code
     */
    fdc1004_error_t measureChannel(fdc1004_channel_t channel, uint8_t capdac, uint16_t* value);
    
    /**
     * @brief Get raw capacitance measurement with CAPDAC value
     * @param channel Channel to measure
     * @param value Pointer to store measurement structure
     * @return Error code
     */
    fdc1004_error_t getRawCapacitance(fdc1004_channel_t channel, fdc1004_raw_measurement_t* value);
    
    // =========================================================================
    // Legacy Interface (for backward compatibility)
    // =========================================================================
    
    uint8_t configureMeasurementSingle(uint8_t measurement, uint8_t channel, uint8_t capdac);
    uint8_t triggerSingleMeasurement(uint8_t measurement, uint8_t rate);
    uint8_t readMeasurement(uint8_t measurement, uint16_t* value);
    uint8_t measureChannel(uint8_t channel, uint8_t capdac, uint16_t* value);
    uint8_t getRawCapacitance(uint8_t channel, fdc1004_raw_measurement_t* value);
    uint16_t read16(uint8_t reg);

private:
    // =========================================================================
    // Private Members
    // =========================================================================
    
    uint8_t _i2c_address;               ///< I2C device address
    fdc1004_sample_rate_t _sample_rate; ///< Current sample rate
    uint8_t _capdac_values[4];          ///< Current CAPDAC values for each channel
    bool _device_initialized;           ///< Initialization status
    
    // =========================================================================
    // Private Methods - I2C Communication
    // =========================================================================
    
    /**
     * @brief Write 16-bit value to register
     * @param reg Register address
     * @param data Data to write
     * @return Error code
     */
    fdc1004_error_t writeRegister16(uint8_t reg, uint16_t data);
    
    /**
     * @brief Read 16-bit value from register
     * @param reg Register address
     * @param data Pointer to store read data
     * @return Error code
     */
    fdc1004_error_t readRegister16(uint8_t reg, uint16_t* data);
    
    /**
     * @brief Legacy I2C write function
     * @param reg Register address
     * @param data Data to write
     */
    void write16(uint8_t reg, uint16_t data);
    
    // =========================================================================
    // Private Methods - Utility Functions
    // =========================================================================
    
    /**
     * @brief Get measurement delay for current sample rate
     * @return Delay in milliseconds
     */
    uint8_t getMeasurementDelay() const;
    
    /**
     * @brief Convert raw measurement to picofarads
     * @param raw_value Raw measurement value
     * @param capdac CAPDAC value used
     * @return Capacitance in picofarads
     */
    float convertToPicofarads(int16_t raw_value, uint8_t capdac) const;
    
    /**
     * @brief Validate input parameters
     */
    bool isValidChannel(uint8_t channel) const;
    bool isValidMeasurement(uint8_t measurement) const;
    bool isValidCapdac(uint8_t capdac) const;
    bool isValidSampleRate(uint8_t rate) const;
    
    /**
     * @brief Auto-adjust CAPDAC based on measurement result
     * @param channel Channel number
     * @param raw_value Current measurement value
     * @return true if adjustment was made
     */
    bool autoAdjustCapdac(fdc1004_channel_t channel, int16_t raw_value);
};

#endif // _PROTOCENTRAL_FDC1004_H
