// Prueba PWM que 


#pragma config CPD = OFF, BOREN = OFF, IESO = OFF, DEBUG = OFF, FOSC = HS
#pragma config FCMEN = OFF, MCLRE = ON, WDTE = OFF, CP = OFF, LVP = OFF
#pragma config PWRTE = ON, BOR4V = BOR21V, WRT = OFF

#include <xc.h>
#include <stdio.h>

int cont;
int inversa;



void init_tmr2()
{
    T2CON = 0; 
    T2CONbits.T2CKPS1 = 0; // Prescalado a 1 del timer 2 primer bit
    T2CONbits.T2CKPS0 = 0; // Prescalado a 1 del timer 2 segundo bit
    
    T2CONbits.TMR2ON = 1; // 1 para encender el timer 2    
    T2CONbits.TOUTPS = 0b0000; // establece el postescalado a su mínimo valor (1:1)
    PIE1bits.TMR2IE = 1;   //Habilita las interruciones del Timer2
    
    
    
}

void init_ccp()
{
    //CCP1CONbits.P1M = 0b00;     // Establece el pin A como salida
    //CCP1CONbits.CCP1M = 0b1111; // pin A,B,C,D a 0 (low) y modo pwm
    PR2 = 166;                  //Valor 'PWM period' sacado de la pág 131
    CCP2CONbits.CCP2M = 0b1100;  // pin A y C a 1, los demás a 0. y modo pwm
    
    //CCPR1L = 0;   //Inicializo ciclo trabajo a 0
    CCPR2L = 166; //Inicializo ciclo trabajo al 100% que sería 166
    TRISC2 = 0;   //Port C2 as output // son dos campos distintos porque tenemos dos señales pwm, se configuran como salida
    //TRISC1 = 0;   //Port C1 as output
}


  void __interrupt() int_routine(void)
  {

      cont += 1;
    if(cont == 1500) // como se pide 30KHz, 1500/50ms = 30, asi se suma,resta,actualiza lo pedido con una frecuencia de 30KHz, reglas de 3 50ms = 0,02KHz; 75000ms = 30KHz
    {
        cont = 0;
        if(inversa == 0)
            CCPR2L += 1;
        
        else
            CCPR2L -= 1;
        
        if(CCPR2L > 166)
            inversa = 1;
        
        if(CCPR2L == 0)
            inversa = 0;
        
    }
  }

void main(void)
{ OSCCON = 0b00001000; // External cristal
 
  
  init_tmr2();
  init_ccp();
 
  cont = 0;
  inversa = 0;
 
  INTCONbits.GIE = 1; //Habilita interrupciones de periféricos
  INTCONbits.T0IE = 1; //Habilita interrupciones generales
  INTCONbits.PEIE = 1;   //Interrupciones de periféricos



  while(1);

}