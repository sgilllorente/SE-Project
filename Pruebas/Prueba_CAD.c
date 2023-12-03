void init_adc(void) //DUDA IMPORTANTE: si solo tenemos un adc y solo te puede llegar por un canal, como hacemos para leer a la vez varios sensores??
{
    //te debria de llegar el canal que quieres hacer
    ADCON0bits.ADCS = 0b10; //para establecer el periodo de carga del adc que nos piden (1,6 us)
    ADCON0bits.ADON = 1; //habilita el ADC
    ADCON0bits.CHS = 0b0000; // seleccionamos el canal 0/ pin 0 del adc
    ADCON0bits.CHS = 0b0001; // seleccionamos el canal 1/ pin 1 del adc
    ADCON0bits.CHS = 0b0010; // seleccionamos el canal 2/ pin 2 del adc


    ADCON1bits.ADFM = 1; //de los dos registros, los dos valores menores de la izquierda sean los dos mayores de la derecha
    ADCON1bits.VCFG1 = 0; //para dar tierra al adc
    ADCON1bits.VCFG0 = 0; //para dar tensión al adc

    TRISAbits.TRISA0 = 1; //para configurar el trisa0 como lectura
    TRISAbits.TRISA1 = 1; //para configurar el trisa0 como lectura
    TRISAbits.TRISA2 = 1; //para configurar el trisa0 como lectura

}

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

void putch(char data)
{
    while(!TXIF) //chekea si el bufffer esta ocupado
        continue;
    TXREG=data;
}

void init_tmr0()
{
    OPTION_REGbits.T0CS = 0; // inicialia el timer 0 en el programa. If 1 se asigna al watchdog
    OPTION_REGbits.PSA = 0; // asigna el preescalador al timer 0, es decir, se usara para divir la frcuencia del reloj If 1 no se asigna y por lo tanto no trabaja
    OPTION_REGbits.PS = 0B111; // establece el preescalador a su maximo valor (1:256)
    TMR0 = 101;
}


void __interrupt() int_routine(void)
{

      TMR0 = 237; 
      if(cont == 10)
      {
            ADCON0bits.CHS = 0b0000; // seleccionamos el canal 0/ pin 0 del adc
            ADCON0bits.GO = 1; //con este bit se inicializa el go. Para sabercuando termina la conversion
            while (ADCON0bits.GO);  //para detectar el fin de la conversion
            reg = (ADRESH << 8) + ADRESL; // para desplazar el array high 6 posiciones a la izquierda
            printf("%d\r\n", reg);
            PORTB = ADRESL;
            cont = 0;
      
      }
      else if(cont == 5000)
      {
	    ADCON0bits.CHS = 0b0001; // seleccionamos el canal 0/ pin 0 del adc
            ADCON0bits.GO = 1; //con este bit se inicializa el go. Para sabercuando termina la conversion
            while (ADCON0bits.GO);  //para detectar el fin de la conversion
            reg = (ADRESH << 8) + ADRESL; // para desplazar el array high 6 posiciones a la izquierda
            printf("%d\r\n", reg);
            PORTB = ADRESL;
            cont = 0;
      }
      else if(cont == 5100)
      {
	    ADCON0bits.CHS = 0b0010; // seleccionamos el canal 0/ pin 0 del adc
            ADCON0bits.GO = 1; //con este bit se inicializa el go. Para sabercuando termina la conversion
            while (ADCON0bits.GO);  //para detectar el fin de la conversion
            reg = (ADRESH << 8) + ADRESL; // para desplazar el array high 6 posiciones a la izquierda
            printf("%d\r\n", reg);
            PORTB = ADRESL;
            cont = 0;
      }

      INTCONbits.T0IF = 0; //Restablece la bandera del timer a 0
      cont += 1;
}

void main(void)
{ 
  OSCCON = 0b00001000; // External cristal
  init_uart();
  init_adc();
  init_tmr0();

  INTCONbits.GIE = 1;
  INTCONbits.T0IE = 1;
  TRISB = 0;  // Para activar la salida por los leds tambien)



  while(1);

}
