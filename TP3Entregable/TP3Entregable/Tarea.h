/*
 * Tarea.h
 *
 * Created: 10/6/2024 10:09:24
 *  Author: LENOVO
 */ 


#ifndef TAREA_H_
#define TAREA_H_

#include "DHT11.h"
#include "rtc_ds3231.h"

void ArmarBuffer();//Guarda en el buffer los datos de temperatura y humedad
void leerComando(char *c);//Para ver si se detiene o se habilita el envio de datos 




#endif /* TAREA_H_ */