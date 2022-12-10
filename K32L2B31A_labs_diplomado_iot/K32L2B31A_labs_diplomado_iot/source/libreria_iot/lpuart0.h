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
#include "peripherals.h"
#include "fsl_lpuart.h"

/*!
 * @addtogroup IRQ
 * @{
 */
/*!
 * @addtogroup LPUART0
 * @{
 */
/*******************************************************************************
 * Public Definitions
 ******************************************************************************/
/*! @brief Tamaño de buffer circular para recibir datos por UART (Unit: Byte). */
#define LONGITUD_BUFFER_CIRCULAR 	100

/*******************************************************************************
 * External vars
 ******************************************************************************/

/*******************************************************************************
 * Public vars
 ******************************************************************************/

/*******************************************************************************
 * Public Prototypes
 ******************************************************************************/
/*--------------------------------------------*/
/*!
 * @brief Calcula el numero de datos nuevos que hay en el buffer circular
 *
 * @return	numero de bytes que estan pendientes por ser procesados
 * @endcode
 */
uint16_t lpUart0CuantosDatosHayEnBuffer(void);
/*--------------------------------------------*/
/*!
 * @brief Obtiene 1 byte desde buffer circular
 *
 * @param nuevo_byte	apuntador de memoria donde almacenar nuevo byte
 * @return	estado de la ejecución
 * @code
 * 		kStatus_Success
 * 		kStatus_Fail
 * @endcode
 */
status_t lpUart0LeerByteDesdeBuffer(uint8_t *nuevo_byte);
/** @} */ // end of LPUART0 group
/** @} */ // end of IRQ group

#endif /* LIBRERIA_IOT_LPUART0_H_ */
