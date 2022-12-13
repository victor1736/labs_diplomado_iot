/*! @file : leds.h
 * @author  Victor Alfonso Fernandez Hoyos
 * @version 1.0.0
 * @date    18/11/2022
 * @brief   Driver para 
 * @details
 *
 */
#ifndef LIBRERIA_IOT_LEDS_H_
#define LIBRERIA_IOT_LEDS_H_
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <hal_gpio.h>
#include "peripherals.h"
#include "pin_mux.h"
#include "board.h"
/*!
 * @addtogroup X
 * @{
 */
/*!
 * @addtogroup X
 * @{
 */
/*******************************************************************************
 * Public Definitions
 ******************************************************************************/
#define LED_VERDE_PIN	KPTD5
#define LED_ROJO_PIN	KPTE31
/*******************************************************************************
 * External vars
 ******************************************************************************/

/*******************************************************************************
 * Public vars
 ******************************************************************************/

/*******************************************************************************
 * Public Prototypes
 ******************************************************************************/
static inline void encenderLedRojo(void) {
	gpioPutValue(LED_ROJO_PIN, 0);
}

static inline void apagarLedRojo(void) {
	gpioPutValue(LED_ROJO_PIN, 1);
}

static inline void toggleLedRojo(void){
	gpioPutToggle(LED_ROJO_PIN);
}

static inline void encenderLedVerde(void) {
	gpioPutValue(LED_VERDE_PIN, 0);
}

static inline void apagarLedVerde(void) {
	gpioPutValue(LED_VERDE_PIN, 1);
}

static inline void toggleLedVerde(void){
	gpioPutToggle(LED_VERDE_PIN);
}
/** @} */ // end of X group
/** @} */ // end of X group

#endif /* LIBRERIA_IOT_LEDS_H_ */
