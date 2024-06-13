/*
 * DHT11.c
 *
 * Created: 3/6/2024 12:37:12
 *  Author: LENOVO
 */ 

#include "DHT11.h"


/*
        18ms       40us         80us        80us
—--|           |--------------|                  |--------------|.......datos…..
—-------------PUC—-----|--------------DHT11…………………….|
*/
#include "DHT11.h"

void DHT11_init(void){
	DDRC |= (1<<PINC0); // PIN COMO SALIDA
	PORTC &= ~(1<<PINC0);  // Pongo en 0
	_delay_ms(20); // mantengo la señal en bajo
	PORTC |= (1<<PORTC0); // pongo la señal en 1
	_delay_us(40); // Espero a que responda el sensor
	DDRC &= ~(1<<PINC0); // Pin como entrada
}

// Verificar la respuesta del sensor tras el inicio.
uint8_t DHT11_checkResponse(void) {
	_delay_us(40);
	if (!(PINC & (1<<PINC0))) {
		_delay_us(80);
		if (PINC & (1<<PINC0)) {
			_delay_us(80);
			return 1;
		}
	}
	return 0;
}

// Leer cada byte de datos del sensor
uint8_t DHT11_readByte(void) {
	uint8_t i, byte = 0;
	for (i = 0; i < 8; i++) {
		while (!(PINC & (1<<PINC0))); // Esperar que PINC0 sea 1
		_delay_us(30);
		if (PINC & (1<<PINC0)) {
			byte |= (1 << (7 - i));
		}
		while (PINC & (1<<PINC0)); // Esperar que PINC0 sea 0
	}
	return byte;
}

void DHT11_read(uint8_t *hE, uint8_t *tE)
{
	uint8_t humedadE, humedadD, temperaturaE, temperaturaD, sumaCheck;

	DHT11_init();

	if (DHT11_checkResponse()) {
		humedadE = DHT11_readByte();
		humedadD = DHT11_readByte();
		temperaturaE = DHT11_readByte();
		temperaturaD = DHT11_readByte();
		sumaCheck = DHT11_readByte();

		if (humedadE + humedadD + temperaturaE + temperaturaD == sumaCheck) {
			*hE = humedadE;
			*tE = temperaturaE;
		}
	}
}
