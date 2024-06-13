/*
 * DHT11.h
 *
 * Created: 3/6/2024 12:37:27
 *  Author: LENOVO
 */ 


#ifndef DHT11_H_
#define DHT11_H_

#define F_CPU 16000000UL // Frecuencia de reloj del MCU en 16MHz
#include <avr/io.h>
#include <util/delay.h>

void DHT11_init(void);
uint8_t DHT11_checkResponse(void);
uint8_t DHT11_readByte(void);
void DHT11_read(uint8_t *hE, uint8_t *tE);



#endif /* DHT11_H_ */