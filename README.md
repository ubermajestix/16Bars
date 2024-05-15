# 16Bars
[![FirmwareBuild](https://github.com/ubermajestix/16Bars/actions/workflows/main.yml/badge.svg)](https://github.com/ubermajestix/16Bars/actions/workflows/main.yml)

![16barsPCB](https://user-images.githubusercontent.com/12533/156662021-8fc3b2a8-bc3f-4ee7-a015-541e6d8de693.png)

16Bars is a utility module designed to display how many 
measures have elapsed on a seven segment display given a clock pulse. 
The module resets every 8 or 16 measures (bars) or when the reset button is pressed.

This is my first module design and I've made several simplifying design choices:
1. A 1U version would be much more convienient
2. Reset jack would be nice to play better with midi or modules that output reset
3. Midi support? Accept MIDI TRS input for start, stop, and reset?
4. LFO output with LFO reset, could use 1st bar, n/2 bars, and last bar to self patch musical phrased LFO

## Rev 2.0 Updates and Issues
I was successfully able to get JLCPCB to manufacture and place a lot of SMD components as I don't have the time or gear to get into surface mount, win! 

![front of 2.0](https://github.com/ubermajestix/16Bars/blob/main/Fabrication/Rev2.0/Screenshot%202023-03-31%20at%2010.10.03%20AM.png)
![back of 2.0](https://github.com/ubermajestix/16Bars/blob/main/Fabrication/Rev2.0/Screenshot%202023-03-31%20at%2010.10.35%20AM.png)


I also breadboarded out and build test PCBS to validate significant portion of the Rev 2.0 design:
- Reset button uses a transistor to send a reset signal instead of directly sending +12v when pressed
- Two transistors to normalize a wide variety of incoming clock signal voltages and send +12v pulses to trigger the three counter ICs and blink the led in the reset button 
- 8 bar reset logic confirmed, 16 bar reset logic didn't work because I was rushing to get Rev 2.0 board to production.
- Built a switch test PCB. I ordered 4 different switches and after getting incorrect footprints for the seven segment leds from Mouser I didn't trust the switch footprints. I also wanted to validate the switch weight, height, footprint size tradeoffs, validate the switches are in-fact on-on SPDT, and ensure the switches are too able to be flipped by accident when patching as they control the reset logic and time signature (4/4 and 8/4). 

Issues
- The [surface mount transistors](https://www.lcsc.com/product-detail/Bipolar-Transistors-BJT_Jiangsu-Changjing-Electronics-Technology-Co-Ltd-C9634_C9634.html) do not work as intended. I am considering replacing them with through-hole NPN's I'm using on the breadboard to validate it's either a component problem or PCB routing problem. 

# Panel Design
Panel design ideas, PCB, and logo are in Panel/. I'm aiming for a wind theme throughout the design and naming.

Rev 2.0 panel design is coming along:

![panel preview](https://raw.githubusercontent.com/ubermajestix/16Bars/main/Panel/Designs/Rev2Panel-Preview.png)

# LED Sheild Practice aka Clock Blink
The design and fabrication process can be intimidating for a beginner. What I've learned from
software development is to start easy and get something small all the way to production before tackling bigger
projects. 

I followed [Contextual Electronic's _Getting To Blinky 5.0](https://www.youtube.com/watch?v=BVhWh3AsXQs)_ to learn
KiCAD schematic design, PCB layout and design, and fabrication on a simple circuit.

I wanted to use a keyed power header, know it worked by lighting up an LED, and take a clock pulse in through a jack to light up an led.

I fabricated the boards and got the power led to light up from my Eurorack case! The blinking feature isn't working and
I'm debugging it as of March 2022. 

# Practice Panel
I also wanted to practice making a panel. Again, to simplify, I wanted to get the mounting holes placed correctly with oval
holes like Intellijel 1U modules b/c my rails are never exactly aligned top to bottom. I was planning to make a control panel for the
the ClockBlink board but I figured why not make a blank 1U panel with some Fiverr artwork? 

The blank panels turned out great, I fabbed them with OshPark.com in purple (their default) and black (After Dark). The holes
line up and they were $7 and $12 shipped for three copies.

![sheep1u](https://user-images.githubusercontent.com/12533/156663417-cd1b6f46-bb50-49d7-b74c-5b3351a9ba3c.png)


# License
[Attribution 4.0 International (CC BY 4.0)](https://creativecommons.org/licenses/by/4.0/)

![Open-source-hardware-logo](https://user-images.githubusercontent.com/12533/228918104-3c78cb60-c5c0-4262-b0ad-a671cfebdcf8.png)
