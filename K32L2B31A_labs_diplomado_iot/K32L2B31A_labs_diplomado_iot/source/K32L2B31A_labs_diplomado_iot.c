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

/******************************************************************************************
 * Private prototypes
 *****************************************************************************************/

/******************************************************************************************
 * External vars
 *****************************************************************************************/

/******************************************************************************************
 * Local vars
 *****************************************************************************************/

    float voltaje;
    float corriente;
    float luz;
    float RLDR;



/******************************************************************************************
 * Private Source Code
 *****************************************************************************************/


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



    /* Force the counter to be placed into memory. */

    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {

        if (leer_bandera_nuevo_dato ()!=0){
        	PRINTF("dato lpuat0: %d \r\n",leer_dato());
        	PRINTF("dato lpuat0: 0x%x \r\n",leer_dato());
        	PRINTF("dato lpuat0: %c \r\n",leer_dato());
        	if (leer_dato()==82){
        		GPIO_PortClear(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN);
        		escribir_bandera_nuevo_dato (0);
        	}
        	if (leer_dato()==114){
        		GPIO_PortSet(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN);
        		escribir_bandera_nuevo_dato (0);
        	        	}
        	if (leer_dato()==71){
        		GPIO_PortClear(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN);
        		escribir_bandera_nuevo_dato (0);
        	}
        	if (leer_dato()==103){
        		GPIO_PortSet(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN);
        		escribir_bandera_nuevo_dato (0);
        	        	}

        	if (leer_dato()==76){
                /* Captura de dato del ADC e imprime por consola */
                PRINTF("ADC Value: %d\r\n", sensor_luz() );
                voltaje=(sensor_luz()*3.3)/4096;
                PRINTF("Voltaje: %2.3f\r\n",voltaje);
                corriente=((3.3-voltaje)/10000)*1000000;
                PRINTF("Corriente: %2.3f\r\n",corriente);
                luz=3.0303*corriente;
                PRINTF("Luz: %3f\r\n",luz);

        	    escribir_bandera_nuevo_dato (0);
        }
        __asm volatile ("nop");
        }
    }
return 0 ;
}
