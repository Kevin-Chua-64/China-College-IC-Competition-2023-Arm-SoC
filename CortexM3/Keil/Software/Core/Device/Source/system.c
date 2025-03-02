#include "CortexM3.h"

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
#define __XTAL            (50000000UL)    /* Oscillator frequency             */

#define __SYSTEM_CLOCK    (__XTAL)


/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock = __SYSTEM_CLOCK;/*!< System Clock Frequency (Core Clock)*/


/*----------------------------------------------------------------------------
  Clock functions
 *----------------------------------------------------------------------------*/
/**
 * Update SystemCoreClock variable
 *
 * @param  none
 * @return none
 *
 * @brief  Updates the SystemCoreClock with current core Clock
 *         retrieved from cpu registers.
 */
void SystemCoreClockUpdate (void)
{

  SystemCoreClock = __SYSTEM_CLOCK;

}

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System.
 */
void SystemInit (void)
{

  SystemCoreClock = __SYSTEM_CLOCK;  

  NVIC_EnableIRQ(SysTick_IRQn);
  NVIC_EnableIRQ(TIMER0_IRQn);
  NVIC_EnableIRQ(TIMER1_IRQn);
	
  uart_init ( (SystemCoreClock / 9600), 1,1,0,0,0,0 );
		
	// LCD init
  LCD->LCD_RST = 0;
	delay(5000000);
	LCD->LCD_RST = 1;
  LCD_Init();
}
