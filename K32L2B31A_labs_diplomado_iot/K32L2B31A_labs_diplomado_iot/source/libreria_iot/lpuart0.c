/*! @file : lpuart0.c
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
#include "lpuart0.h"
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "K32L2B31A.h"
#include "fsl_lpuart.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define LPUART0_BUFFER_SIZE_MAX 50

/*******************************************************************************
 * Private Prototypes
 ******************************************************************************/


/*******************************************************************************
 * External vars
 ******************************************************************************/


/*******************************************************************************
 * Local vars
 ******************************************************************************/
    static uint8_t dato_lpuart0[LPUART0_BUFFER_SIZE_MAX];
    static uint8_t flag_nuevo_dato_lpuart0 = 0;
    static uint8_t dato_lpuart0_index = 0;
    static uint16_t control_retardo = 0;
/*******************************************************************************
 * Private Source Code
 ******************************************************************************/
     /* LPUART0_IRQn interrupt handler */
    void LPUART0_SERIAL_RX_TX_IRQHANDLER(void) {
      uint32_t intStatus;
      /* Captura bandera de estado para el puerto lpuart0
       * esta lectura tambien borra las banderas de estado */
      intStatus = LPUART_GetStatusFlags(LPUART0_PERIPHERAL);


      /* verifica que la IRQ es por llegada de nuevo dato por RX */
      if ((kLPUART_RxDataRegFullFlag) & intStatus) {

      dato_lpuart0[dato_lpuart0_index] = LPUART_ReadByte(LPUART0);
      dato_lpuart0_index++;
      if(dato_lpuart0_index>LPUART0_BUFFER_SIZE_MAX){
    	  dato_lpuart0_index=0;
      }
      flag_nuevo_dato_lpuart0=1;

      }

      /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
         Store immediate overlapping exception return operation might vector to incorrect interrupt. */
      #if defined __CORTEX_M && (__CORTEX_M == 4U)
        __DSB();
      #endif
    }

/*******************************************************************************
 * Public Source Code
 ******************************************************************************/
    uint8_t leer_dato(void){
    if (dato_lpuart0_index!=0){
    	return(dato_lpuart0[dato_lpuart0_index-1]);
    }else{
    	return(0x00);
    }
   }

uint8_t* lectura_buffer(void){
    retardo(10000);
    return dato_lpuart0;
   }


 uint8_t leer_bandera_nuevo_dato(void){
	return(flag_nuevo_dato_lpuart0);
}

 void escribir_bandera_nuevo_dato(uint8_t nuevo_valor){
	flag_nuevo_dato_lpuart0 = nuevo_valor;
}
 void lpuart0_borrar_buffer (void){
	 for(uint8_t i; i<LPUART0_BUFFER_SIZE_MAX; i++){
		 dato_lpuart0[i]=0x00;
	 }
	 dato_lpuart0_index=0;
 }
 void retardo(uint16_t retardo_mensaje){
	 retardo_mensaje = retardo_mensaje ? retardo_mensaje: 10000;
	 while (control_retardo<retardo_mensaje){
		 control_retardo++;
	 }
	 control_retardo=0;
 }
