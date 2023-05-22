# TODO
1. Breadboard 4510 reset logic with two ics, 4 transistors, a reset button, and an led
- 5/15 Breadboarded two 4510's, reset signal from 4510 outputs not strong enough to reset both ICs. Button reset works, validate current restriction resistor from +12v to button. 
2. Seven segment sub-board. Use mousebites to breakoff sub-board that contains two 4033 ICs and seven segment LEDs with resistor and capictors in place. Build headers to send +12, GND, ClockBar, and Reset. Saves time testing, I know the 4033's and seven segment leds work, and this would push them closer to the panel for easier viewing. 
3. More jacks: 
    1. Inputs: reset jack, 
    2. Outputs: first bar, middle bar, last bar, clock bar
4. Move entire build to microcontroller 