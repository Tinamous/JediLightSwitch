Jedi LightSwitch PCB.
=====================

Licensed under Creative Commons ShareAlike 4.0 https://creativecommons.org/licenses/by-sa/4.0/

PCB Fabrication by OSH Park.
PCB: <TODO: Insert PCB link>

PCB Stensil by OSH Stencil (https://oshstencils.com/).


PCB Assembly Notes:
-------------------
APDS-9960 and HTU21D are tricky components to solder onto the board. The switch may be used without them if you wish to not solder them and prefer to use touch only.

Use either a hot air gun for solder reflow or reflow oven for the top layer. Use a stencil from OSH Stencils and place U1/U2 (APDS-9960 and HTU21D) carefully. 

Using a low flow and a temperature appropriate for the solder paste heat the board around U1/U2 bringing until the solder melds. Allow a little longer for U1 as the solder is not visible.

The bottom of the board should be hand solderered to prevent U1/U2 dropping off. Most parts are 1206 or 0805 and not overly difficult. U4 (AT42QT1010) is a little difficult but excess solder can be removed with solder wick if needed.

The value of C4 may need to be adjusted to increase/reduce sensitivity. 10nF was found to be to little. 68nF was overly sensitive. A 22nF capacitor worked well.

The value of R5/R6/R8 and R9 should be selected based on the required brightness of the LEDs and the LED selected as well as the enclosure. For a clear enclosure increase the resistance (e.g. 1k) to dim the LEDs, other materials such as grey PLA let enough light through to work well with the blue LEDs and 100R resistors.


Resources:
----------
SparkFun Eagle Library and breakout boards used under CC-SA.

SparkFun Eagle Library: https://github.com/sparkfun/SparkFun-Eagle-Libraries

SparkFun APDS-9960 breakout: https://www.sparkfun.com/products/12787
SparkFun HTU21D breakout: https://www.sparkfun.com/products/12064
SparkFun AT42QT1010 breakout: https://www.sparkfun.com/products/12041