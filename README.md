ProtoCentral FDC1004 Capacitance Sensor Breakout Board
================================
[![Laser Sensor](https://www.protocentral.com/4220-thickbox_default/fdc1004-capacitance-converter-breakout-board.jpg)  
*PROTOCENTRAL FDC1004 CAPACITANCE SENSOR BREAKOUT BOARD (PC-SEN-2002)*](https://www.protocentral.com/gasliquid/1082-fdc1004-capacitance-converter-breakout-board.html?search_query=fdc1004&results=1)

The FDC1004 is a single-chip IC for capacitance measurement for application including proximity sensing and liquid level sensing. This is based on the concept of measuring capacitance of an electrode with respect to ground. 

This breakout board contains everything to connect it to an Arduino's I2C pins and it can be connected to any other platform with support for I2C two-wire interfaces. 

We have made several cool projects with it including proximity sensing and liquid-level sensing from the outside of a tank. Check out our Hackster page (hackster.io/protocentral) for the detailed project description. 

Features:
---------
Breakout contains the Texas Instruments FDC1004 IC 
4 channel measurement
On-board 3.3V voltage regulator
Output data rates up to 400 Samples/second
Easy-to-use I2C interface

Specifications:
---------------
Capacitance measurement range: +/- 15 pF
Resolution: 0.5 fF
Maximum offset capacitance: 100 pF

Includes:
----------
* 1x ProtoCentral FDC1004 Capacitance Sensor Breakout Board
* 1x Set of breakaway headers

Repository Contents
-------------------
* **/software** - Arduino library and example sketch
* **/hardware** - All Eagle design files (.brd, .sch)
* **/extras** - datasheets and extras

Arduino boards connections


    Arduino   FDC1004 board
    -------   -------------
         5V - VCC
        GND - GND
         A4 - SDA
         A5 - SCL
 
###  Uploading Arduino Sketch For FDC1004 breakout

 a. Open the Arduino IDE to load the embedded code for Protocentral_FDC1004.

 b. Click on File -> Open and Browse the .ino code for Arduino from Protocentral_FDC1004-master\arduino and export the application to Arduino.
 
 c.  Open serial terminal window, you should see the capacitance values obtained from the FDC1004 Breakout Board as shown below.
![Read RAW	capacitance](https://www.protocentral.com/img/p/4/2/1/2/4212.jpg?time=1482129242629)


License Information
===================
This product is open source! Both, our hardware and software are open source and licensed under the following licenses:

Hardware
---------
**All hardware is released under [Creative Commons Share-alike 4.0 International](http://creativecommons.org/licenses/by-sa/4.0/).**
![CC-BY-SA-4.0](https://i.creativecommons.org/l/by-sa/4.0/88x31.png)

Software
--------
**All software is released under the MIT License(http://opensource.org/licenses/MIT).**

Please check [*LICENSE.md*](LICENSE.md) for detailed license descriptions.
