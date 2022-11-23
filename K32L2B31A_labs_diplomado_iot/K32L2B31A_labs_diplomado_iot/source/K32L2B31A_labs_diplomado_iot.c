/*
 * @file   	 	:K32L2B31A_labs_diplomado_iot.c
 * @author 		:Victor Alfonso Fernandez Hoyos
 * @version		:0.0.000
 * @date		:08/11/2022
 * @brief   	:Funcion principal main
 * @details
 * 				v0.0.000  	Proyecto base creado usando MCUXpresso
 */

/******************************************************************************************
 * Include
 *****************************************************************************************/
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "K32L2B31A.h"

#include "fsl_debug_console.h"
#include "fsl_adc16.h"
#include "lpuart0.h"
#include "leds.h"
#include "sensor_luz.h"
/******************************************************************************************
 * Definitions
 *****************************************************************************************/
#define BOARD_LED_GPIO1     BOARD_LED_RED_GPIO
#define BOARD_LED_GPIO_PIN1 BOARD_LED_RED_GPIO_PIN
#define BOARD_LED_GPIO2     BOARD_LED_GREEN_GPIO
#define BOARD_LED_GPIO_PIN2 BOARD_LED_GREEN_GPIO_PIN
#define LPUART0_BUFFER_SIZE_MAX 50

enum{
	CMD_AT_ATI_Display_Product_Identification_Information=0,
	CMD_AT_AT_GMI_Request_Manofacturer_Identification,
	CMD_AT_AT_GMM_Request_TA_Model_Identification,
	CMD_AT_AT_GMR_Request_TA_Revision_Identification_of_Sofware_Release,
	CMD_AT_AT_CGMI_Request_Manofacturer_Identification,
	Error,
};

typedef struct _iot_nodo_data{
	uint16_t Data_sensor_luz_adc;				//2 bytes
	uint16_t Data_sensor_luz_lux;				//2 bytes
	//--------------------------------------
	uint8_t Data_sensor_luz_voltaje;			//1 bytes
	uint8_t Data_sensor_luz_corriente;			//1 bytes
	uint16_t Data_sensor_luz_humedad;			//2 bytes
	//----------------------------------------
	uint16_t Data_sensor_presion_atmosferica;	//2 bytes
	uint8_t Data_sensor_luz_temperatura;		//1 bytes
	uint8_t Reservado;							//1 bytes

}iot_nodo_data_t;

typedef struct _sigfox__data_upllink_MAC_frame{
	uint32_t Preamble;				//4 bites
	uint16_t Frame_sync;			//2 bites
	uint32_t End_device_ID;			//4 bites
	uint8_t Payload[12];			//12 bits
	uint16_t Authentification;
	uint16_t FCS;					//2 bits

}sigfox__data_upllink_MAC_frame_t;

enum{
	FSM_ESTADO_INICIO=0,
	FSM_ESTADO_ESPERA_NUEVO_DATO_LPUART0,
	FSM_ESTADO_ANALIZANDO_NUEVO_DATO_LPUART0,
	FSM_ESTADO_ENVIAR_COMANDO_ATI,
	FSM_ESTADO_ENVIAR_COMANDO_AT_GMI,
	FSM_ESTADO_ENVIAR_COMANDO_AT_GMM,
	FSM_ESTADO_ENVIAR_COMANDO_AT_GMR,
	FSM_ESTADO_ENVIAR_COMANDO_AT_CGMI,
	FSM_ESTADO_ANALIZAR_COMANDO_ATI,
	FSM_ESTADO_ANALIZAR_COMANDO_AT_GMI,
	FSM_ESTADO_ANALIZAR_COMANDO_AT_GMM,
	FSM_ESTADO_ANALIZAR_COMANDO_AT_GMR,
	FSM_ESTADO_ANALIZAR_COMANDO_AT_CGMI,
	FSM_ESTADO_STAR_ADC,
	FMS_ESTADO_ESPERAR_TIEMPO_RESULTADO,
	FSM_ESTADO_CAPTURA_RASULTADO_ADC,
	FSM_ESTADO_CAPTURA_IMPRIME_RESULTADO_ADC,
	FSM_ESTADO_ENCENDIDO_LED_ROJO,
	FSM_ESTADO_APAGADO_LED_ROJO,
	FSM_ESTADO_ENCENDIDO_LED_VERDE,
	FSM_ESTADO_APAGADO_LED_VERDE,
};

/******************************************************************************************
 * Private prototypes
 *****************************************************************************************/

/******************************************************************************************
 * External vars
 *****************************************************************************************/

/******************************************************************************************
 * Local vars
 *****************************************************************************************/
    const char* cmd_at[5]={
    		"ATI\r\n", 			//M
			"AT+GMI\r\n",		//Q
			"AT+GMM\r\n",		//W
			"AT+GMR\r\n",		//E
			"AT+CGMI\r\n",		//A
    };

    const char* respuestas_cmd_AT[]={
    		"EC25EFAR02A09M4G",				//ATI
			"OK",															//AT+GMI
			"EC2x",															//AT+GMM
			"EC25EFAR02A09",												//AT+GMR
			"READY",														//AT+CGMI
			"ERROR",														//ERROR
    };


 iot_nodo_data_t datos_locales;
 sigfox__data_upllink_MAC_frame_t sigfox_frame;
 uint8_t fst_estado_actual=FSM_ESTADO_INICIO;
 uint8_t *data;
 char respuesta_mensaje[LPUART0_BUFFER_SIZE_MAX];
 

/******************************************************************************************
 * Private Source Code
 *****************************************************************************************/
void lectura_respuesta_mensaje(uint8_t *respuesta, char *suceso_mensaje, char *error_msg) {
	for (uint8_t i = 1; i < LPUART0_BUFFER_SIZE_MAX; i++) {
		respuesta_mensaje[i - 1] = *(respuesta + i);
	}

	if (strstr(respuesta_mensaje, suceso_mensaje)) {
		PRINTF("Respuesta: OK");
	} else if (strstr(respuesta_mensaje, error_msg)) {
		PRINTF("Error");
	} else {
		PRINTF("No especificado");
	}
	lpuart0_borrar_buffer();
	escribir_bandera_nuevo_dato(0);

	for (uint8_t i = 0; i < LPUART0_BUFFER_SIZE_MAX; i++) {
		respuesta_mensaje[i] = 0;
	}
}						
						
						
void ec25_print_data_row(uint8_t *data_ptr, uint32_t data_size){
	for(uint32_t i=0; i < data_size; i++){
		PRINTF("%c", *(data_ptr + i));
	}
}
void ec25_print_data_ascii_hex(uint8_t *data_ptr, uint32_t data_size){
	for(uint32_t i=0; i < data_size; i++){
		PRINTF("%02x", *(data_ptr + i));
	}
}

int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif


    PRINTF("Hola mundo\r\n");
	//
	//ec25_print_data_ascii_hex((uint8_t*)(&datos_locales),sizeof(datos_locales));

	//memcpy(&sigfox_frame.Payload[0],(uint8_t*)(&datos_locales),sizeof(datos_locales));
	//envio de datos de la base de datos
	//ec25_print_data_row((uint8_t*)(&sigfox_frame),sizeof(sigfox_frame));


    while(1) {
		switch (fst_estado_actual) {
		/**********************************************************************************/
		case FSM_ESTADO_INICIO:
			/*Escribir condiciones iniciales para la ejecucion de toda la FSM*/
	    	/*
	    	 * Simulacion de  tx  por sigfox
	    	 */
	    	sigfox_frame.Preamble=0x1234;
	    	sigfox_frame.End_device_ID=0x1234;
	    	sigfox_frame.Frame_sync=0x2345;
			fst_estado_actual=FSM_ESTADO_ESPERA_NUEVO_DATO_LPUART0;
			break;
		/************************************************************************************/
		case FSM_ESTADO_ESPERA_NUEVO_DATO_LPUART0:
			 if (leer_bandera_nuevo_dato ()!=0){
	        	    escribir_bandera_nuevo_dato(0);
	        	    fst_estado_actual=FSM_ESTADO_ANALIZANDO_NUEVO_DATO_LPUART0;
			 }

			break;
		/************************************************************************************/
		case FSM_ESTADO_ANALIZANDO_NUEVO_DATO_LPUART0:
			switch(leer_dato()){
			case 'l':
					fst_estado_actual=FSM_ESTADO_STAR_ADC;
				break;
			case 'R':
				fst_estado_actual=FSM_ESTADO_ENCENDIDO_LED_ROJO;
				break;
			case 'r':
				fst_estado_actual=FSM_ESTADO_APAGADO_LED_ROJO;
				break;
    		case 'G':
			fst_estado_actual=FSM_ESTADO_ENCENDIDO_LED_VERDE;
				break;
			case 'g':
				fst_estado_actual=FSM_ESTADO_APAGADO_LED_VERDE;
				break;
			case 'M':
				fst_estado_actual=FSM_ESTADO_ENVIAR_COMANDO_ATI;
				break;
			case 'Q':
				fst_estado_actual=FSM_ESTADO_ENVIAR_COMANDO_AT_GMI;
				break;
			case 'W':
				fst_estado_actual=FSM_ESTADO_ENVIAR_COMANDO_AT_GMM;
				break;
			case 'E':
				fst_estado_actual=FSM_ESTADO_ENVIAR_COMANDO_AT_GMR;
				break;
			case 'A':
				fst_estado_actual=FSM_ESTADO_ENVIAR_COMANDO_AT_CGMI;
				break;
			case 'b':
				lpuart0_borrar_buffer();
				PRINTF("El buffer a sido borrado");
				break;
			default://estado ilegal
				fst_estado_actual=FSM_ESTADO_INICIO;
				break;
			}
			break;

		/************************************************************************************/
		case FSM_ESTADO_ENVIAR_COMANDO_ATI:
			PRINTF("%s",cmd_at[CMD_AT_ATI_Display_Product_Identification_Information]);
			fst_estado_actual=FSM_ESTADO_ANALIZAR_COMANDO_ATI;
			break;
		/************************************************************************************/
		/************************************************************************************/
		case FSM_ESTADO_ANALIZAR_COMANDO_ATI:
			if(leer_bandera_nuevo_dato ()!=0){
				data=lectura_buffer();
				lectura_respuesta_mensaje(data,respuestas_cmd_AT[CMD_AT_ATI_Display_Product_Identification_Information],respuestas_cmd_AT[Error]);
				fst_estado_actual=FSM_ESTADO_INICIO;
			}

			break;
		/************************************************************************************/
		case FSM_ESTADO_ENVIAR_COMANDO_AT_GMI:
			PRINTF("%s",cmd_at[CMD_AT_AT_GMI_Request_Manofacturer_Identification]);
			fst_estado_actual=FSM_ESTADO_ANALIZAR_COMANDO_AT_GMI;
			break;

		/************************************************************************************/
		/************************************************************************************/
		case FSM_ESTADO_ANALIZAR_COMANDO_AT_GMI:
			if(leer_bandera_nuevo_dato ()!=0){
				data=lectura_buffer();
				lectura_respuesta_mensaje(data,respuestas_cmd_AT[CMD_AT_AT_GMI_Request_Manofacturer_Identification],respuestas_cmd_AT[Error]);
				fst_estado_actual=FSM_ESTADO_INICIO;
			}

			break;
		/************************************************************************************/
		/************************************************************************************/
		case FSM_ESTADO_ENVIAR_COMANDO_AT_GMM:
			PRINTF("%s",cmd_at[CMD_AT_AT_GMM_Request_TA_Model_Identification]);
			fst_estado_actual=FSM_ESTADO_ANALIZAR_COMANDO_AT_GMM;
			break;

		/************************************************************************************/
		/************************************************************************************/
		case FSM_ESTADO_ANALIZAR_COMANDO_AT_GMM:
			if(leer_bandera_nuevo_dato ()!=0){
				data=lectura_buffer();
				lectura_respuesta_mensaje(data,respuestas_cmd_AT[CMD_AT_AT_GMM_Request_TA_Model_Identification],respuestas_cmd_AT[Error]);
				fst_estado_actual=FSM_ESTADO_INICIO;
			}

			break;
		/************************************************************************************/
		/************************************************************************************/
		case FSM_ESTADO_ENVIAR_COMANDO_AT_GMR:
			PRINTF("%s",cmd_at[CMD_AT_AT_GMR_Request_TA_Revision_Identification_of_Sofware_Release]);
			fst_estado_actual=FSM_ESTADO_ANALIZAR_COMANDO_AT_GMR;
			break;

		/************************************************************************************/
		case FSM_ESTADO_ANALIZAR_COMANDO_AT_GMR:
			if(leer_bandera_nuevo_dato ()!=0){
				data=lectura_buffer();
				lectura_respuesta_mensaje(data,respuestas_cmd_AT[CMD_AT_AT_GMR_Request_TA_Revision_Identification_of_Sofware_Release],respuestas_cmd_AT[Error]);
				fst_estado_actual=FSM_ESTADO_INICIO;
			}

			break;
		/************************************************************************************/
		/************************************************************************************/
		case FSM_ESTADO_ENVIAR_COMANDO_AT_CGMI:
			PRINTF("%s",cmd_at[CMD_AT_AT_CGMI_Request_Manofacturer_Identification]);
			fst_estado_actual=FSM_ESTADO_ANALIZAR_COMANDO_AT_CGMI;
			break;

		/************************************************************************************/
		/************************************************************************************/
		case FSM_ESTADO_ANALIZAR_COMANDO_AT_CGMI:
			if(leer_bandera_nuevo_dato ()!=0){
				data=lectura_buffer();
				lectura_respuesta_mensaje(data,respuestas_cmd_AT[CMD_AT_AT_CGMI_Request_Manofacturer_Identification],respuestas_cmd_AT[Error]);
				fst_estado_actual=FSM_ESTADO_INICIO;
			}

			break;
		/************************************************************************************/
		/************************************************************************************/
		case FSM_ESTADO_STAR_ADC:
			/* Genera Señal de STAR para tomar dato ADC*/
			sensor_luz();				//No a terminado de realizar la operacion
			fst_estado_actual=FSM_ESTADO_CAPTURA_RASULTADO_ADC;

			break;
		/************************************************************************************/
		case FMS_ESTADO_ESPERAR_TIEMPO_RESULTADO:
			fst_estado_actual=FSM_ESTADO_CAPTURA_RASULTADO_ADC;
			break;
		/************************************************************************************/
		case FSM_ESTADO_CAPTURA_RASULTADO_ADC:
	    	//Datos referente al sensor de luz y calculos correspondientes
    		datos_locales.Data_sensor_luz_adc=sensor_luz();
            datos_locales.Data_sensor_luz_voltaje=(datos_locales.Data_sensor_luz_adc*3.3)/4096;
            datos_locales.Data_sensor_luz_corriente=((3.3-datos_locales.Data_sensor_luz_voltaje)/10000)*1000000;
            datos_locales.Data_sensor_luz_lux=3.0303*datos_locales.Data_sensor_luz_corriente;
			fst_estado_actual=FSM_ESTADO_CAPTURA_IMPRIME_RESULTADO_ADC;
			break;
		/************************************************************************************/
		case FSM_ESTADO_CAPTURA_IMPRIME_RESULTADO_ADC:
            //Salida del monitor serial
        	PRINTF("dato lpuat0: %d \r\n",leer_dato());
        	PRINTF("dato lpuat0: 0x%x \r\n",leer_dato());
        	PRINTF("dato lpuat0: %c \r\n",leer_dato());
            PRINTF("Data_sensor_luz_ADC: %d\r\n", datos_locales.Data_sensor_luz_adc );
            PRINTF("Data_sensor_luz_voltaje: %d\r\n",datos_locales.Data_sensor_luz_voltaje);
            PRINTF("Data_sensor_luz_corriente: %d\r\n",datos_locales.Data_sensor_luz_corriente);
            PRINTF("Luz: %d\r\n",datos_locales.Data_sensor_luz_lux);
			fst_estado_actual=FSM_ESTADO_INICIO;
			break;


		/*************************************************************************************/
		case FSM_ESTADO_ENCENDIDO_LED_ROJO:
        	PRINTF("dato lpuat0: %d \r\n",leer_dato());
        	PRINTF("dato lpuat0: 0x%x \r\n",leer_dato());
        	PRINTF("dato lpuat0: %c \r\n",leer_dato());
			GPIO_PortClear(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN);
			fst_estado_actual=FSM_ESTADO_INICIO;
			break;
		/*************************************************************************************/
		/*************************************************************************************/
		case FSM_ESTADO_APAGADO_LED_ROJO:
        	PRINTF("dato lpuat0: %d \r\n",leer_dato());
        	PRINTF("dato lpuat0: 0x%x \r\n",leer_dato());
        	PRINTF("dato lpuat0: %c \r\n",leer_dato());
			GPIO_PortSet(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN);
			fst_estado_actual=FSM_ESTADO_INICIO;
			break;
		/*************************************************************************************/
		/*************************************************************************************/
		case FSM_ESTADO_ENCENDIDO_LED_VERDE:
        	PRINTF("dato lpuat0: %d \r\n",leer_dato());
        	PRINTF("dato lpuat0: 0x%x \r\n",leer_dato());
        	PRINTF("dato lpuat0: %c \r\n",leer_dato());
			GPIO_PortClear(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN);
			fst_estado_actual=FSM_ESTADO_INICIO;
			break;
		/*************************************************************************************/
		/*************************************************************************************/
		case FSM_ESTADO_APAGADO_LED_VERDE:
        	PRINTF("dato lpuat0: %d \r\n",leer_dato());
        	PRINTF("dato lpuat0: 0x%x \r\n",leer_dato());
        	PRINTF("dato lpuat0: %c \r\n",leer_dato());
			GPIO_PortSet(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN);
			fst_estado_actual=FSM_ESTADO_INICIO;
			break;
		/*************************************************************************************/
		/************************************************************************************/
		default://estado ilegal
			fst_estado_actual=FSM_ESTADO_INICIO;
			break;

		}

		//__asm volatile ("nop");
    }

return 0 ;
}
