/*! @file : sensor_luz.c
 * @author  Victor Alfonso Fernandez Hoyos
 * @version 1.0.0
 * @date    19/11/2022
 * @brief   Driver para 
 * @details
 *
*/
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "sensor_luz.h"
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "K32L2B31A.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Private Prototypes
 ******************************************************************************/


/*******************************************************************************
 * External vars
 ******************************************************************************/


/*******************************************************************************
 * Local vars
 ******************************************************************************/

    static uint32_t dato_adc;

/*******************************************************************************
 * Private Source Code
 ******************************************************************************/
    uint32_t sensor_luz(void){
            /*Configurar canal ADC por donde se desea lectura
         *dar señal de star al ADC*/
        ADC16_SetChannelConfig(ADC0_PERIPHERAL, ADC0_CH0_CONTROL_GROUP, &ADC0_channelsConfig[0]);
        /* Esperar que el ADC finalice el ADC */
        if (0U == (kADC16_ChannelConversionDoneFlag & ADC16_GetChannelStatusFlags(ADC0_PERIPHERAL, ADC0_CH0_CONTROL_GROUP))){

        }


        /*
         * dato_adc = 12 bits
         * VREFH = 3.3V
         * VREFL = 0V
         * Resistencia = 10k
         */

    	 dato_adc =ADC16_GetChannelConversionValue(ADC0_PERIPHERAL, ADC0_CH0_CONTROL_GROUP);
    	         return(dato_adc);
     }


/*******************************************************************************
 * Public Source Code
 ******************************************************************************/
