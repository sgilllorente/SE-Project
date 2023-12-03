void init_adc(void) //DUDA IMPORTANTE: si solo tenemos un adc y solo te puede llegar por un canal, como hacemos para leer a la vez varios sensores??
{
    ADCON0bits.ADCS = 0b10; //para establecer el periodo de carga del adc que nos piden (1,6 us)
    ADCON0bits.ADON = 1; //habilita el ADC
    //ADCON0bits.CHS = 0b0000; // seleccionamos el canal 0/ pin 0 del adc


    ADCON1bits.ADFM = 1; //de los dos registros, los dos valores menores de la izquierda sean los dos mayores de la derecha
    ADCON1bits.VCFG1 = 0; //para dar tierra al adc
    ADCON1bits.VCFG0 = 0; //para dar tensión al adc

    TRISAbits.TRISA0 = 1; //para configurar el trisa0 como lectura

}

void seleccionar_sensor(int canal)
{
    if(canal == 0)
        ADCON0bits.CHS = 0b0000;
    else if(canal == 1)
        ADCON0bits.CHS = 0b0001;
    else if(canal == 2)
        ADCON0bits.CHS = 0b0010;
}