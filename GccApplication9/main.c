/*
 * GccApplication9.c
 *
 * Created: 29/5/2024 19:59:43
 * Author : Notebook
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "serialPort.h"
#include "rtc_ds3231.h"
#include "dht11.h"


volatile uint8_t send_data = 1;  // Variable global para controlar el envío de datos


// ISR para manejar la recepción de datos desde la PC
ISR(USART_RX_vect) {
	char received_char = UDR0; // Leer el carácter recibido desde el registro de datos UART
	if (received_char == 's' || received_char == 'S') {  // Comprobar si el carácter recibido es 's' o 'S'
		send_data = !send_data;  // Alternar el estado de envío de datos
	}
}

int main(void)
{
	 // Inicialización de UART
	 uint8_t uartConfig = 0x33; // Configuración para 9600bps @ F_CPU = 8MHz
	 SerialPort_Init(uartConfig);// Configuración de la UART
	 SerialPort_TX_Enable(); // // Habilitar la transmisión por UART
	 SerialPort_RX_Enable(); // // Habilitar la recepción por UART
	 
	 DHT11_init(); // inicializo DHT11
	 RTC_DS3231_Init(); // inicializo DS3231
	 UCSR0B |= (1 << RXCIE0); // Habilitar interrupción de recepción UART
	 sei();  // Habilitar interrupciones globales
	 DateTime now; // Estructura para almacenar fecha y hora
	 
    while (1) 
    {
		uint8_t temp = 0;
		uint8_t hum = 0;
		
		if (send_data) { // Si se presiono S va alternando entre 0 o 1. Si es uno ejecuto.
			if(DHT11_read(&temp, &hum)) // 1 si funciona el DHT11
			{
				
				now = RTC_DS3231_Get();// Obtener la fecha y hora actuales del RTC
				//Enviamos por UART TEMPERATURA/HUMEDAD/FECHA/HORA
				 SerialPort_Send_String("TEMP:"); 
				 SerialPort_Send_uint8_t(temp); 
				 SerialPort_Send_String("°C");
				 SerialPort_Send_String("HUM:");
				 SerialPort_Send_uint8_t(hum);
				 SerialPort_Send_String("%");
				 SerialPort_Send_String("FECHA:");
				 SerialPort_Send_uint8_t(now.day);
				 SerialPort_Send_String("/");
				 SerialPort_Send_uint8_t(now.month);
				 SerialPort_Send_String("/");
				 SerialPort_Send_uint8_t(now.year);
				 SerialPort_Send_String(" ");
				 SerialPort_Send_String("HORA:");
				 SerialPort_Send_uint8_t(now.hour);
				 SerialPort_Send_String(":");
				 SerialPort_Send_uint8_t(now.minute);
				 SerialPort_Send_String(":");
				 SerialPort_Send_uint8_t(now.second);
				 SerialPort_Send_String("\r\n");
				}
		 }
		 _delay_ms(2000); // Esperamos 2 seg
    }
}

