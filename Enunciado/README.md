# Importante
Hay que leerse bien tanto el documento de Especificaciones como el documento de Requisitos. Si surgen dudas, se anotan.

# Anotaciones Enunciado
En esta sección se comentarán aspectos relevantes del [enunciado](https://github.com/sgilllorente/SE-Project/blob/main/Enunciado/requisitos-v-0-2.pdf)).
## 2. Objetivo
Los objetivos generales son:
  1. Monitorizar 5 parámetros: C02, temperatura, humedad, luminosidad y ruido ambiente.
  2. Permitir ajustar: el color de la luz (RGB) y la luminosidad
## 2.1 Componentes destacados del prototipo
  1. Soporte mecánico -> Ya está hecho por los de industriales.
  2. Circuito de acondicionamiento -> Ya está hecho por Norberto (q grande).
  3. Sistema programable -> Lo tenemos que hacer.
## 2.2 Componentes destacados del prototipo
  * Especificación
    - Se suministrará un documento de especificación de [requisitos](https://github.com/sgilllorente/SE-Project/blob/main/Enunciado/requisitos-v-0-2.pdf). Leer con mucho detalle. Se deberá aclarar y completar con el profesor.
    - Previsión de esfuerzo.
    - Coste asociado. El presupuesto debe incluir las horas de ingeniero. El coste material en este proyecto es muy poco en comparación con las horas de trabajo.
  * Diseño
    - Para el diseño hardware iremos muy acopmpañados, sobre todo en el laboratorio.
    - Para el software se hará un diseño de alto nivel con SysML y un diseño detallado. El programador necesitará un diseño detallado que no le genere ninguna duda. Este diseño debe ser superdetallado. Recomendados los diagramas de estado de SysML para realizar diagramas detallados.
    - Para las trazas se recomienda hacer tablas para cada una.
  * Pruebas
    - Tener lista una batería de pruebas para cuando el Sistema Empotrado esté construido.
    - Se recomienda el uso de pruebas de Caja Negra de tal manera que a partir de los interfaces/bloques podamos hacer pruebas.
## 3. Entragas a realizar
  1. Especificación y diseño.
  2. Cosntrucción y pruebas. Interesa comentar todos los problemas que han surgido. Sobre todo cómo hemos activado la generación de soluciones frente a esos problemas.

# Anotaciones Requisitos
En esta sección se comentarán aspectos relevantes y dudas de la especificación de [requisitos](https://github.com/sgilllorente/SE-Project/blob/main/Enunciado/requisitos-v-0-2.pdf).
## 2.2 Perspectiva del producto
  Tenemos un componente (SMA) que es el que tenemos que hacer, este circuito irá conectado a otro (CA) que es el encargado de realizar las actividades de acondicionamiento para sensores y actuadores.
## 2.3.1 Interfaz de componente CA 
  - I2C: lo más importante es la velocidad de funcionamiento estándar. En los requisitos nos piden que sea de 100 KHz.
  - Bus SPI: Nos da Norberto una función que imita el funcionamiento del bus SPI.
## 2.3.1 Interfaz computador personal
La conexión será por UART, igual que hemos hecho en las prácticas.
## 3. Requisitos específicos 
  - ENC-30: Se deberá escribir en la memoria eeprom para poder guardar el últimos estado de la lámpara. Luz blanca = códgios RGB todos iguales.
  - AC-10: 3 char de 255 valores (Rojo, Verde, Azul).
  - AC-29: El ventilador lo vamos a controlar con una señal PWM. 0% -> ciclo de trabajo del 0%; 100 -> ciclo de trabajo del 100%. Para valores itermedios hacemos una regla de 3.
