#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdint.h>

const uint8_t num_estados = 6;

// Rojo = 0, Verde = 1, Verde parpadeante = 2
const uint8_t estados[6][3] PROGMEM = {
    {1, 0, 0}, // S0
    {2, 0, 0}, // S1
    {0, 1, 0}, // S2
    {0, 2, 0}, // S3
    {0, 0, 0}, // S4
    {0, 0, 1}  // S5
};

const uint16_t niveles[3] PROGMEM = {
    1000, 500, 2000};

// Retardo variable
void esperar(uint16_t tiempo)
{
    for (uint16_t j = 0; j < tiempo; j++)
    {
        _delay_ms(1);
    }
}

// Lee los DIP conectados a GND.
// Con pull-up interna: cerrado = 1, abierto = 0.
uint8_t leer_dip(void)
{
    return (~PIND) & 0b00000111;
}

int main(void)
{
    // Configurar salidas
    DDRC |= (1 << PC1) | (1 << PC2) |
            (1 << PC3) | (1 << PC4) |
            (1 << PC5);

    // Configurar entradas
    DDRD &= ~((1 << PD0) | (1 << PD1) | (1 << PD2));

    // Activar resistencias pull-up internas
    PORTD |= (1 << PD0) | (1 << PD1) | (1 << PD2);

    // Apagar todos los LEDs inicialmente
    PORTC &= ~((1 << PC1) | (1 << PC2) |
               (1 << PC3) | (1 << PC4) |
               (1 << PC5));

    uint8_t cont_estado = 0;

    while (1)
    {
        uint8_t dip = leer_dip();
        uint8_t modo = dip & 0b00000011;
        uint8_t peatonal = dip & (1 << PD2);

        // Evitar que el modo 11 avance sin retardo
        if (modo >= 3)
        {
            modo = 0;
        }

        uint16_t tiempo = pgm_read_word(&niveles[modo]);

        uint8_t estado1 =
            pgm_read_byte(&estados[cont_estado][0]);

        uint8_t estado2 =
            pgm_read_byte(&estados[cont_estado][1]);

        uint8_t estado3 =
            pgm_read_byte(&estados[cont_estado][2]);

        // Limpiar las salidas antes de cada estado
        PORTC &= ~((1 << PC1) | (1 << PC2) |
                   (1 << PC3) | (1 << PC4) |
                   (1 << PC5));

        // Semaforo 1
        if (estado1 == 0)
        {
            PORTC |= (1 << PC1);
        }
        else if (estado1 == 1)
        {
            PORTC |= (1 << PC2);
        }

        // Semaforo 2
        if (estado2 == 0)
        {
            PORTC |= (1 << PC3);
        }
        else if (estado2 == 1)
        {
            PORTC |= (1 << PC4);
        }

        // Semaforo peatonal
        if (estado3 == 1 && peatonal)
        {
            PORTC |= (1 << PC5);
        }

        // Parpadeo de verdes
        if (estado1 == 2 || estado2 == 2 || estado3 == 2)
        {
            for (uint16_t t = 0; t < tiempo; t += 250)
            {
                // Encender verde correspondiente
                if (estado1 == 2)
                {
                    PORTC |= (1 << PC2);
                }

                if (estado2 == 2)
                {
                    PORTC |= (1 << PC4);
                }

                if (estado3 == 2  && peatonal )
                {
                    PORTC |= (1 << PC5);
                }

                esperar(125);

                // Apagar verde correspondiente
                if (estado1 == 2)
                {
                    PORTC &= ~(1 << PC2);
                }

                if (estado2 == 2)
                {
                    PORTC &= ~(1 << PC4);
                }

                if (estado3 == 2 && peatonal)
                {
                    PORTC &= ~(1 << PC5);
                }
                esperar(125);
            }
        }
        else
        {
            esperar(tiempo);
        }

        // Avanzar estados
        if (cont_estado == num_estados - 2)
        {
            if (leer_dip() & (1 << PD2))
            {
                cont_estado++;
            }
            else
            {
                cont_estado = 0;
            }
        }
        else if (cont_estado >= num_estados - 1)
        {
            cont_estado = 0;
        }
        else
        {
            cont_estado++;
        }
    }
}
