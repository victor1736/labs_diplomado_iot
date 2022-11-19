/*! @file : leds.c
 * @author  Victor Alfonso Fernandez Hoyos
 * @version 1.0.0
 * @date    18/11/2022
 * @brief   Driver para 
 * @details
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
#include "leds.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_LED_GPIO1     BOARD_LED_RED_GPIO
#define BOARD_LED_GPIO_PIN1 BOARD_LED_RED_GPIO_PIN
#define BOARD_LED_GPIO2     BOARD_LED_GREEN_GPIO
#define BOARD_LED_GPIO_PIN2 BOARD_LED_GREEN_GPIO_PIN

/*******************************************************************************
 * Private Prototypes
 ******************************************************************************/


/*******************************************************************************
 * External vars
 ******************************************************************************/


/*******************************************************************************
 * Local vars
 ******************************************************************************/


/*******************************************************************************
 * Private Source Code
 ******************************************************************************/
 void encender_led_rojo(void){
	return( GPIO_PortClear(BOARD_LED_GPIO1, 1U << BOARD_LED_GPIO_PIN1));
 }
 void apagar_led_rojo(void){
	 return(GPIO_PortClear(BOARD_LED_GPIO1, 1U << BOARD_LED_GPIO_PIN1));
 }


/*******************************************************************************
 * Public Source Code
 ******************************************************************************/

