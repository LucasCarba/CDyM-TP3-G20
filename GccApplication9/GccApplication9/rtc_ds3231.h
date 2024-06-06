/*
 * rtc_ds3231.h
 */ 

#ifndef RTC_DS3231_H
#define RTC_DS3231_H

#include <avr/io.h>
#include <stdint.h>

// Estructura para la Fecha y Hora
typedef struct {
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t day;
	uint8_t month;
	uint16_t year;
} DateTime;

// Funciones Públicas
void RTC_DS3231_Init(void);
DateTime RTC_DS3231_Get(void);

#endif // RTC_DS3231_H
