// MODULO 0, generador 0, 1, 2 PP. 1672
// generador 0 --> PWM0 - PF0(6) - 42 - M0PWM0
// generador 1 --> PWM2 - PF2(6) - 44 - M0PWM2
// generador 2 --> PWM4 - PG0(6) - 49 - M0PWM4

#include "lib/include.h"

extern void Configura_Reg_PWM0(uint16_t freq)
{
    // Habilito PWM0 pg 398 (la tiva grande solo tiene 1)
    SYSCTL->RCGCPWM |= (1<<0); 
    
    // Habilito el puerto G y F (porque usare los pines PG0, PF0 y PF2) pg 383
    SYSCTL->RCGCGPIO |= (1<<6) | (1<<5);

    // Habilito función alternativa para PG0, PF0 y PF2 pp. 770
    GPIOG_AHB->AFSEL |= (1<<0);                 // 0 por PG0 jeje
    GPIOF_AHB->AFSEL |= (1<<0) | (1<<2);        // 0 Y 2 por PF0 PF2

    // Habilito la salida de PG0, PF0, PF2
    GPIOF_AHB->DIR = (1<<0) | (1<<2);
    GPIOG_AHB->DIR = (1<<0);

    // Combinación de pg 788 para ver que numero estan en el campo de bits (para PG0 y PF0 es del 0 al 3)
    //                                                                      para PF2 es del 8 al 11
    //                pg 1808 le tengo que poner un 6 porque ahí esta "M0PWM4"
    // Entonces pongo en el campo de bits 0, un 6
    // Para PF0 y PF2 pongo un 6 en el campo de bits 0 y 2  
    GPIOG_AHB->PCTL |= (GPIOG_AHB->PCTL&0xFFFFFFF0) | 0x00000006;   // se pone 0 en la posición 0, se pone 6 en la posición 0
    GPIOF_AHB->PCTL |= (GPIOF_AHB->PCTL&0xFFFFF0F0) | 0X00000606;   // se pone 0 en las posiciones 0 y 2, se pone 6 en la posición 0 y 2
    
    // Los pines son digitales:
    GPIOG_AHB->DEN |= (1<<0); 
    GPIOF_AHB->DEN |= (1<<0) | (1<<2);
    
    // PASO 5 Activo divisor 
    PWM0->CC &= ~(1<<20);  /*Enable o Disable Divisor  Pag 1747*/
    
    //////////////////// CONFIGURACION PARAMETROS PWM /////////////////
    
    // BLOQUEO (estoy usando generador 0,1 y 2)
        PWM0->_0_CTL = (0<<0);
        PWM0->_1_CTL = (0<<0);
        PWM0->_2_CTL = (0<<0);
        
    // Voy usar el comparador A
    /* Action for counter = 0 --> do nothing
       Action for counter = LOAD --> drive pwmA HIGH    
       pp. 1686 indica el generador del pwm (A o B)*/
    PWM0->_0_GENA = 0x0000008C;
    PWM0->_1_GENA = 0x0000008C;
    PWM0->_2_GENA = 0x0000008C;         // Registro de las acciones del pwm Pag 1722 (Pte 2, min 30)
    
    // VALOR DE CARGAS
    /*          cuentas = fclk/fpwm  
    para 1kHz:  cuentas = (16,000,000/1000)
    para 20MHz: cuentas = (20 000 000/50) = 400 000 cuentas */
    //PWM0->_0_LOAD = ???; 
    //PWM0->_1_LOAD = ???;   
    //PWM0->_2_LOAD = ???;

    // Establecer el valor del ciclo de trabajo
    //PWM0->_0_CMPA = ?;
    //PWM0->_1_CMPA = ?;
    //PWM0->_2_CMPA = ?;           // ciclo de trabajo ??, los primeras ???? cuentas estaran en alto, y despues va a bajar

    // DESBLOQUEO
    PWM0->_0_CTL = (1<<0);
    PWM0->_1_CTL = (1<<0);
    PWM0->_2_CTL = (1<<0);

    // Habilito el modulo para que me deje pasar la señal :D
    PWM0->ENABLE = (1<<0) | (1<<2) | (1<<4);          // pp 1687
}
