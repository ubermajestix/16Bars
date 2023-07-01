# TODO
6. Reduce to 4HP.
7. Is SevenSegment more stable with shift registers controlled by Arduino? 4033 docs say they are easily inteferred with, and they seem a little wonky, maybe it's my soldering, but they don't always work. 
8. Some pins on Nano are non-functional or a bit wonky
* D12 output (firstbar jack) very low voltage
* D6 input (8/16 bars switch) non functional, jumpered to A4
9. Probably need to move to Schmitt trigger, does JLC have SMD? This would help condition outputs to correct voltage and protect Nano from modular craziness. 
10. SeverSegment - assemble with ICs on top for more space
11. Use 2x 01x02 connectors for seven segment to make it more mechanically stable.