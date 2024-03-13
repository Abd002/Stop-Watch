## Stop Watch System Implementation

This repository contains the implementation of a Stop Watch system based on the following specifications:

1. **Microcontroller:** ATmega32 operating at a frequency of 1MHz.
2. **Timer Configuration:** Timer1 in ATmega32 configured in CTC mode to count the Stop Watch time.
3. **Display:** Utilizes six Common Anode 7-segment displays.
4. **Multiplexing Technique:** The 7-segments are connected using the multiplexed technique. A 7447 decoder is used for all 7-segments, and their enable/disable is controlled by NPN BJT transistors connected to MCU pins.
5. **Multiplexing Method:** Enables connecting more than one 7-segment display. The Microcontroller drives one 7-segment display at a time while others are turned off, switching displays using transistors.
6. **Connection:** Connect 7447 decoder 4-pins to the first 4-pins in PORTC.
7. **Control Pins:** Utilizes the first 6-pins in PORTA as the enable/disable pins for the six 7-segments.
8. **Start Condition:** Stop Watch counting starts once power is connected to the MCU.
9. **External Interrupt INT0:** Configured with a falling edge. A push button with an internal pull-up resistor is connected. When a falling edge is detected, the Stop Watch time is reset.
10. **External Interrupt INT1:** Configured with a rising edge. A push button with an external pull-down resistor is connected. When a rising edge is detected, the Stop Watch time is paused.
11. **External Interrupt INT2:** Configured with a falling edge. A push button with an internal pull-up resistor is connected. When a falling edge is detected, the Stop Watch time is resumed.
12. **Reference:** Watch this video for further details: [Stop Watch System Video](https://youtu.be/emp-musYxII)
