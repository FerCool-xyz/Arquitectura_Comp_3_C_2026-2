#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

ISR(INT0_vect)
{

    PORTC ^= (1 << PORTC5); 
    
    // Función anti-rebote por software
    _delay_ms(50); 
    
    EIFR = (1 << INTF0); 
}

int main(void)
{

    DDRC |= (1 << DDC5);          

    DDRD &= ~(1 << DDD2);         
    
    PORTD |= (1 << PORTD2);       

    // Configuración de los modos
    // Programa 1 - Nivel bajo
    //EICRA = 0;
    //Programa 2 - Cualquier cambio
    EICRA = (1 << ISC00);
    //Programa 3 - Flanco de bajada
    //EICRA = (1 << ISC01);
    //Programa 4 - Flanco de subida
    //EICRA = (1 << ISC01) | (1 << ISC00);


    EIFR = (1 << INTF0);
    EIMSK = (1 << INT0);
    sei();

    while (1)
    {

    }
}
