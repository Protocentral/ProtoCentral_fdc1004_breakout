/*
 this example measures raw capacitance across CHANNEL0 and Gnd and 
 prints on serial terminal
 Arduino connections
 Arduino   FDC1004 board
-------   -------------
     5V - Vin
    GND - GND
    SDA - A4
    SCL - A5
*/	
	
#include <Wire.h>
#include <Protocentral_FDC1004.h>

#define UPPER_BOUND  0X4000                 // max readout capacitance 
#define LOWER_BOUND  (-1 * UPPER_BOUND)    
#define CHANNEL 0                          // channel to be read
#define MEASURMENT 0                       // measurment channel

int capdac = 0;
char result[100];

FDC1004 FDC;

void setup()
{
  Wire.begin();        //i2c begin
  Serial.begin(115200); // serial baud rate
}

void loop() 
{

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
}
