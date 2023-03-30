# 16Bars

![16barsPCB](https://user-images.githubusercontent.com/12533/156662021-8fc3b2a8-bc3f-4ee7-a015-541e6d8de693.png)

16Bars is a utility module designed to display how many 8-beat 
measures have elapsed on a seven segment display given a quarter note clock pulse. 
The module resets every 16 measures (bars) or when the reset button is pressed.

This is my first module design and I've made several simplifying design choices:
1. Only takes quarter note pulses
2. ~Only counts 8-beats/measure~ Rev 2 can run in 4/4 or 8/4 mode
3. Only uses IC's, no microcontroller.
4. Only 1 PCB in 3U format. 1U would require a stacking PCBs
5. Only a reset button, no reset jack.

Obviously this list is a roadmap for future improvements. ~I'd like the clock input and beats/measure settings to be configurable 
with jumpers or switches~ Done! A 1U version would be much more convienient, reset jack would be nice, etc. 

## Rev 2.0 Updates and Issues
I was successfully able to get JLCPCB to manufacture and place a lot of SMD components as I don't have the time or gear to get into surface mount, win! 

I also breadboarded out and build test PCBS to validate significant portion of the Rev 2.0 design:
- Reset button uses a transistor to send a reset signal instead of directly sending +12v when pressed
- Two transistors to normalize a wide variety of incoming clock signal voltages and send +12v pulses to trigger the three counter ICs and blink the led in the reset button 
- 8 bar reset logic confirmed, 16 bar reset logic didn't work because I was rushing to get Rev 2.0 board to production.
- Built a switch test PCB. I ordered 4 different switches and after getting incorrect footprints for the seven segment leds from Mouser I didn't trust the switch footprints. I also wanted to validate the switch weight, height, footprint size tradeoffs, validate the switches are in-fact on-on SPDT, and ensure the switches are too able to be flipped by accident when patching as they control the reset logic and time signature (4/4 and 8/4). 

Issues
- The [surface mount transistors](https://www.lcsc.com/product-detail/Bipolar-Transistors-BJT_Jiangsu-Changjing-Electronics-Technology-Co-Ltd-C9634_C9634.html) do not work as intended 

## LED Sheild Practice aka Clock Blink
The design and fabrication process can be intimidating for a beginner. What I've learned from
software development is to start easy and get something small all the way to production before tackling bigger
projects. 

I followed [Contextual Electronic's _Getting To Blinky 5.0](https://www.youtube.com/watch?v=BVhWh3AsXQs)_ to learn
KiCAD schematic design, PCB layout and design, and fabrication on a simple circuit.

I wanted to use a keyed power header, know it worked by lighting up an LED, and take a clock pulse in through a jack to light up an led.

I fabricated the boards and got the power led to light up from my Eurorack case! The blinking feature isn't working and
I'm debugging it as of March 2022. 

# LED Shield Practice Panel
I also wanted to practice making a panel. Again, to simplify, I wanted to get the mounting holes placed correctly with oval
holes like Intellijel 1U modules b/c my rails are never exactly aligned top to bottom. I was planning to make a control panel for the
the ClockBlink board but I figured why not make a blank 1U panel with some Fiverr artwork? 

The blank panels turned out great, I fabbed them with OshPark.com in purple (their default) and black (After Dark). The holes
line up and they were $7 and $12 shipped for three copies.

![sheep1u](https://user-images.githubusercontent.com/12533/156663417-cd1b6f46-bb50-49d7-b74c-5b3351a9ba3c.png)
