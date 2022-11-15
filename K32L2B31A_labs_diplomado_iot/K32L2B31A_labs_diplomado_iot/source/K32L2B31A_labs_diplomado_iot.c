/*
 * @file   	 	:K32L2B31A_labs_diplomado_iot.c
 * @author 		:Victor Alfonso Fernandez Hoyos
 * @version		:0.0.000
 * @date		:08/11/2022
 * @brief   	:Funcion principal main
 * @details
 * 				v0.0.000  	Proyecto base creado usando MCUXpresso
 *
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
/******************************************************************************************
 * Definitions
 *****************************************************************************************/

/******************************************************************************************
 * Private prototypes
 *****************************************************************************************/

/******************************************************************************************
 * External vars
 *****************************************************************************************/

/******************************************************************************************
 * Local vars
 *****************************************************************************************/
    volatile static int i = 0 ;
    float voltaje=12.5f;
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
    PRINTF("Voltaje: %2.3f\r\n",voltaje);
    /* Force the counter to be placed into memory. */

    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
        i++ ;
        /* 'Dummy' NOP to allow source level single stepping of
            tight while() loop */
        __asm volatile ("nop");
    }
    return 0 ;
}
