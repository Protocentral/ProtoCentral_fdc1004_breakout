ProtoCentral FDC1004 Capacitance Sensor Breakout Board
================================
[![Laser Sensor](https://www.protocentral.com/4064-large_default/protocentral-vl53l0x-laser-tof-sensor-breakout.jpg)  
*PROTOCENTRAL VL53L0X LASER TOF SENSOR BREAKOUT (PC-SEN-2001)*](https://www.protocentral.com/motiondistance/1049-protocentral-vl53l0x-laser-tof-sensor-breakout.html)

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
         5V - VIN
        GND - GND
        SDA - SDA
        SCL - SCL
 
###  Uploading Arduino Sketch For VL530lx breakout

 a. Open the Arduino IDE to load the embedded code for VL530lx.

 b. Click on File -> Open and Browse the .ino code for Arduino from Protocentral_VL530lx-master\arduino and export the application to Arduino.

###  Open Processing & launch the openview_tof

 a. If Processing IDE was open, close it and reopen to refresh the libraries and sketches. The repositories are not refreshed if the IDE was open while the Sketches and Libraries were being updated.

 b. Double-click any of the .pde files in the openview_pulseox directory to open all of the pulse oximeter code in the Processing IDE.

 c. If everything done so far was good, clicking the "run" button on the top left corner of the IDE, should run the code! If it does not, make sure you installed your libraries correctly.

 d. Once the GUI is running, select the port connect with breakout board from the "SELECT PORT" dropdown as shown in the figure below

![Port Selection](https://github.com/Protocentral/Protocentral_VL530LX/blob/master/Processing%20Code/Final%20Output/portselection.png)

 e. Once the port selection is appropriate the START button gets enabled. Click "START" to initiate visualization

 f. You should see the PPG ( RED and IR) waves generated with the values obtained from the AFE4490 Oximeter Shield Breakout Board as shown below.

![Read mm Form in Processing](https://github.com/Protocentral/Protocentral_VL530LX/blob/master/Processing%20Code/Final%20Output/output.png)


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
