/*
 * @file   	 	:K32L2B31A_labs_diplomado_iot.c
 * @author 		:Victor Alfonso Fernandez Hoyos
 * @version		:0.0.000
 * @date		:08/11/2022
 * @brief   	:Funcion principal main
 * @details
 * 				v0.0.000  	Proyecto base creado usando MCUXpresso
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
#define BOARD_LED_GPIO1     BOARD_LED_RED_GPIO
#define BOARD_LED_GPIO_PIN1 BOARD_LED_RED_GPIO_PIN
#define BOARD_LED_GPIO2     BOARD_LED_GREEN_GPIO
#define BOARD_LED_GPIO_PIN2 BOARD_LED_GREEN_GPIO_PIN
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
    volatile uint32_t g_systickCounter;
/******************************************************************************************
 * Private Source Code
 *****************************************************************************************/
    void SysTick_Handler(void)
    {
        if (g_systickCounter != 0U)
        {
            g_systickCounter--;
        }
    }

    void SysTick_DelayTicks(uint32_t n)
    {
        g_systickCounter = n;
        while (g_systickCounter != 0U)
        {
        }
    }
int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif
    if (SysTick_Config(SystemCoreClock / 1000U))
    {
        while (1)
        {
        }
    }

    PRINTF("Hola mundo\r\n");
    PRINTF("Voltaje: %2.3f\r\n",voltaje);

    /* Force the counter to be placed into memory. */
    volatile static int i = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
        i++ ;
        /* Delay 1000 ms */
        SysTick_DelayTicks(1000U);
        GPIO_PortToggle(BOARD_LED_GPIO1, 1u << BOARD_LED_GPIO_PIN1);
        SysTick_DelayTicks(1000U);
        GPIO_PortToggle(BOARD_LED_GPIO2, 1u << BOARD_LED_GPIO_PIN2);
        /* 'Dummy' NOP to allow source level single stepping of
            tight while() loop */
        __asm volatile ("nop");
    }
    return 0 ;
}
