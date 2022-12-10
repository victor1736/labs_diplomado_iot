/*! @file : K32L2B31A_Project.c
 * @author  Ernesto Andres Rincon Cruz
 * @version 0.0.000
 * @date    23/08/2021
 * @brief   Funcion principal main
 * @details
 *			v0.0.000	Proyecto base creado usando MCUXpresso
 *
 *
 */
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "K32L2B31A.h"
#include "fsl_debug_console.h"
#include <limits.h>
#include <float.h>

#include "hal_i2c_1.h"

#include <leds.h>
#include <sensor_luz.h>
#include "botones.h"
#include "sensor_temperatura.h"
#include "sensor_analogico.h"
#include "EC200T.h"

#include "lptimer_0.h"
#include "lpuart0.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define HABILITAR_SENSOR_BME280		0


#define HABILITAR_TLPTMR0			1
#define HABILITAR_I2C1				1

/*******************************************************************************
 * Private Prototypes
 ******************************************************************************/

/*******************************************************************************
 * External vars
 ******************************************************************************/

/*******************************************************************************
 * Local vars
 ******************************************************************************/
char mensaje_text[]="mqtt_topi:temperatura";
/*******************************************************************************
 * Private Source Code
 ******************************************************************************/
int main(void) {
	/*Crea variables locales -------------------------------------*/
	uint32_t adc_sensor_value;
	uint32_t adc_light_value;
	float temperature_value;
	status_t status;
	uint8_t nuevo_byte_uart;

#if HABILITAR_SENSOR_BME280
	bme280_data_t bme280_datos;
	uint8_t bme280_detectado=0;
	uint8_t bme280_base_de_tiempo=0;
#endif


	/* Finaliza la creación de variables locales-----------------*/

    /* Inicialización del microcontrolador ----------------------*/
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    /* Finaliza inicialización ----------------------------------*/

#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Inicializa el puerto serial para enviar mensajes al MODEM/DOCKLIGHT*/
    BOARD_InitDebugConsole();
#endif

#if HABILITAR_TLPTMR0
    /* Activa LPTMR0 para que iniciar contador y posterior IRQ cada 1 segundo*/
    printf("Inicializa LPTMR0:");
    LPTMR_StartTimer(LPTMR0);
    printf("OK\r\n");
#endif

#if HABILITAR_I2C1
    /* Inicializa I2C1 para lectura de sensores SHT31 y BME280*/
    //Solo avanza si es exitoso el proceso
    printf("Inicializa I2C1:");
    if(i2c1MasterInit(100000)!=kStatus_Success){	//100kbps
    	printf("Error");
    	return 0 ;
    }
    printf("OK\r\n");
#endif



#if HABILITAR_SENSOR_BME280
    printf("Detectando BME280:");
    //LLamado a funcion que identifica sensor BME280
    if (bme280WhoAmI() == kStatus_Success){
    	printf("OK\r\n");
    	(void)bme280Init();	//inicializa sensor bme280
    	bme280_detectado=1;	//activa bandera que indica (SI HAY BME280)
    }
#endif

    /* Activa LPTMR0 para que iniciar contador y posterior IRQ cada 1 segundo*/
    printf("Inicializa Modem EC200T\r\n");
    ec200tInicializacion();

    while(1) {
    	if(lptmr0_ticks>100){
    		lptmr0_ticks=0;
        	toggleLedRojo();
        	toggleLedVerde();

        	ec200tPolling();	//Actualiza FSM de modem EC25

    		//Busca si llegaron nuevos datos desde modem mientras esperaba
    		if (lpUart0CuantosDatosHayEnBuffer() > 0) {
    			status = lpUart0LeerByteDesdeBuffer(&nuevo_byte_uart);
    			if (status == kStatus_Success) {

#if HABILITAR_SENSOR_BME280
					if(bme280_detectado==1){
						bme280_base_de_tiempo++;	//incrementa base de tiempo para tomar dato bme280
						if(bme280_base_de_tiempo>0){	//	>10 equivale aproximadamente a 2s
							bme280_base_de_tiempo=0; //reinicia contador de tiempo
							if(bme280ReadData(&bme280_datos)==kStatus_Success){	//toma lectura humedad, presion, temperatura
								printf("BME280 ->");
								printf("temperatura:0x%X ",bme280_datos.temperatura);	//imprime temperatura sin procesar
								printf("humedad:0x%X ",bme280_datos.humedad);	//imprime humedad sin procesar
								printf("presion:0x%X ",bme280_datos.presion);	//imprime presion sin procesar
								printf("\r\n");	//Imprime cambio de linea
							}
						}
					}
#endif



    	            printf("boton1:%d\r\n",leerBoton1());
    	            printf("boton2:%d\r\n",leerBoton2());
    	            printf("\r\n");


    	            switch(nuevo_byte_uart){
    	            	case 'm':
    	            		ec200tEnviarMensajeDeTexto((uint8_t*)(&mensaje_text[0]),(uint8_t)(strlen(&mensaje_text[0])));
    	            		printf("Nuevo byte:%c - 0x%2x\r\n",nuevo_byte_uart,nuevo_byte_uart);/* Imprime byte recibido por puerto LPUART0*/
    	            		printf("Has enviado el mensaje\r\n");
    	            		break;
    	            	case 'R':
    	            		encenderLedRojo();
    	            		printf("Nuevo byte:%c - 0x%2x\r\n",nuevo_byte_uart,nuevo_byte_uart);/* Imprime byte recibido por puerto LPUART0*/
    	            		printf("Has encendido el led rojo\r\n");
    	            		break;
    	            	case 'r':
    	            		apagarLedRojo();
    	            		printf("Nuevo byte:%c - 0x%2x\r\n",nuevo_byte_uart,nuevo_byte_uart);/* Imprime byte recibido por puerto LPUART0*/
    	            		printf("Has apagado el led rojo\r\n");
    	            		break;
    	            	case 'V':
    	            		encenderLedVerde();
    	            		printf("Nuevo byte:%c - 0x%2x\r\n",nuevo_byte_uart,nuevo_byte_uart);/* Imprime byte recibido por puerto LPUART0*/
    	            		printf("Has encendido el verde rojo\r\n");
    	            		break;
    	            	case 'v':
    	            		apagarLedVerde();
    	            		printf("Nuevo byte:%c - 0x%2x\r\n",nuevo_byte_uart,nuevo_byte_uart);/* Imprime byte recibido por puerto LPUART0*/
    	            		printf("Has apagado el verde rojo\r\n");
    	            		break;
    	            	case 'L':
    	            		/* Toma lectura del sensor de luz*/
    	            		adc_light_value=getLightADC();
    	            		printf("ADC Light: %d\r\n", adc_light_value);
    	            		printf("Nuevo byte:%c - 0x%2x\r\n",nuevo_byte_uart,nuevo_byte_uart);/* Imprime byte recibido por puerto LPUART0*/
    	            		break;
    	            	case 'T':
    	            		/* Toma lectura de temperatura*/
    	    				temperature_value=getTemperatureValue();
    	    				printf("Temperature: %f\r\n", temperature_value);
    	    				printf("Nuevo byte:%c - 0x%2x\r\n",nuevo_byte_uart,nuevo_byte_uart);/* Imprime byte recibido por puerto LPUART0*/
    	            		break;
    	            	case 'S':
    	            		/* Toma lectura de sensor análogico externo*/
    	            		adc_sensor_value=getSensorADC();
    	            		printf("ADC sensor: %d\r\n", adc_sensor_value);
    	            		printf("Nuevo byte:%c - 0x%2x\r\n",nuevo_byte_uart,nuevo_byte_uart);/* Imprime byte recibido por puerto LPUART0*/
    	            		break;
    	            }
    			}
    		}


    	}
    }
    return 0 ;
}

