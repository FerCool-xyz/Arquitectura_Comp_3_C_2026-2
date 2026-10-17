//Pendulo de 8 velocidades, 3 entradas DIP, 4  salidas
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

// velocidades
const uint16_t tabla_velocidades[8] PROGMEM = {
    500,
    400,
    300,
    200,
    150,
    100,
    60,
    30};

void init_system(void)
{

    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2);
    DDRD |= (1 << PD7);

    DDRC &= ~((1 << PC3) | (1 << PC4) | (1 << PC5));
    PORTC |= (1 << PC3) | (1 << PC4) | (1 << PC5);
}

void apagar_leds(void)
{
    PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2));
    PORTD &= ~(1 << PD7);
}

int main(void)
{
    init_system();

    uint8_t selector = 0;
    uint16_t retardo = 0;

    while (1)
    {

        selector = (PINC >> 3) & 0x07;

        retardo = pgm_read_word(&tabla_velocidades[selector]);

        apagar_leds();
        PORTB |= (1 << PB0);
        for (uint16_t i = 0; i < retardo; i++)
            _delay_ms(1);

        apagar_leds();
        PORTB |= (1 << PB1);
        for (uint16_t i = 0; i < retardo; i++)
            _delay_ms(1);

        apagar_leds();
        PORTB |= (1 << PB2);
        for (uint16_t i = 0; i < retardo; i++)
            _delay_ms(1);

        apagar_leds();
        PORTD |= (1 << PD7);
        for (uint16_t i = 0; i < retardo; i++)
            _delay_ms(1);

        apagar_leds();
        PORTB |= (1 << PB2);
        for (uint16_t i = 0; i < retardo; i++)
            _delay_ms(1);

        apagar_leds();
        PORTB |= (1 << PB1);
        for (uint16_t i = 0; i < retardo; i++)
            _delay_ms(1);
    }
}
