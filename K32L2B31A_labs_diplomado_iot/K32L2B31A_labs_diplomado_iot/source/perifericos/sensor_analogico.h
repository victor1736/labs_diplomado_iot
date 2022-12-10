/*! @file : sensor_analogico.h
 * @author  Victor Alfonso Fernandez Hoyos
 * @version 1.0.0
 * @date    09/12/2022
 * @brief   Driver para cualquier sensor analogico
 * @details
 *
 */
#ifndef PERIFERICOS_SENSOR_ANALOGICO_H_
#define PERIFERICOS_SENSOR_ANALOGICO_H_
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "peripherals.h"

/*!
 * @addtogroup PERIPHERALS
 * @{
 */
/*!
 * @addtogroup SENSOR_ANA
 * @{
 */
/*******************************************************************************
 * Public Definitions
 ******************************************************************************/
#define INDEX_SENSOR_ANA	3	//PTB0 ADC0_SE8
/*******************************************************************************
 * External vars
 ******************************************************************************/

/*******************************************************************************
 * Public vars
 ******************************************************************************/

/*******************************************************************************
 * Public Prototypes
 ******************************************************************************/
uint32_t getSensorADC(void);

/** @} */ // end of SENSOR_ANA group
/** @} */ // end of PERIPHERALS group

#endif /* PERIFERICOS_SENSOR_ANALOGICO_H_ */
