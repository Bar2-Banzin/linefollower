#define __AVR_ATmega328P__
#include <avr/io.h>
#include <util/delay.h>

void main(void)
{
    DDRB |= 1<<5;
    while(1)
    {
        PORTB |= 1<<5;
        _delay_ms(1000);
        PORTB &= ~(1<<5);
        _delay_ms(1000);
    }
}