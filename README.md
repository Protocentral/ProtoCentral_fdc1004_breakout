ProtoCentral FDC1004 capacitance sensor breakout board
================================
![FDC1004 Breakout](docs/images/fdc1004_brk.jpg)  

If you don't already have one, you can buy the ProtoCentral FDC1004 breakout board from:

* [ProtoCentral(Worldwide)](https://www.protocentral.com/gasliquid/1082-fdc1004-capacitance-converter-breakout-board-0642078949616.html)

* [ProtoCentral USA](https://protocentral.us/breakout-boards/9-prototypical-fdc1004-capacitance-sensor-breakout-board.html)

The FDC1004 is a single-chip IC for capacitance measurement for application including proximity sensing and liquid level sensing. This is based on the concept of measuring capacitance of an electrode with respect to ground.

This breakout board contains everything to connect it to an Arduino's I2C pins and it can be connected to any other platform with support for I2C two-wire interfaces.

We have made several cool projects with it including proximity sensing and liquid-level sensing from the outside of a tank. Check out our [Hackster page](https://www.hackster.io/team-protocentral/non-contact-capacitive-liquid-level-sensing-using-fdc1004-9333c7) for the detailed project description.

## Features

* Breakout contains the Texas Instruments FDC1004 IC
* 4 channel measurement
* On-board 3.3V voltage regulator
* Output data rates up to 400 Samples/second
* Easy-to-use I2C interface

## Specifications
* Capacitance measurement range: +/- 15 pF
* Resolution: 0.5 fF
* Maximum offset capacitance: 100 pF

## Includes
* 1x ProtoCentral FDC1004 Capacitance Sensor Breakout Board
* 1x Set of breakaway headers

## Repository Contents

* **/software** - Arduino library and example sketch
* **/hardware** - All Eagle design files (.brd, .sch)

## Connecting the breakout to an Arduino

Wire the FDC1004 breakout to an Arduino Uno using the following connections.


    Arduino   FDC1004 board
    -------   -------------
         5V <-> VCC
        GND <-> GND
         A4 <-> SDA
         A5 <-> SCL

##  Uploading Arduino Sketch For FDC1004 breakout

1. Open the Arduino IDE to load the embedded code for Protocentral_FDC1004.

2. Click on File -> Open and Browse the .ino code for Arduino from Protocentral_FDC1004-master\arduino and export the application to Arduino.

3.  Open serial terminal window, you should see the capacitance values obtained from the FDC1004 Breakout Board.

License Information
===================

Hardware
---------

**All hardware is released under [Creative Commons Share-alike 4.0 International](http://creativecommons.org/licenses/by-sa/4.0/).**

You are free to:

Share — copy and redistribute the material in any medium or format
Adapt — remix, transform, and build upon the material
for any purpose, even commercially.
The licensor cannot revoke these freedoms as long as you follow the license terms.
Under the following terms:

Attribution — You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
ShareAlike — If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
No additional restrictions — You may not apply legal terms or technological measures that legally restrict others from doing anything the license permits.
Notices:

You do not have to comply with the license for elements of the material in the public domain or where your use is permitted by an applicable exception or limitation.
No warranties are given. The license may not give you all of the permissions necessary for your intended use. For example, other rights such as publicity, privacy, or moral rights may limit how you use the material.


Software
--------

**All software is released under the MIT License(http://opensource.org/licenses/MIT).**

The MIT License (MIT)

Copyright (c) 2019 ProtoCentral

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
