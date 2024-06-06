/*
 * dht11.c
 *
 * Created: 29/5/2024 20:02:01
 *  Author: Notebook
 */ 


#include "dht11.h"
#define DHT_DDR DDRD
#define DHT_PORT PORTD
#define DHT_PIN PIND
#define PIN 2
void DHT11_init(void){
	DHT_DDR |= (1<<PIN); // PIN COMO SALIDA
	DHT_PORT   |= (1<<PIN);  // NIVEL ALTO
}
/*
        18ms       40us         80us        80us
�--|           |--------------|                  |--------------|.......datos�..
�-------------PUC�-----|--------------DHT11��������.|
*/

// Si devuelve 1 es valido el dato, si devuelve 0 hay un problema con el DHT11
uint8_t DHT11_read(uint8_t *dht_temperatura, uint8_t *dht_humedad)
{
	int bits[5];
	int i,j=0;
	int contador=0;

// paso 1, enviar un pulso en bajo durante 18 ms
	DHT_PORT &= ~(1<<PIN); // nivel bajo
	_delay_ms(18);
	DHT_PORT   |= (1<<PIN);  // NIVEL ALTO
	DHT_DDR &= ~((1<<PIN)); //Pin como entrada

//Paso 2 esperamos 20 a 40us hasta que el dht11 envie 0
	contador=0;
	while(DHT_PIN & (1<<PIN)) // mientras este en alto
	{
		_delay_us(2);
		contador += 2; // incremento cada dos microsegundos
		if(contador > 60){ // si pasaron mas de 60 microsegundos
				DHT_DDR |= (1<<PIN); // Pin como salida
				DHT_PORT |= (1<<PIN); // Nivel alto
				return 0; // hay un problema con el dht11
		}
	}
//paso 3 espero 80 us hasta que el dht11 envie 1
	contador = 0;
	while (!(DHT_PIN & (1<<PIN))) //mientras este en bajo
	{
		_delay_us(2);
		contador += 2; // incremento cada dos microsegundos
		if(contador > 100){ // si pasaron mas de 100 microsegundos
			DHT_DDR |= (1<<PIN); // Pin como salida
				DHT_PORT |= (1<<PIN); // Nivel alto
				return 0; // hay un problema con el dht11
		}
	} 
// paso 4 esperamos 80us hasta que el dht11 envie 0
	contador = 0;
	while(DHT_PIN & (1<<PIN))
	{
		_delay_us(2);
		contador += 2; // incremento cada dos microsegundos
		if(contador > 100){ // si pasaron mas de 100 microsegundos
			DHT_DDR |= (1<<PIN); // Pin como salida
			DHT_PORT |= (1<<PIN); // Nivel alto
			return 0; // hay un problema con el dht11
		}
	}

// paso 5 leemos los 40 bits o 5 bytes los dos primero bytes, corresponde a la humedad 3 y 4 a la temperatura
	for (j=0; j<5; j++)
	{
		uint8_t result=0; // Inicializar la variable de resultado
		for(i=0; i<8;i++){  // Leer 8 bits de datos del sensor DHT11
			while(!(DHT_PIN & (1<<PIN)))
			_delay_us(35); // Retraso para esperar el inicio de la transmisi�n
			if(DHT_PIN & (1<<PIN))   // Esperar a que el pin del sensor est� alto (datos)
				result |= (1<<(7-i)); // Agregar el bit al resultado
			while((DHT_PIN & (1<<PIN))); // Esperar hasta que el bit haya sido completamente transmitido
			}
			bits[j] = result;  // Almacenar el resultado de la lectura de 8 bits
		}
		DHT_DDR |= (1<<PIN); // Configurar el pin del sensor DHT11 como salida
		DHT_PORT |= (1<<PIN); // Establecer el nivel l�gico alto en el pin del sensor DHT11

// paso 6 convertir la temperatura y humedad

	if((uint8_t) (bits[0]+bits[1]+bits[2]+bits[3]) == bits[4]) // Pregunto si es vlaido el dato que llego
	{
		uint16_t rawhumedad = bits[0]<<8 | bits[1];  // Combinar los primeros 8 bits en un valor de 16 bits
		uint16_t rawtemperatura = bits[2]<<8 | bits[3]; // Combinar los siguientes 8 bits en un valor de 16 bits
		*dht_temperatura = (uint8_t)(rawtemperatura / 10);  // Convertir la temperatura y truncarla a un entero de 8 bits
		*dht_humedad = (uint8_t)(rawhumedad / 10);  // Convertir la humedad y truncarla a un entero de 8 bits

		return 1;
	}
 
	return 1;
}
