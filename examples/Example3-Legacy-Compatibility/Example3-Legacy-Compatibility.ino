//////////////////////////////////////////////////////////////////////////////////////////
//
//    Legacy API compatibility demo for the FDC1004 capacitance sensor breakout board
//
//    Author: Ashwin Whitchurch
//    Copyright (c) 2018 ProtoCentral
//
//    This example demonstrates that the original API still works unchanged,
//    ensuring backward compatibility with existing projects.
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
//   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
//   NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//   For information on how to use, visit https://github.com/protocentral/ProtoCentral_fdc1004_breakout
/////////////////////////////////////////////////////////////////////////////////////////

#include <Wire.h>
#include <Protocentral_FDC1004.h>

#define UPPER_BOUND  0X4000                 // max readout capacitance
#define LOWER_BOUND  (-1 * UPPER_BOUND)
#define CHANNEL 0                          // channel to be read
#define MEASURMENT 0                       // measurement channel

int capdac = 0;
char result[100];

// Using legacy constructor - this still works exactly as before!
FDC1004 FDC;

void setup()
{
  Wire.begin();        //i2c begin
  Serial.begin(115200); // serial baud rate
  
  Serial.println("FDC1004 Legacy API Compatibility Demo");
  Serial.println("=====================================");
  Serial.println("This example shows the original API still works unchanged!");
  Serial.println();
}

void loop()
{
  // Original API calls - completely unchanged from the original example
  FDC.configureMeasurementSingle(MEASURMENT, CHANNEL, capdac);
  FDC.triggerSingleMeasurement(MEASURMENT, FDC1004_100HZ);

  //wait for completion
  delay(15);
  uint16_t value[2];
  if (! FDC.readMeasurement(MEASURMENT, value))
  {
    int16_t msb = (int16_t) value[0];
    int32_t capacitance = ((int32_t)457) * ((int32_t)msb); //in attofarads
    capacitance /= 1000;   //in femtofarads
    capacitance += ((int32_t)3028) * ((int32_t)capdac);

    Serial.print((((float)capacitance/1000)),4);
    Serial.print("  pf, ");

    if (msb > UPPER_BOUND)               // adjust capdac accordingly
	{
      if (capdac < FDC1004_CAPDAC_MAX)
	  capdac++;
    }
	else if (msb < LOWER_BOUND)
	{
      if (capdac > 0)
	  capdac--;
    }

  }
  
  // You can also mix legacy and new API calls:
  // int32_t legacy_capacitance = FDC.getCapacitance(0); // Returns femtofarads
  // float new_capacitance = FDC.getCapacitancePicofarads(FDC1004_CHANNEL_0); // Returns picofarads
}
