#pragma config PLLDIV = 2, CPUDIV = OSC1_PLL2, USBDIV = 2
#pragma config FOSC = HSPLL_HS, FCMEN = OFF, IESO = OFF
#pragma config PWRT = OFF, BOR = OFF, BORV = 3, VREGEN = OFF
#pragma config WDT = OFF
#pragma config WDTPS = 32768
#pragma config CCP2MX = ON, PBADEN = OFF, LPT1OSC = OFF, MCLRE = ON
#pragma config STVREN = ON, LVP = OFF, ICPRT = OFF, XINST = OFF
#pragma config CP0 = OFF, CP1 = OFF, CP2 = OFF, CP3 = OFF
#pragma config CPB = OFF, CPD = OFF
#pragma config WRT0 = OFF, WRT1 = OFF, WRT2 = OFF, WRT3 = OFF
#pragma config WRTC = OFF, WRTB = OFF, WRTD = OFF
#pragma config EBTR0 = OFF, EBTR1 = OFF, EBTR2 = OFF, EBTR3 = OFF
#pragma config EBTRB = OFF

#define _XTAL_FREQ 48000000
#include <xc.h>

#include "spi.h"                                    // Libreria del protocolo SPI

unsigned char dato_rx = 0;
unsigned char dato_adc;
short dato_boton;

void main() 
{
    ADCON1 = 0x0E;                                  // Vref = VSS y GND, Configuracion de entradas analogicas
    ADCON0 = 0x00;                                  // Seleccion del canal, Habilitación del conversor
    ADCON2 = 0x0F;                                  // Tiempo de adquisicion, Justificacion hacia la izquierda
    ADCON0bits.ADON = 1;                            // Enciende el conversor
    TRISDbits.RD0 = 1;                              // Pin RD0 como entrada
    SPI_Init_Slave(SPI_SLAVE_PIN, MODE_1);
    
    while(1)
    { 
        dato_rx = SPI_Read();                       // Lee el dato del maestro
        switch(dato_rx)                             // Compara el dato recibido
        {
            case 0xA0:                              // Si llego 0xA0, maestro solicita el dato del adc
                ADCON0bits.GO_DONE = 1;             // Inicia la conversion
                while(ADCON0bits.GO_DONE == 1);     // Espera a que termine la conversion
                dato_adc = ADRESH;                  // Almacena el dato de 8 bits del adc
                SSPBUF = dato_adc;                  // Envia el dato del adc
                break;
                
            case 0xB0:                              // Si llego 0xB0, maestro solicita el dato del pulsador
                dato_boton = PORTDbits.RD0;         // Lectura del pulsador
                SSPBUF = dato_boton;                // Envia del dato del pulsador
                break;
        }
    }
}