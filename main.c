#include "lib/include.h"

/*Usando el modulo 0 de PWM con una frecuencia de reloj del sistema de 20,000,000 Hz
 * junto con el generador 0,1,2  habilitar alguno de los pwm's asociados y obtener un PWM
 * cuya frecuencia sea de 50Hz , utilizando el uart de la practica 3
 * se enviara dato desde interfaz de simulink para controlar la intensidad luminosa 
 * usando un led RGB externa 
 *
 */

volatile uint8_t red;
volatile uint8_t green;
volatile uint8_t blue;
volatile uint8_t colour;

uint8_t contador;

int main(void)
{
    // VARIABLES ("R" "G" "B")
    red = 114;
    green = 103;
    blue = 98;

    Configurar_PLL(); 
    Configurar_UART0();
    Configura_Reg_PWM0(8);


    while(1) //LED RGB
    {
        colour = readChar();

        switch (colour)
        {
            case 'r':
                while (red == 114)
                {
                    red = readChar();   
                }
                PWM0->_0_CMPA = 50000-((red*25000)/50);
                red = 114;
                break;

            case 'g':
                while (green)
                {
                    green = readChar();   
                }
                PWM0->_2_CMPA = 50000-((green*25000)/50);
                green = 103;
                break;

            case 'b':
                while (blue)
                {
                    blue = readChar();   
                }
                PWM0->_1_CMPA = 50000-((blue*25000)/50);
                blue = 98;
                break;
        }
    }
}
