/*
 * dht11.h
 */ 
#ifndef DHT11_H_
#define DHT11_H_
#include <avr/io.h>
#include <util/delay.h>

void DHT11_init(void);
uint8_t DHT11_read(uint8_t *dht_temperatura, uint8_t *dht_humedad);

#endif /* DHT11_H_ */