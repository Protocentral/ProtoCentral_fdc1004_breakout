Protocentral FDC1004 capacitance sensor breakout board
================================
[![Compile Examples](https://github.com/Protocentral/ProtoCentral_fdc1004_breakout/workflows/Compile%20Examples/badge.svg)](https://github.com/Protocentral/ProtoCentral_fdc1004_breakout/actions?workflow=Compile+Examples) 

## Don't have one? [Buy it here](https://protocentral.com/product/protocentral-fdc1004-capacitance-sensor-breakout-board/)

![FDC1004 Breakout](docs/images/fdc1004_brk.jpg) 


The FDC1004 is a single-chip IC for capacitance measurement for application including proximity sensing and liquid level sensing. This is based on the concept of measuring capacitance of an electrode with respect to ground.

This breakout board contains everything to connect it to an Arduino's I2C pins and it can be connected to any other platform with support for I2C two-wire interfaces.

We have made several cool projects with it including proximity sensing and liquid-level sensing from the outside of a tank. Check out our [Hackster page](https://www.hackster.io/team-protocentral/non-contact-capacitive-liquid-level-sensing-using-fdc1004-9333c7) for the detailed project description.

## Hardware Setup
Connection with the Arduino board is as follows:

|FDC1004 pin label| Arduino Connection  |Pin Function      |
 |:-----------------: |:---------------------:|:------------------:|
 | GND                | GND                   |  GND               |
 | SCL                | A5                    |  Serial Clock      |
 | Vcc                | 5V                    |  Power             |
 | SDA                | A4                    |  Serial Data       |


For further details of the board, refer the documentation
<center>

[ FDC1004 breakout board Documentation](https://docs.protocentral.com/getting-started-with-FDC1004/)

</center>



License Information
===================

![License](license_mark.svg)

This product is open source! Both, our hardware and software are open source and licensed under the following licenses:

Hardware
---------

**All hardware is released under [Creative Commons Share-alike 4.0 International](http://creativecommons.org/licenses/by-sa/4.0/).**
![CC-BY-SA-4.0](https://i.creativecommons.org/l/by-sa/4.0/88x31.png)

You are free to:

* Share — copy and redistribute the material in any medium or format
* Adapt — remix, transform, and build upon the material for any purpose, even commercially.
The licensor cannot revoke these freedoms as long as you follow the license terms.

Under the following terms:

* Attribution — You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
* ShareAlike — If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.

Software
--------

**All software is released under the MIT License(http://opensource.org/licenses/MIT).**

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


Please check [*LICENSE.md*](LICENSE.md) for detailed license descriptions.
