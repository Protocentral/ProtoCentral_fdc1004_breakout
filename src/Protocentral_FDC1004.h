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

/********************************************************************************************************
 * typedefs
 *******************************************************************************************************/
typedef struct fdc1004_measurement_t
{
    int16_t value;
    uint8_t capdac;
}fdc1004_measurement_t;

/******************************************************************************************
 * Function Declarations
 ******************************************************************************************/
class FDC1004
{
 public:
    FDC1004(uint16_t rate = FDC1004_100HZ);
    int32_t getCapacitance(uint8_t channel = 1);
    uint8_t getRawCapacitance(uint8_t channel, fdc1004_measurement_t * value);
    uint8_t configureMeasurementSingle(uint8_t measurement, uint8_t channel, uint8_t capdac);
    uint8_t triggerSingleMeasurement(uint8_t measurement, uint8_t rate);
    uint8_t readMeasurement(uint8_t measurement, uint16_t * value);
    uint8_t measureChannel(uint8_t channel, uint8_t capdac, uint16_t * value);
	    uint16_t read16(uint8_t reg);

 private:
    uint8_t _addr;
    uint8_t _rate;
    uint8_t _last_capdac[4];
    void write16(uint8_t reg, uint16_t data);

};

#endif
