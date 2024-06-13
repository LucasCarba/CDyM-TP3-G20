/*
 * TP3Entregable.c
 *
 * Created: 3/6/2024 11:19:22
 * Author : LENOVO
 */ 

#include "main.h"

volatile uint8_t send_data = 1;  // Variable global para controlar el envío de datos
volatile char buffer[]= "TEMP:  C HUM:  % FECHA:  /  /   HORA   :  :   \r\n";

char comando[2];
uint8_t hayDato=0;//Para ver si hay dato a procesar 
volatile char RX_BUFFER;
volatile uint8_t cant =0;
uint8_t flag=0b11111111;

int main(void)
{
	
	// Inicialización de UART
	SerialPort_Init(0x67); 		// Inicializo formato 8N1 y 0x67=103 BAUDRATE = 9600bps
	SerialPort_TX_Enable();    // Activo el transmisor del puerto serie
	SerialPort_RX_Enable();    //Activo el receptor del puerto serie 
	SerialPort_RX_Interrupt_Enable();//Activo interrupcion de recepcion
	sei();
	//DHT11_init(); // inicializo sensor
	RTC_DS3231_Init();//inicializo rtc
	
	while (1)
	{
		        if (flag) // Si flag es xFF, funciona, si flag es x00, parada.
		        {  
					_delay_ms(3000);
					 hayDato=1;
					 ArmarBuffer();
					 SerialPort_TX_Interrupt_Enable();//Habilito interrupcion para imprimir buffer
		        }							
	}
}
	ISR(USART_RX_vect){//interrupcion para recibir datos de la terminal
		SerialPort_TX_Interrupt_Disable();//deshabilito interrupciones 
		RX_BUFFER=SerialPort_Recive_Data();// recibo datos de la terminal para procesar
		int cant = 0;
		if(RX_BUFFER!='\r'){ // guardo el dato que puede ser s o S
		    comando[cant]=RX_BUFFER;
			cant++;// el usuario no dio enter
	    }
	    else {
		comando[cant]='\n';
		}
		if (comando[0]=='s'|| comando[0]=='S')
		{
			flag=!flag; // alterno valor del flag
		}
	   
	}
	ISR(USART_UDRE_vect){//interrupcion para enviar datos a la terminal 
		if (hayDato) // imprimo temperatura , humdad , fecha y hora
		{
			static uint8_t i=0;
			SerialPort_Send_Data(buffer[i]);
			i++;
			if (buffer[i]=='\0')
			{
				i=0;
				hayDato=0;
				SerialPort_TX_Interrupt_Disable();//Deshabilito interrupcion
			}
		}
		
	}

