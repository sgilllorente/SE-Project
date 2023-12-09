#include <16f877a.h>
#fuses HS,NOWDT,NOPROTECT,NOPUT,NOLVP,BROWNOUT
#use delay(clock=20M)
#use I2C(MASTER, SDA=PIN_C4, SCL=PIN_C3, FAST, FORCE_HW)       // Configuracion del bus I2C
#use standard_io(D)

void main()
{
   setup_timer_2(t2_div_by_16, 255, 1);               // Configuracion del timer 2
   setup_ccp1(ccp_pwm);                               // Configuracion del CCP1 en modo pwm
   set_pwm1_duty(0);                                  // Inicializa el pwm en 0
   
   while(true)
   {
      i2c_start();                                    // Iniciar comunicacion
      i2c_write(0xA0);                                // Direccion del esclavo
      i2c_write(0x01);                                // Direccion de memoria que se desea leer
      i2c_stop();                                     // Detener comunicacion
      
      i2c_start();                                    // Iniciar comunicacion
      i2c_write(0xA1);                                // Direccion del esclavo para modo lectura
      int dato_rcv = i2c_read(0);                     // Almacena el dato recibido del esclavo
      i2c_stop();                                     // Detener comunicacion
      delay_ms(10);
      set_pwm1_duty(dato_rcv);                        // Escribe el valor del pwm que viene del esclavo
      
      if(input(PIN_D0) == 1)                          // Si se presiona el pulsador 1, envia el dato 0x02
      {
         while(input(PIN_D0) == 1);
         delay_ms(10);
         
         i2c_start();                                 // Iniciar comunicacion
         i2c_write(0xA0);                             // Direccion del esclavo
         i2c_write(0x02);                             // Escribe datos en el esclavo
         i2c_stop();                                  // Detener comunicacion
         delay_ms(100);                               // Retardo
      }
      
      if(input(PIN_D1) == 1)                          // Si se presiona el pulsador 2, envia el dato 0x03
      {
         while(input(PIN_D1) == 1);
         delay_ms(10);
         
         i2c_start();                                 // Iniciar comunicacion
         i2c_write(0xA0);                             // Direccion del esclavo
         i2c_write(0x03);                             // Escribe datos en el esclavo
         i2c_stop();                                  // Detener comunicacion
         delay_ms(100);                               // Retardo
      }
   }
}
