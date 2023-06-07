# TODO
1. Moved to Arduino, software for basic operation done. 
2. Need to redo schematic/pcb for Nano.
3. Add jacks to schematic/pcb (with schmitt trigger ic) for reset input, bar clock, first bar, middle bar, last bar.
  * Can we power leds and resets or cmos logic before schmitt trigger?
4. Power conditioning to 5v for nano
5. Add 4 pin header for seven segment sub-board to schematic/pcb
6. Reduce to 4HP.
7. Is SevenSegment more stable with shift registers controlled by Arduino? 4033 docs say they are easily inteferred with, and they seem a little wonky, maybe it's my soldering, but they don't always work. 
