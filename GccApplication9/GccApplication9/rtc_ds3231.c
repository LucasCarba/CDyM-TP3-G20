/*
 * rtc_ds3231.c
 *
 * Created: 29/5/2024 20:02:22
 *  Author: Notebook
 */ 

#include <avr/io.h>
#include <util/twi.h>
#include "rtc_ds3231.h"
#include <util/twi.h>

#define DS3231_ADDRESS 0x68 //especificar la dirección I2C del módulo RTC DS3231

void TWI_Init(void) {
	TWSR = 0x00;  // Prescaler de TWI = 1
	TWBR = 72;    // Configurar SCL a 100 kHz con F_CPU = 16 MHz
	TWCR = 0x04;  // Habilitar TWI (Two-Wire Interface)
}

void TWI_Start(void) {
	TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
	while ((TWCR & (1 << TWINT)) == 0);
}

void TWI_Stop(void) {
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO) ;
}

void TWI_Write(uint8_t data) {
	TWDR = data;
	TWCR = (1 << TWEN) | (1 << TWINT);
	while ((TWCR & (1 << TWINT)) == 0);
}

unsigned char TWI_Read(unsigned char isLast) {
	if (isLast == 0) // send ACK
		TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA); // send ACK
	else
		TWCR = (1 << TWEN) | (1 << TWINT); // send NACK
	while ((TWCR & (1 << TWINT)) == 0);
	return TWDR;
}


uint8_t bcd_to_dec(uint8_t val) {
	//val contiene dos dígitos decimales en formato BCD y lo convierte a un valor decimal.
	return ((val / 16 * 10) + (val % 16));
}

void RTC_DS3231_Init(void) {
	TWI_Init();
}

DateTime RTC_DS3231_Get(void) {
	DateTime now;
	// Iniciar la comunicación I2C
	TWI_Start();
	TWI_Write((DS3231_ADDRESS << 1) | TW_WRITE);  // Dirección + bit de escritura
	TWI_Write(0x00);  // Dirección del primer registro (segundos)

	// Reiniciar la comunicación para leer
	TWI_Start();
	TWI_Write((DS3231_ADDRESS << 1) | TW_READ);  // Dirección + bit de lectura

	// Leer los datos de los registros consecutivos
	now.second = bcd_to_dec(TWI_Read(1)); // TWI_Read_ACK() para leer un byte de datos del bus I2C
	now.minute = bcd_to_dec(TWI_Read(1));
	now.hour = bcd_to_dec(TWI_Read(1));
	now.day = bcd_to_dec(TWI_Read(1));
	now.month = bcd_to_dec(TWI_Read(1));
	now.year = bcd_to_dec(TWI_Read(1)) + 2024;  // El RTC da los años desde el 2024

	// Finalizar la comunicación I2C
	TWI_Stop();

	return now;
}

