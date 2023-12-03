//Prueba para los timer e interrupt


#pragma config CPD = OFF, BOREN = OFF, IESO = OFF, DEBUG = OFF, FOSC = HS
#pragma config FCMEN = OFF, MCLRE = ON, WDTE = OFF, CP = OFF, LVP = OFF
#pragma config PWRTE = ON, BOR4V = BOR21V, WRT = OFF

#include <xc.h>
#include <stdio.h>


int ruido_anterior, contador_ruido, contador_sensores, contador_ruido_aux;


void init_timer0()
{
    OPTION_REGbits.T0CS = 0; // Por que usamos el reloj interno del TMR0
    OPTION_REGbits.PSA = 0; // PSA a 0 para que nos acepte el preescalado.
    OPTION_REGbits.PS = 0b111; // ratio 1:256
    TMR0 = 237; //256-19.53  (5*10^6/ 256)*0.001 -> 0,001 porque quiero que el retardo sea de 1 ms, así va contando cada 1 ms los ticks
}


void interrupt_ruido()
{
    contador_ruido++; //me he declarado un contador ruido para que cuando llegue a 10 pueda tratar el ruido y poder posteriormente ponerlo a 0

    /*ruido=medi_ruido();
    ruido_anterior = ruido;*/
    if(contador_ruido==10)
    {
        contador_ruido_aux++;
        ruido = medir_ruido();//no está
        if(ruido<ruido_anterior)
            ruido = ruido_anterior;
        if(contador_ruido_aux==10)
        {
            printf("Timer ok. Enviar valores senosres. Espabila Vanesa...\n");

        }
        contador_ruido = 0;
    }
}

void __interrupt() interruptHandler(void)
{
    TMR0 = 237;
    //ticks++;// esta variable va a ir aumentando infinitamente, no se resetea en ningún momento
    
    contador_sensores++; //me he declarado un contador ruido para que cuando llegue a 5000 pueda tratar los senosres y poder posteriormente ponerlo a 0

    interrupt_ruido();
    
    if(contador_sensores=5000)
    {
        printf("Convertir cosos con el adc");
        
    }
        
    INTCONbits.T0IF = 0;
}

void main(void)
{ OSCCON = 0b00001000; // External cristal

  init_tmr0();
  //contadores a 0 aquí?


  while(1)
    
 }
