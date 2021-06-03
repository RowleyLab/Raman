Firmware Notes
==============

Dependencies
------------

The firmware is written in Energia, which is a fork of the popular Arduino software development platform adapted for use with TI MSP products. Energia can be downloaded [here](https://energia.nu/ "Energia")

Energia does not come bundled with everything needed. Two additional steps are required to prepare for working with the MSP432P401R launchpad
* Install the Red Launchpad MSP432P401R libraries in the *Boards Manager*. The manager is found in the *Tools* menu, at the top of the board selection list
* The top half of the MSP432 launchpad board includes an XDS 110 debugger. This peripheral hardware helps with programming the microcontroller and facilitates serial communication over USB. Some systems may require installation of the XDS 110 debugger drivers. These drivers come packaged with TIs *Code Composer Studio*, or can be found packaged alone on TAs own website. However, since we are using Energia I recommend finding the drivers linked from their website [here](http://energia.nu/files/xds110_drivers.zip "xds1110 Drivers")



