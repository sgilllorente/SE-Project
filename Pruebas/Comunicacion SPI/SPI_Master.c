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
#include <stdio.h>

#include "spi.h"                            // Libreria del protocolo SPI
#include "lcd.h"                            // Libreria de la pantalla LCD

#define SS_DIR TRISBbits.RB2                // Pin SS
#define SS_PIN LATBbits.LB2

char buffer[20];
unsigned char dato_1;
unsigned char dato_2;
unsigned char SPI_Read_Slave(unsigned char reg);

void main()
{
    ADCON1bits.PCFG = 0x0F;                 // Coloca todos los pines como digitales
    SS_DIR = 0;                             // Pin SS como salida
    SPI_Init_Master(SPI_CLK_DIV_16, MODE_1, HALF_DATA);
    SS_PIN = 1;                             // Inicialmente deshabilita al esclavo
    Lcd_Init();                             // Inicializa la pantalla lcd
    
    while (1)
    {
        dato_1 = SPI_Read_Slave(0xA0);      // Lee el dato del ADC
        dato_2 = SPI_Read_Slave(0xB0);      // Lee el dato del pulsador
        
        Lcd_Set_Cursor(1,1);
        sprintf(buffer, "ADC: %u  ", dato_1);
        Lcd_Write_String(buffer);
        Lcd_Set_Cursor(2,1);
        sprintf(buffer, "Boton: %u  ", dato_2);
        Lcd_Write_String(buffer);
    }
}

unsigned char SPI_Read_Slave(unsigned char reg)
{
    SS_PIN = 0;                             // Habilita al esclavo
    SPI_Write(reg);                         // Envia la direccion del registro a leer del esclavo
    SS_PIN = 1;                             // Deshabilita al esclavo
    __delay_us(100);
    
    SS_PIN = 0;                             // Habilita al esclavo
    unsigned char dato_rx = SPI_Read();     // Lee el dato del esclavo
    SS_PIN = 1;                             // Deshabilita al esclavo
    return dato_rx;
}