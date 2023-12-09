#include <16f877a.h>
#device ADC = 10
#fuses HS,NOWDT,NOPROTECT,NOPUT,NOLVP,BROWNOUT
#use delay(clock=20M)
#use I2C(SLAVE, SDA=PIN_C4, SCL=PIN_C3, ADDRESS=0xA0, FAST, FORCE_HW)      // Configurcion del bus I2C
#use standard_io(D)

#include <map_function.c>                                // Libreria para la funcion map

int estado;                                              // Variable de cambio de estado
int8 dato = 0;
int8 address, buffer[0x04];                              // Direccion y array de Bytes

#INT_SSP                                                 // Interrupcion por I2C
void interrupcion_i2c()
{
   estado = i2c_isr_state();                             // Lee el cambio de estado del bus I2C
   if(estado < 0x80){                                    // El maestro esta enviando datos
      if(estado == 0){                                   // Se recibe el dato
         dato = i2c_read();                              // Almacena el dato recibido por I2C
      }
      if(estado == 1){                                   // El primer byte es la direccion 
         address = i2c_read();                           // Almacena la direccion
      }
      if(estado == 2){                                   // El segundo byte es el dato
         buffer[address] = i2c_read();                   // Almacena la direccion de datos
      }
   }
   
   if(estado == 0x80){                                   // El maestro solicita datos del esclavo
      i2c_write(buffer[address]);                        // Devuelve el dato de la direccion solicitada por el maestro
   }
}

void main()
{
   setup_adc_ports(AN0);                                 // Configuracion de ADC
   setup_adc(adc_clock_internal);
   enable_interrupts(INT_SSP);                           // Habilita la interrupcion por I2C
   enable_interrupts(GLOBAL);                            // Habilita las interrupciones globales
   output_low(PIN_D0);                                   // Inicializa el pin D0 en estado bajo
   
   while(true)
   {
      set_adc_channel(0);
      delay_us(2);
      long valor_adc = read_adc();                       // Lectura del canal 0 del ADC
      int valor_pwm = map(valor_adc, 0, 1023, 0, 255);   // Conversion de los valores del ADC en un rango de 0 a 255
      buffer[0x01] = valor_pwm;                          // Escribe el valor del pwm en la direccion de memoria 0x01
      
      int estado_led = address;                          // Almacena la direccion de memoria enviada por el maestro
      if(estado_led == 0x02)                             // Si la direccion es 0x02
      {
         output_high(PIN_D0);                            // Enciende el led
      }
      if(estado_led == 0x03)                             // Si la direccion es 0x03
      {
         output_low(PIN_D0);                             // Apaga el led
      }
      delay_ms(50);
   }
}
