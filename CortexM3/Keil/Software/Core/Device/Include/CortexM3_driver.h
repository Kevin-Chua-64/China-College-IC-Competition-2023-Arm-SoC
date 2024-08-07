#include "CortexM3.h"

extern uint32_t  uart_init(uint32_t divider, uint32_t tx_en,
                           uint32_t rx_en, uint32_t tx_irq_en, uint32_t rx_irq_en, uint32_t tx_ovrirq_en, uint32_t rx_ovrirq_en);

  /**
   * @brief Returns whether the UART RX Buffer is Full.
   */

 extern uint32_t  uart_GetRxBufferFull(void);

  /**
   * @brief Returns whether the UART TX Buffer is Full.
   */

 extern uint32_t  uart_GetTxBufferFull(void);

  /**
   * @brief Sends a character to the UART TX Buffer.
   */


 extern void  uart_SendChar(char txchar);

  /**
   * @brief Receives a character from the UART RX Buffer.
   */

 extern char  uart_ReceiveChar(void);

  /**
   * @brief Returns UART Overrun status.
   */

 extern uint32_t  uart_GetOverrunStatus(void);

  /**
   * @brief Clears UART Overrun status Returns new UART Overrun status.
   */

 extern uint32_t  uart_ClearOverrunStatus(void);

  /**
   * @brief Returns UART Baud rate Divider value.
   */

 extern uint32_t  uart_GetBaudDivider(void);

  /**
   * @brief Return UART TX Interrupt Status.
   */

 extern uint32_t  uart_GetTxIRQStatus(void);

  /**
   * @brief Return UART RX Interrupt Status.
   */

 extern uint32_t  uart_GetRxIRQStatus(void);

  /**
   * @brief Clear UART TX Interrupt request.
   */

 extern void  uart_ClearTxIRQ(void);

  /**
   * @brief Clear UART RX Interrupt request.
   */

 extern void  uart_ClearRxIRQ(void);

  /**
   * @brief Set CM3DS_MPS2 Timer for multi-shoot mode with internal clock
   */
 extern void  uart_SendString(char *string);

/************************SysTick***********************************************/
 extern void delay(uint32_t time);
 extern void delay_us(uint32_t time);
 extern void delay_ms(uint32_t time);
 extern void Set_SysTick_CTRL(uint32_t ctrl);
 extern void Set_SysTick_LOAD(uint32_t load);
 extern uint32_t Read_SysTick_VALUE(void);
 extern void Set_SysTick_VALUE(uint32_t value);
 extern void Set_SysTick_CALIB(uint32_t calib);
 extern uint32_t SysTick_Ini(void);
 extern uint8_t SysTick_Stop(uint32_t *duration_t,uint32_t start_t);
 extern void SysCountDown(void);
 extern uint32_t ReadSystickIRQ(void);

/************************Timer*************************************************/
extern void Set_Timer0_CTRL(uint32_t ctrl);
extern void Set_Timer0_LOAD(uint32_t load);
extern uint32_t Read_Timer0_VALUE(void);
extern void Set_Timer0_VALUE(uint32_t value);
extern void Set_Timer1_CTRL(uint32_t ctrl);
extern void Set_Timer1_LOAD(uint32_t load);
extern uint32_t Read_Timer1_VALUE(void);
extern void Set_Timer1_VALUE(uint32_t value);
extern void Timer_Init(void);

/************************LCD***************************************************/
extern void LCD_Init(void);
extern void LCD_DisplayOn(void);
extern void LCD_DisplayOff(void);
extern void LCD_Clear(uint16_t Color);
extern void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);
extern void LCD_DrawPoint(uint16_t x,uint16_t y);
extern void LCD_Fast_DrawPoint(uint16_t x,uint16_t y,uint16_t color);
extern void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
extern void LCD_DrawLine_color(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
extern void LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color);
extern void LCD_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color);
extern void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode);
extern void LCD_ShowChar_color(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode,uint16_t pointColor,uint16_t backColor);
extern void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size);
extern void LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode);
extern void LCD_ShowxNum_color(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode,uint16_t pointColor,uint16_t backColor);
extern void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,char *p);
extern void LCD_ShowString_color(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,char *p,uint16_t pointColor,uint16_t backColor);

extern void LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue);
extern uint16_t LCD_ReadReg(uint16_t LCD_Reg);
extern void LCD_WriteRAM_Prepare(void);
extern void LCD_WriteRAM(uint16_t RGB_Code);
extern void LCD_Scan_Dir(uint8_t dir);
extern void LCD_Display_Dir(uint8_t dir);
extern void LCD_Set_Window(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height);

/************************Key***************************************************/
extern uint8_t keyScan(uint8_t mode);
