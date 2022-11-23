/*! @file : lpuart0.h
 * @author  Victor Alfonso Fernandez Hoyos
 * @version 1.0.0
 * @date    18/11/2022
 * @brief   Driver para 
 * @details
 *
 */
#ifndef LIBRERIA_IOT_LPUART0_H_
#define LIBRERIA_IOT_LPUART0_H_
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fsl_common.h"
#include "fsl_lpuart.h"
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

/*******************************************************************************
 * External vars
 ******************************************************************************/

/*******************************************************************************
 * Public vars
 ******************************************************************************/

/*******************************************************************************
 * Public Prototypes
 ******************************************************************************/
uint8_t leer_dato(void);
uint8_t leer_bandera_nuevo_dato(void);
void escribir_bandera_nuevo_dato(uint8_t nuevo_valor);
void lpuart0_borrar_buffer (void);
void retardo(uint16_t retardo_mensaje);
uint8_t* lectura_buffer(void);

/** @} */ // end of X group
/** @} */ // end of X group

#endif /* LIBRERIA_IOT_LPUART0_H_ */
