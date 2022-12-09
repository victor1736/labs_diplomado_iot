/*! @file : EC200T.h
 * @author  Victor Alfonso Fernandez Hoyos
 * @version 1.0.0
 * @date    09/12/2022
 * @brief   Driver para modem EC200T
 * @details
 *
*/
#ifndef PERIFERICOS_EC200T_H_
#define PERIFERICOS_EC200T_H_
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "stdio.h"
#include "lpuart0.h"

/*!
 * @addtogroup PPH
 * @{
 */
/*!
 * @addtogroup EC25AU
 * @{
 */
/*******************************************************************************
 * Public Definitions
 ******************************************************************************/
enum _ec200t_lista_comandos_at {
	kAT = 0,
	kATI,
	kAT_CPIN,
	kAT_CREG,
	kAT_CMGF_1,
	kAT_CMGS,
	kAT_TEXT_MSG_END,
};

enum _fsm_ec200t_state{
	kFSM_INICIO=0,
	kFSM_ENVIANDO_AT,
	kFSM_ENVIANDO_ATI,
	kFSM_ENVIANDO_CPIN,
	kFSM_ENVIANDO_CREG,
	kFSM_ENVIANDO_CMGF,
	kFSM_ENVIANDO_CMGS,
	kFSM_ENVIANDO_MENSAJE_TXT,
	kFSM_ESPERANDO_RESPUESTA,
	kFSM_RESULTADO_ERROR,
	kFSM_RESULTADO_EXITOSO
};
/*******************************************************************************
 * External vars
 ******************************************************************************/

/*******************************************************************************
 * Public vars
 ******************************************************************************/

/*******************************************************************************
 * Public Prototypes
 ******************************************************************************/
status_t ec200tInicializacion(void);
status_t ec200tEnviarMensajeDeTexto(uint8_t *mensaje, uint8_t size_mensaje );
uint8_t  ec200tPolling(void);

/** @} */ // end of X group
/** @} */ // end of X group

#endif /* PERIFERICOS_EC200T_H_ */
