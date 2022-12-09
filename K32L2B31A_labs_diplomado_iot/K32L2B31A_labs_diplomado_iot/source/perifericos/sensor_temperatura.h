/*
 * sensor_temperatura.h
 *
 *  Created on: 9/12/2022
 *      Author: victo
 */






/*! @file : sensor_temperature.c
 * @author  Victor Alfonso Fernandez Hoyos
 * @version 1.0.0
 * @date    09/12/2022
 * @brief   Driver para lectura de sensor de temperatura interna del microcontrolador
 * @details
 *
*/
#ifndef PERIFERICOS_SENSOR_TEMPERATURA_H_
#define PERIFERICOS_SENSOR_TEMPERATURA_H_
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "peripherals.h"

/*!
 * @addtogroup PERIPHERAL
 * @{
 */
/*!
 * @addtogroup TEMPERATURE
 * @{
 */
/*******************************************************************************
 * Public Definitions
 ******************************************************************************/
#define MAX_ADC			(float)65535
#define VDD_MAX			(float)(3.3)
#define VTEMP25			(float)(0.716)


#define ADC_VTEMP25		14219//(VTEMP25 * MAX_ADC)/(VDD_MAX)
#define ADC_VSLOPE		32//(float)(VTEMP25 * MAX_ADC)/(VDD_MAX)


/*******************************************************************************
 * External vars
 ******************************************************************************/

/*******************************************************************************
 * Public vars
 ******************************************************************************/

/*******************************************************************************
 * Public Prototypes
 ******************************************************************************/

 float getTemperatureValue(void);

/** @} */ // end of TEMPERATURE group
/** @} */ // end of PERIPHERAL group

#endif /* PERIFERICOS_SENSOR_TEMPERATURA_H_ */
