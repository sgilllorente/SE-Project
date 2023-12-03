#pragma config CPD = OFF, BOREN = OFF, IESO = OFF, DEBUG = OFF, FOSC = HS
#pragma config FCMEN = OFF, MCLRE = ON, WDTE = OFF, CP = OFF, LVP = OFF
#pragma config PWRTE = ON, BOR4V = BOR21V, WRT = OFF
#pragma intrinsic(_delay)

#include <xc.h>
#include <stdio.h>

#define _XTAL_FREQ 20000000 // necessary for __delay_us

//int ticks = 0;
int ticks;

//variables ambientales
int ruido, co2, humedad, temperatura, luminosidad;
//variables mecánicas
int led_R, led_G, led_B, velocidad_ventilador;

// variables auxiliares;
int ruido_anterior, contador_ruido, contador_sensores, contador_ruido_aux;


int medir_ruido();
void enviar_usart();
int cambiar_sensores_pantalla();
void mostrar_sensores();
void interfaz_usuario();
void modificar_sensores(int *led_R, int *led_G, int *led_B, int *luminosidad, int *velocidad_ventilador);

void init_uart(void)
{  
  TXSTAbits.BRGH = 0;
  BAUDCTLbits.BRG16 = 0;//bit de la documentación pag 157

  // SPBRGH:SPBRG = 
  SPBRGH = 0; //velocidad low
  SPBRG = 32;  // 9600 baud rate with 20MHz Clock
  
  TXSTAbits.SYNC = 0; /* Asynchronous */
  TXSTAbits.TX9 = 0; /* TX 8 data bit */ //solo 8 bits
  RCSTAbits.RX9 = 0; /* RX 8 data bit */

  PIE1bits.TXIE = 0; /* Disable TX interrupt */ //no hace falta activar las interrupciones porque solo miramos el registro
  PIE1bits.RCIE = 0; /* Disable RX interrupt */

  RCSTAbits.SPEN = 1; /* Serial port enable */ //para indicar que las dos patitas es paracomunicación

  TXSTAbits.TXEN = 0; /* Reset transmitter */ //activa el hdw de transmision, se envia por las patas lo de este reg
  TXSTAbits.TXEN = 1; /* Enable transmitter */
  
 }

 void init_timer0()
{
    OPTION_REGbits.T0CS = 0; // Por que usamos el reloj interno del TMR0
    OPTION_REGbits.PSA = 0; // PSA a 0 para que nos acepte el preescalado.
    OPTION_REGbits.PS = 0b111; // ratio 1:256
    TMR0 = 237; //256-19.53  (5*10^6/ 256)*0.001 -> 0,001 porque quiero que el retardo sea de 1 ms, así va contando cada 1 ms los ticks
}

void __interrupt() interruptHandler(void)
{
    TMR0 = 159;
    ticks++;// esta variable va a ir aumentando infinitamente, no se resetea en ningún momento
    contador_ruido++; //me he declarado un contador ruido para que cuando llegue a 10 pueda tratar el ruido y poder posteriormente ponerlo a 0
    contador_sensores++; //me he declarado un contador ruido para que cuando llegue a 5000 pueda tratar los senosres y poder posteriormente ponerlo a 0

    /*ruido=medi_ruido();
    ruido_anterior = ruido;*/
    if(contador_ruido==10)
        contador_ruido_aux++;
        ruido = medir_ruido();
        if(ruido<ruido_anterior)
            ruido = ruido_anterior;
        if(contador_ruido_aux==10)
        {
            if(ruido<=0 && ruido<=400)
	            printf("Ruido bajo: %d", ruido);
            else if (ruido<=401 && ruido<=900)
	            printf("Ruido intermedio: %d", ruido);
            else 
                printf("Ruido alto: %d", ruido);
            
            enviar_usart(ruido);

        }
        contador_ruido = 0;
    if(contador_sensores=5000)
    {
        int adc = conversion_adc();
        //aquí hay que ver qué señal ha convertido el adc, si co2, humedad, temperatura o luminosidad

        //enviar_usart(señal de adc tratada);
        
    }
        
    INTCONbits.T0IF = 0;
}
int cambiar_sensores_pantalla()
{
    int respuesta;

    printf("Quieres modificar el color o la intensidad de la lámapara? (pulsa "1" para Sí, pulsa "0" para No)");
    scanf("%d", &respuesta);

    return respuesta;
}
void mostrar_sensores()
{
    printf("- Nivel de ruido: %d", ruido);
    printf("\n");
    printf("- Nivel de CO2: %d", co2);
    printf("\n");
    printf("- Huemdad realtiva: %d", humedad);
    printf("\n");
    printf("- Temperatura: %d", ruido);
    printf("\n");
    printf("- Luminosidad: %d", ruido);
    printf("\n\n");
    
}

void modificar_sensores(int *led_R, int *led_G, int *led_B, int *luminosidad, int *velocidad_ventilador)
{
    printf("Introduzca el nuevo valor del color R del Led (color actual = %d): ", *led_R);
    scanf("%d", led_R);
    printf("\n");
    printf("Introduzca el nuevo valor del color G del Led (color actual = %d): ", *led_G);
    scanf("%d", led_G);
    printf("\n");
    printf("Introduzca el nuevo valor del color B del Led (color actual = %d): ", *led_B);
    scanf("%d", led_B);
    printf("\n");
    printf("Introduzca el nuevo valor de luminosidad (valor actual = %d): ", *luminosidad);
    scanf("%d", luminosidad);
    printf("\n");
    printf("Introduzca el nuevo valor de velocidad del ventilador (valor actual = %d): ", *velocidad_ventilador);
    scanf("%d", velocidad_ventilador);
    printf("\n\n");
}

//**********LAMP************

//la he puesto como función para facilitarme la comprensión, esto habrá que meterlo en el main
void interfaz_usuario()
{
    mostrar_sensores();
    if(cambiar_sensores_pantalla())
        modificar_sensores(int *led_R, int *led_G, int *led_B, int *luminosidad, int *velocidad_ventilador);
        enviar_usart(todos los sensores); //aquí habría que ver de enviar lo que hemos modificado
    
}


void main(void)
{ OSCCON = 0b00001000; // External cristal
  init_uart();

  ticks = 0;


  while(1)
    
 }

 