/*
 * Code to write data to two daisychained TPIC6B595 SIPO shift registers.
 * 
 * The TPIC6B595 is a monolithic, high-voltage, medium-current power 8-bit 
 * shift register designed for use in systems that require relatively high 
 * load power.
 *
 * This device contains an 8-bit serial-in, parallel-out shift register that 
 * feeds an 8-bit D-type storage register. Data transfers through both the 
 * shift and storage registers on the rising edge of the shift-register clock 
 * (SRCK) and the register clock (RCK), respectively. The storage register 
 * transfers data to the output buffer when shift-register clear (SRCLR) is 
 * high. When SRCLR is low, the input shift register is cleared. When output
 * enable (G) is held high, all data in the output buffers is held low and all 
 * drain outputs are off. When G is held low, data from the storage register 
 * is transparent to the output buffers. When data in the output buffers is 
 * low, the DMOS-transistor outputs are off. When data is high, the DMOS- 
 * transistor outputs have sink-current capability. The serial output (SER 
 * OUT) allows for cascading of the data from the shift register to additional 
 * devices.
 *
 * http://www.adafruit.com/datasheets/tpic6b595.pdf
 * 
 * To compile and upload run: make clean; make; make program;
 * 
 * Copyright 2011-2012 Mika Tuupola
 *
 * Licensed under the MIT license:
 *   http://www.opensource.org/licenses/mit-license.php
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>

#include "main.h"
#include "uart/uart.h" 
#include "shift/shift.h"

int main(void) {    
    
    shift_out_init();
    uart_init();
    stdout = &uart_output;
    stdin  = &uart_input;
    char binary[17];
    
    /* Show pattern for 5 seconds. */
    shift_out(0b10101010);
    shift_out(0b11110000);
    shift_out_latch();
    _delay_ms(5000);
    
    while (1) {
        for(uint16_t i = 0; i < 0xffff; i++) {
            
            /* Print the number to serial for debugging. */
            itoa(i, binary, 2);
            printf("%s %d \n", binary, i);

            /* Shift high byte first to shift register. */
            shift_out(i >> 8); 
            shift_out(i & 0xff);

            /* Pulse latch to transfer data from shift registers */
            shift_out_latch();

            _delay_ms(50);
        }
    }
    return 0;
  
}
