// Usando el módulo 0 de PWM con una frecuencia de reloj del sistema de 20,000,000 Hz. Junto con el generador 0,1,2 habilitar alguno de los pwm's asociados y obtener un PWM cuya frecuencia sea de 50Hz
#include "lib/include.h"

/*Usando el modulo 0 de PWM con una frecuencia de reloj del sistema de 20,000,000 Hz
 * junto con el generador 0,1,2  habilitar alguno de los pwm's asociados y obtener un PWM
 * cuya frecuencia sea de 50Hz , utilizando el uart de la practica 3
 * se enviara dato desde interfaz de simulink para controlar la intensidad luminosa 
 * usando un led RGB externa 
 *
 */

uint16_t red;
uint16_t green;
uint16_t blue;
uint16_t colour;
uint16_t contador;

int main(void)
{
    // VARIABLES
    red = 2;
    green = 3;
    blue = 4;

    Configurar_PLL(); 
    Configurar_UART0();
    Configura_Reg_PWM0(8);


    while(1) //LED RGB
    {
        switch (colour)
        {
            case 'r':
                while (red)
                {
                    red = readChar;   
                }
                PWM0->_1_CMPA = 20000-((red*10000)/50);
                red = 2;
                break;

            case 'g':
                while (green)
                {
                    green = readChar;   
                }
                PWM0->_1_CMPB = 20000-((green*10000)/50);
                green = 3;
                break;

            case 'b':
                while (blue)
                {
                    blue = readChar;   
                }
                PWM0->_1_CMPA = 20000-((blue*10000)/50);
                blue = 4;
                break;
        }
    }
}
