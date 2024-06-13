/*
 * Tarea.c
 *
 * Created: 8/6/2024 22:04:56
 *  Author: LENOVO
 */ 

#include "Tarea.h"

extern char buffer[];
extern uint8_t modo;
	uint8_t temperaturaE;
	uint8_t humedadE;

void ArmarBuffer(){
	temperaturaE=0;
	humedadE=0;
	DHT11_read(&humedadE,&temperaturaE);
	//Almaceno datos en el buffer para luego imprimir por la terminal
	buffer[6]='0'+ (temperaturaE/10); // primer digito
	buffer[7]='0'+ (temperaturaE%10); // segundo digito
	buffer[14]='0'+(humedadE/10); // primer digito
	buffer[15]='0'+(humedadE%10); // segundo digito
	

	DateTime now;
	now = RTC_DS3231_Get(); // obtengo los datos de fecha y hora
	//fecha
	buffer[24]='0'+now.day/10;
	buffer[25]='0'+now.day%10;
	buffer[27]='0'+now.month/10;
	buffer[28]='0'+now.month%10;
	buffer[30]='0'+now.year/10;
	buffer[31]='0'+now.year%10;
	//hora
	buffer[38]='0'+now.hour/10;
	buffer[39]='0'+now.hour%10;
	buffer[41]='0'+now.minute/10;
	buffer[42]='0'+now.minute%10;
	buffer[44]='0'+now.second/10;
	buffer[45]='0'+now.second%10;
	
} 

