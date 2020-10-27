# MPLAB Assignments 2k20

## What's this??

* This repository contains the solutions of *Embedded Systems* 2020 assignments

* All the solutions are suitable for **disPIC30f4011** board (there are some portings also for *ATmega328P* aka *Arduino UNO*)

## Setup

* Download [MPLAB X IDE 5.35](https://www.microchip.com/development-tools/pic-and-dspic-downloads-archive) *(choose the right version and not the newest, which is buggy)*

* Download [XC16 Compiler](https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-xc-compilers)

* If you want to build you applications also for *ATmega328P*

  * Download [XC8 Compiler](https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-xc-compilers)

  * Download [USBTinyISP drivers](https://learn.adafruit.com/usbtinyisp/drivers)

  * Download [WinAVR](https://sourceforge.net/projects/winavr/)

## Simulating

* You can build and execute the assignments made for *disPIC30f4011* in local machine by using the MPLAB X built in simulator

* If you are rich enough, you can buy a real board and also a programmer like *PICkit 4* and deploy to it

## Deploy to ATmega328P/Arduino UNO

* If you are not so rich, you can buy an *Arduino UNO* and a *USBTinyISP* programmer

* For each *ATmega328P*-compatible project, you can

  * Build the solution with the MPLAB X built-in command

  * Export the HEX file *(right click on project -> Export HEX file)*

  * Open a Powershell in the directory of the HEX file and run
```
avrdude -c usbtiny -p m328p -P \\.\COMX -Uflash:w:FILE.hex
```
Replace *COMX* with the right COM port and *FILE* with the right filename

* [Here](http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf) you can find the *ATmega328P* datasheet

* And [here](https://content.arduino.cc/assets/UNO-TH_Rev3e_sch.pd) there's *Arduino UNO* schematics
