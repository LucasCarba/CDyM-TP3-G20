/*
 * rtc_ds3231.c
 *
 * Created: 8/6/2024 18:58:24
 *  Author: LENOVO
 */ 

#include "rtc_ds3231.h"


#define DS3231_ADDRESS 0x68 //especificar la dirección I2C del módulo RTC DS3231
#define ALTA 0b11110000 // PARTE ALTA
#define BAJA 0b00001111 // PARTE BAJA

void TWI_Init(void) 
{
	TWSR = 0x00;  // Prescaler de TWI = 1
	TWBR = 152;    // Configurar SCL a 100 kHz con F_CPU = 16 MHz
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
	return ((val & ALTA) >> 4 ) * 10 + ( val & BAJA);
}

uint16_t bcd_anio_dec(uint8_t bcd_year) {
	// Convertir año en formato BCD a decimal y ajustar a partir del año 2024
	return 24 + bcd_to_dec(bcd_year);
}

uint8_t config_errorFecha(uint8_t val) {
	// Convierte un byte BCD de día/mes en su equivalente decimal, iniciando desde 1.
	uint8_t num = bcd_to_dec(val);
	if (num == 0) {
		return 1; // Ajuste para iniciar desde 1 si el día/mes es 0 en BCD
	}
	return num;
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
	now.second = bcd_to_dec(TWI_Read(0));  // Leer segundos (sin ACK)
	now.minute = bcd_to_dec(TWI_Read(0));  // Leer minutos (sin ACK)
	now.hour = bcd_to_dec(TWI_Read(0));    // Leer horas (sin ACK)
	TWI_Read(0);  // Leer día de la semana y descartar (sin ACK)
	now.day = config_errorFecha(TWI_Read(0));     // Leer día del mes (sin ACK)
	now.month = config_errorFecha(TWI_Read(0));   // Leer mes (sin ACK)
	now.year = bcd_anio_dec(TWI_Read(1));  // Leer año (con ACK)

	// Finalizar la comunicación I2C
	TWI_Stop();

	return now;
} 