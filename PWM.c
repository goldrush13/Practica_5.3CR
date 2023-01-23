// MODULO 0, generador 0, 1, 2 PP. 1672
// generador 0 --> PWM0 - PF1(6) - 43 - M0PWM1
// generador 1 --> PWM2 - PF2(6) - 44 - M0PWM2
// generador 2 --> PWM4 - PG0(6) - 49 - M0PWM4

#include "lib/include.h"

extern void Configura_Reg_PWM0(uint16_t freq)
{
    // Habilito PWM0 pg 398 (la tiva grande solo tiene 1)
    SYSCTL->RCGCPWM |= (1<<0); 
    
    // Habilito el puerto G y F (porque usare los pines PG0, PF1 y PF2) pg 383
    SYSCTL->RCGCGPIO |= (1<<6) | (1<<5);
    
    // Habilito función alternativa para PG0, PF1 y PF2 pp. 770
    GPIOG_AHB->AFSEL |= (1<<0);             // 0 por PG0 jeje
    GPIOF_AHB->AFSEL |= (1<<1) | (1<<2);      // 1 Y 2 por PF1 PF2
    
    // Habilito la salida de PG0, PF1, PF2
    GPIOG_AHB->DIR = (1<<0);
    GPIOF_AHB->DIR = (1<<1) | (1<<2);
   
    // Combinación de pg 788 para ver que numero estan en el campo de bits (para PG0 y PF0 es del 0 al 3)
    //                                                                      para PF2 es del 8 al 11
    //                pg 1808 le tengo que poner un 6 porque ahí esta "M0PWM4"
    // Entonces pongo en el campo de bits 0, un 6
    // Para PF0 y PF2 pongo un 6 en el campo de bits 0 y 2  
    GPIOG_AHB->PCTL |= (GPIOG_AHB->PCTL&0xFFFFFFF0) | 0x00000006;
    GPIOF_AHB->PCTL |= (GPIOF_AHB->PCTL&0xFFFFF00F) | 0x00000660;
    
    // Los pines son digitales:
    GPIOG_AHB->DEN |= (1<<0); 
    GPIOF_AHB->DEN |= (1<<1) | (1<<2);
    
    // PASO 5 Activo divisor (USEPWM, PWMDIV)
    PWM0->CC = (1<<8) | (0<<2) | (1<<1) | (0<<0);  /*Enable o Disable Divisor  Pag 1747*/
    
    
    //////////////////// CONFIGURACION PARAMETROS PWM /////////////////
    // BLOQUEO (estoy usando generador 0, 1, 2)
    PWM0->_0_CTL = (0<<0);
    PWM0->_1_CTL = (0<<0);
    PWM0->_2_CTL = (0<<0);
    
    // Voy usar el comparador A y B
    /* Action for counter = 0 --> do nothing
       Action for counter = LOAD --> drive pwmA HIGH    
       pp. 1687 indica el generador del pwm (A o B)*/
    PWM0->_0_GENB = 0x0000080C;         //Registro de las acciones del pwm Pag 1722 (Pte 2, min 30)
    PWM0->_1_GENA = 0x0000008C;
    PWM0->_2_GENA = 0x0000008C;
    
    // VALOR DE CARGAS
    /*          cuentas = fclk/fpwm  
    para 1kHz:  cuentas = (16,000,000/1000)
    para 20MHz: cuentas = (20 000 000/50) = 400 000 cuentas */
    PWM0->_0_LOAD = 50000;
    PWM0->_1_LOAD = 50000;
    PWM0->_2_LOAD = 50000;
    
    // Establecer el valor del ciclo de trabajo
    PWM0->_0_CMPB = 35000;           // aqui estaba mi error
    PWM0->_1_CMPA = 35000;           // ciclo de trabajo 70%, los primeras 3500 cuentas estaran en alto, y despues va a bajar
    PWM0->_2_CMPA = 35000;
     
    // DESBLOQUEO
    PWM0->_0_CTL = (1<<0);
    PWM0->_1_CTL = (1<<0);
    PWM0->_2_CTL = (1<<0);
    
    // Habilito el modulo para que me deje pasar la señal :D
    PWM0->ENABLE = (1<<1) | (1<<2) | (1<<4);          // pp 1687
}