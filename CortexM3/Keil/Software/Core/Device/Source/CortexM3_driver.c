#include <string.h>
#include "CortexM3.h"

/**
 *
 * @brief  Initialises the UART specifying the UART Baud rate divider value and whether the send and recieve functionality is enabled. It also specifies which of the various interrupts are enabled.
 *
 * @param *uart UART Pointer
 * @param divider The value to which the UART baud rate divider is to be set
 * @param tx_en Defines whether the UART transmit is to be enabled
 * @param rx_en Defines whether the UART receive is to be enabled
 * @param tx_irq_en Defines whether the UART transmit buffer full interrupt is to be enabled
 * @param rx_irq_en Defines whether the UART receive buffer full interrupt is to be enabled
 * @param tx_ovrirq_en Defines whether the UART transmit buffer overrun interrupt is to be enabled
 * @param rx_ovrirq_en Defines whether the UART receive buffer overrun interrupt is to be enabled
 * @return 1 if initialisation failed, 0 if successful.
 */

 uint32_t  uart_init(uint32_t divider, uint32_t tx_en,
                           uint32_t rx_en, uint32_t tx_irq_en, uint32_t rx_irq_en, uint32_t tx_ovrirq_en, uint32_t rx_ovrirq_en)
 {
       uint32_t new_ctrl=0;

       if (tx_en!=0)        new_ctrl |=  UART_CTRL_TXEN_Msk;
       if (rx_en!=0)        new_ctrl |=  UART_CTRL_RXEN_Msk;
       if (tx_irq_en!=0)    new_ctrl |=  UART_CTRL_TXIRQEN_Msk;
       if (rx_irq_en!=0)    new_ctrl |=  UART_CTRL_RXIRQEN_Msk;
       if (tx_ovrirq_en!=0) new_ctrl |=  UART_CTRL_TXORIRQEN_Msk;
       if (rx_ovrirq_en!=0) new_ctrl |=  UART_CTRL_RXORIRQEN_Msk;

        UART->CTRL = 0;         /* Disable UART when changing configuration */
        UART->BAUDDIV = divider;
        UART->CTRL = new_ctrl;  /* Update CTRL register to new value */

       if(( UART->STATE & ( UART_STATE_RXOR_Msk |  UART_STATE_TXOR_Msk))) return 1;
       else return 0;
 }

/**
 *
 * @param *uart UART Pointer
 * @return RxBufferFull
 *
 * @brief  Returns whether the RX buffer is full.
 */

 uint32_t  uart_GetRxBufferFull(void)
 {
        return (( UART->STATE &  UART_STATE_RXBF_Msk)>>  UART_STATE_RXBF_Pos);
 }

/**
 *
 * @param *uart UART Pointer
 * @return TxBufferFull
 *
 * @brief  Returns whether the TX buffer is full.
 */

 uint32_t  uart_GetTxBufferFull(void)
 {
        return (( UART->STATE &  UART_STATE_TXBF_Msk)>>  UART_STATE_TXBF_Pos);
 }

/**
 *
 * @param *uart UART Pointer
 * @param txchar Character to be sent
 * @return none
 *
 * @brief  Sends a character to the TX buffer for transmission.
 */

 void  uart_SendChar(char txchar)
 {
       while( 1 ){
              if(!(UART->STATE &  UART_STATE_TXBF_Msk)) break;
       };
        UART->DATA = (uint32_t)txchar;
 }

/**
 *
 * @param *uart UART Pointer
 * @return rxchar
 *
 * @brief  returns the character from the RX buffer which has been received.
 */

 char  uart_ReceiveChar(void)
 {
       while(!( UART->STATE &  UART_STATE_RXBF_Msk));
       return (char)( UART->DATA);
 }

/**
 *
 * @param *uart UART Pointer
 * @return 0 - No overrun
 * @return 1 - TX overrun
 * @return 2 - RX overrun
 * @return 3 - TX & RX overrun
 *
 * @brief  returns the current overrun status of both the RX & TX buffers.
 */


 uint32_t  uart_GetOverrunStatus(void)
 {
        return (( UART->STATE & ( UART_STATE_RXOR_Msk |  UART_STATE_TXOR_Msk))>> UART_STATE_TXOR_Pos);
 }

/**
 *
 * @param *uart UART Pointer
 * @return 0 - No overrun
 * @return 1 - TX overrun
 * @return 2 - RX overrun
 * @return 3 - TX & RX overrun
 *
 * @brief  Clears the overrun status of both the RX & TX buffers and then returns the current overrun status.
 */

 uint32_t  uart_ClearOverrunStatus(void)
 {
        UART->STATE = ( UART_STATE_RXOR_Msk |  UART_STATE_TXOR_Msk);
        return (( UART->STATE & ( UART_STATE_RXOR_Msk |  UART_STATE_TXOR_Msk))>> UART_STATE_TXOR_Pos);
 }

/**
 *
 * @param *uart UART Pointer
 * @return BaudDiv
 *
 * @brief  Returns the current UART Baud rate divider. Note that the Baud rate divider is the difference between the clock frequency and the Baud frequency.
 */

 uint32_t  uart_GetBaudDivider(void)
 {
       return  UART->BAUDDIV;
 }

 /**
 *
 * @param *uart UART Pointer
 * @return TXStatus
 *
 * @brief  Returns the TX interrupt status.
 */

 uint32_t  uart_GetTxIRQStatus(void)
 {
       return (( UART->INTSTATUS &  UART_CTRL_TXIRQ_Msk)>> UART_CTRL_TXIRQ_Pos);
 }

/**
 *
 * @param *uart UART Pointer
 * @return RXStatus
 *
 * @brief  Returns the RX interrupt status.
 */

 uint32_t  uart_GetRxIRQStatus(void)
 {
       return (( UART->INTSTATUS &  UART_CTRL_RXIRQ_Msk)>> UART_CTRL_RXIRQ_Pos);
 }

 /**
 *
 * @param *uart UART Pointer
 * @return none
 *
 * @brief  Clears the TX buffer full interrupt status.
 */

 void  uart_ClearTxIRQ(void)
 {
        UART->INTCLEAR =  UART_CTRL_TXIRQ_Msk;
 }

/**
 *
 * @param *uart UART Pointer
 * @return none
 *
 * @brief  Clears the RX interrupt status.
 */

 void  uart_ClearRxIRQ(void)
 {
        UART->INTCLEAR =  UART_CTRL_RXIRQ_Msk;
 }

 void  uart_SendString(char *string) {
        uint32_t length,i;
        length = strlen(string);
        for(i = 0;i < length;i++) {
               uart_SendChar(string[i]);
        }
 }


/************************SysTick***********************************************/
void Set_SysTick_CTRL(uint32_t ctrl)
{
	SysTick->CTRL = ctrl;
}

void Set_SysTick_LOAD(uint32_t load)
{
	SysTick->LOAD = load;
}

uint32_t Read_SysTick_VALUE(void)
{
	return(SysTick->VALUE);
}

void Set_SysTick_CALIB(uint32_t calib)
{
	SysTick->CALIB = calib;
}

void Set_SysTick_VALUE(uint32_t value)
{
	SysTick->VALUE = value;
}

uint32_t SysTick_Ini(void)
{
	SysTick->CTRL = 0;
	SysTick->LOAD = 0xffffff;
	SysTick->VALUE = 0;
	SysTick->CTRL = 0x5;
	while(SysTick->VALUE == 0);
	return(SysTick->VALUE);
}
uint8_t SysTick_Stop(uint32_t *duration_t,uint32_t start_t)
{
	uint32_t stop_t;
	stop_t = SysTick->VALUE;
	if((SysTick->CTRL & 0x10000) == 0)
	{
		*duration_t = start_t - stop_t;
		return(1);
	}
	else
	{
		return(0);
	}
}

void delay(uint32_t time)
{
	Set_SysTick_CTRL(0);
	Set_SysTick_LOAD(time);
	Set_SysTick_VALUE(0);
	Set_SysTick_CTRL(0x7);
	__wfi();
}

void delay_us(uint32_t time)  // 最高8位无法重装载，utime < 335544
{
	Set_SysTick_CTRL(0);
	Set_SysTick_LOAD(50*time-1);
	Set_SysTick_VALUE(0);
	Set_SysTick_CTRL(0x7);
	__wfi();
}

void delay_ms(uint32_t time)
{
	uint32_t i;
	for (i=0;i<time;i++)
		delay_us(1000);
}

void SysCountDown(void){
	Set_SysTick_CTRL(0);
	Set_SysTick_LOAD(0xFFFFFF);
	Set_SysTick_VALUE(0);
	Set_SysTick_CTRL(0x7);
}

uint32_t ReadSystickIRQ(void){
  return (SysTick->CTRL & 0x10000);
}


/************************Timer*************************************************/
void Set_Timer0_CTRL(uint32_t ctrl)
{
	Timer0->CTRL = ctrl;
}

void Set_Timer0_LOAD(uint32_t load)
{
	Timer0->LOAD = load;
}

uint32_t Read_Timer0_VALUE(void)
{
	return(Timer0->VALUE);
}

void Set_Timer0_VALUE(uint32_t value)
{
	Timer0->VALUE = value;
}

void Set_Timer1_CTRL(uint32_t ctrl)
{
	Timer1->CTRL = ctrl;
}

void Set_Timer1_LOAD(uint32_t load)
{
	Timer1->LOAD = load;
}

uint32_t Read_Timer1_VALUE(void)
{
	return(Timer1->VALUE);
}

void Set_Timer1_VALUE(uint32_t value)
{
	Timer1->VALUE = value;
}

// 初始化定时器参数
_timer_para timerPara;
void Timer_Para_Init(void)
{
	timerPara.cnt   = 0;
	timerPara.index = 42;
}

// 50MHz内部时钟
void Timer_Init(void)
{
	Timer_Para_Init();
	Timer0->CTRL  = 0;
	Timer0->LOAD  = 500000;  // 10ms
	Timer0->VALUE = 0;
	Timer0->CTRL  = 0b1001;  // 使能Timer0和中断，不使用外部时钟
}


/************************LCD***************************************************/
#include "lcdfont.h"

_lcd_dev lcddev;

void MakeData( uint16_t data ) {
    LCD->LCD_DATA[0] = ( data >> 0 ) & 1 ;
    LCD->LCD_DATA[1] = ( data >> 1 ) & 1 ;
    LCD->LCD_DATA[2] = ( data >> 2 ) & 1 ;
    LCD->LCD_DATA[3] = ( data >> 3 ) & 1 ;
    LCD->LCD_DATA[4] = ( data >> 4 ) & 1 ;
    LCD->LCD_DATA[5] = ( data >> 5 ) & 1 ;
    LCD->LCD_DATA[6] = ( data >> 6 ) & 1 ;
    LCD->LCD_DATA[7] = ( data >> 7 ) & 1 ;
    LCD->LCD_DATA[8] = ( data >> 8 ) & 1 ;
    LCD->LCD_DATA[9] = ( data >> 9 ) & 1 ;
    LCD->LCD_DATA[10] = ( data >> 10 ) & 1;
    LCD->LCD_DATA[11] = ( data >> 11 ) & 1;
    LCD->LCD_DATA[12] = ( data >> 12 ) & 1;
    LCD->LCD_DATA[13] = ( data >> 13 ) & 1;
    LCD->LCD_DATA[14] = ( data >> 14 ) & 1;
    LCD->LCD_DATA[15] = ( data >> 15 ) & 1;
}

//lcd读数据
uint16_t ReadData() {
	uint16_t ans = 0;
	for (int i = 0; i < 16; i++) {
		if (LCD->LCD_DATA[i] == 1)
			ans += (1 << i);
	}
	return ans;
}

//写寄存器函数
//data:寄存器值
void LCD_WR_REG( uint16_t data ) {
  LCD_RS_CLR; 
 	LCD_CS_CLR; 
	delay(500);
	MakeData( data );
	LCD_WR_CLR;
	delay(500);
	LCD_WR_SET; 
 	LCD_CS_SET;         
}

//写数据函数
//可以替代LCD_WR_DATAX宏,拿时间换空间.
//data:寄存器值
void LCD_WR_DATAX( uint16_t data ) {
  LCD_RS_SET;
	LCD_CS_CLR;
	delay(500);
	MakeData( data );
	LCD_WR_CLR;
	delay(500);
	LCD_WR_SET;
	LCD_CS_SET;
}

//读LCD数据
//返回值:读到的值
uint16_t LCD_RD_DATA(void)
{										   
	uint16_t t;

	LCD_RS_SET;
	LCD_CS_CLR;
	LCD_RD_CLR;			   
	t=ReadData();
	delay(500);  
	LCD_RD_SET;
	LCD_CS_SET; 

	return t;  
}

//写寄存器
//LCD_Reg:寄存器编号
//LCD_RegValue:要写入的值
void LCD_WriteReg( uint16_t LCD_Reg, uint16_t LCD_RegValue ) {
	LCD_WR_REG( LCD_Reg );  
	LCD_WR_DATA( LCD_RegValue );	    		 
} 

//读寄存器
//LCD_Reg:寄存器编号
//返回值:读到的值
uint16_t LCD_ReadReg(uint16_t LCD_Reg)
{										   
 	LCD_WR_REG(LCD_Reg);   
	delay(500);
	return LCD_RD_DATA(); 
}

//开始写GRAM
void LCD_WriteRAM_Prepare( void ) {
    LCD_WR_REG( lcddev.wramcmd );
}

//LCD写GRAM
void LCD_WriteRAM( uint16_t RGB_Code ) {
    LCD_WR_DATA( RGB_Code );
}

void opt_delay( uint8_t t ) {
    while ( t-- );
}

//LCD开启显示
void LCD_DisplayOn( void ) {
    LCD_WR_REG( 0x29 );
}

//LCD关闭显示
void LCD_DisplayOff( void ) {
    LCD_WR_REG( 0x28 );
}

//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
void LCD_SetCursor( uint16_t Xpos, uint16_t Ypos ) {
  LCD_WR_REG( lcddev.setxcmd ); 
	LCD_WR_DATA( Xpos >> 8 );
  LCD_WR_DATA( Xpos & 0XFF ); 			 
	LCD_WR_REG( lcddev.setycmd ); 
	LCD_WR_DATA( Ypos >> 8 );
  LCD_WR_DATA( Ypos & 0XFF ); 
}

//设置LCD的自动扫描方向
//注意:其他函数可能会受到此函数设置的影响(尤其是9341/6804这两个奇葩),
//所以,一般设置为L2R_U2D即可,如果设置为其他扫描方式,可能导致显示不正常.
//dir:0~7,代表8个方向(具体定义见lcd.h)
//9320/9325/9328/4531/4535/1505/b505/5408/9341/5310/5510/1963等IC已经实际测试	   	   
void LCD_Scan_Dir(uint8_t dir)
{
	uint16_t regval=0;
	uint16_t dirreg=0;
	uint16_t temp;  
	if((lcddev.dir==1&&lcddev.id!=0X6804&&lcddev.id!=0X1963)||(lcddev.dir==0&&lcddev.id==0X1963))
	{			   
		switch(dir)
		{
			case 0:dir=6;break;
			case 1:dir=7;break;
			case 2:dir=4;break;
			case 3:dir=5;break;
			case 4:dir=1;break;
			case 5:dir=0;break;
			case 6:dir=3;break;
			case 7:dir=2;break;	     
		}
	} 
	if(lcddev.id==0x9341||lcddev.id==0X6804||lcddev.id==0X5310||lcddev.id==0X5510||lcddev.id==0X1963)//9341/6804/5310/5510/1963
	{
		switch(dir)
		{
			case L2R_U2D:
				regval|=(0<<7)|(0<<6)|(0<<5); 
				break;
			case L2R_D2U:
				regval|=(1<<7)|(0<<6)|(0<<5); 
				break;
			case R2L_U2D:
				regval|=(0<<7)|(1<<6)|(0<<5); 
				break;
			case R2L_D2U:
				regval|=(1<<7)|(1<<6)|(0<<5); 
				break;	 
			case U2D_L2R:
				regval|=(0<<7)|(0<<6)|(1<<5); 
				break;
			case U2D_R2L://′óé?μ???,′óóòμ?×ó
				regval|=(0<<7)|(1<<6)|(1<<5); 
				break;
			case D2U_L2R://′ó??μ?é?,′ó×óμ?óò
				regval|=(1<<7)|(0<<6)|(1<<5); 
				break;
			case D2U_R2L://′ó??μ?é?,′óóòμ?×ó
				regval|=(1<<7)|(1<<6)|(1<<5); 
				break;	 
		}
		if(lcddev.id==0X5510)dirreg=0X3600;
		else dirreg=0X36;
 		if((lcddev.id!=0X5310)&&(lcddev.id!=0X5510)&&(lcddev.id!=0X1963))regval|=0X08;//5310/5510/19632?DèòaBGR   
		if(lcddev.id==0X6804)regval|=0x02;//6804μ?BIT6oí9341μ?·′á?	   
		LCD_WriteReg(dirreg,regval);
		if(lcddev.id!=0X1963)//19632?×?×?±ê′|àí
		{
			if(regval&0X20)
			{
				if(lcddev.width<lcddev.height)//????X,Y
				{
					temp=lcddev.width;
					lcddev.width=lcddev.height;
					lcddev.height=temp;
				}
			}else  
			{
				if(lcddev.width>lcddev.height)//????X,Y
				{
					temp=lcddev.width;
					lcddev.width=lcddev.height;
					lcddev.height=temp;
				}
			}  
		}
		if(lcddev.id==0X5510)
		{
			LCD_WR_REG(lcddev.setxcmd);LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setxcmd+1);LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setxcmd+2);LCD_WR_DATA((lcddev.width-1)>>8); 
			LCD_WR_REG(lcddev.setxcmd+3);LCD_WR_DATA((lcddev.width-1)&0XFF); 
			LCD_WR_REG(lcddev.setycmd);LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setycmd+1);LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setycmd+2);LCD_WR_DATA((lcddev.height-1)>>8); 
			LCD_WR_REG(lcddev.setycmd+3);LCD_WR_DATA((lcddev.height-1)&0XFF);
		}else
		{
			LCD_WR_REG(lcddev.setxcmd); 
			LCD_WR_DATA(0);LCD_WR_DATA(0);
			LCD_WR_DATA((lcddev.width-1)>>8);LCD_WR_DATA((lcddev.width-1)&0XFF);
			LCD_WR_REG(lcddev.setycmd); 
			LCD_WR_DATA(0);LCD_WR_DATA(0);
			LCD_WR_DATA((lcddev.height-1)>>8);LCD_WR_DATA((lcddev.height-1)&0XFF);  
		}
  	}else 
	{
		switch(dir)
		{
			case L2R_U2D:
				regval|=(1<<5)|(1<<4)|(0<<3); 
				break;
			case L2R_D2U:
				regval|=(0<<5)|(1<<4)|(0<<3); 
				break;
			case R2L_U2D:
				regval|=(1<<5)|(0<<4)|(0<<3);
				break;
			case R2L_D2U:
				regval|=(0<<5)|(0<<4)|(0<<3); 
				break;	 
			case U2D_L2R:
				regval|=(1<<5)|(1<<4)|(1<<3); 
				break;
			case U2D_R2L:
				regval|=(1<<5)|(0<<4)|(1<<3); 
				break;
			case D2U_L2R:
				regval|=(0<<5)|(1<<4)|(1<<3); 
				break;
			case D2U_R2L:
				regval|=(0<<5)|(0<<4)|(1<<3); 
				break;	 
		} 
		dirreg=0X03;
		regval|=1<<12; 
		LCD_WriteReg(dirreg,regval);
	}
}     

//画点
//x,y:坐标
//POINT_COLOR:此点的颜色
void LCD_DrawPoint(uint16_t x,uint16_t y)
{
	LCD_SetCursor(x,y);		
	LCD_WriteRAM_Prepare();	
	LCD_WR_DATA(POINT_COLOR); 
}	 

//快速画点
//x,y:坐标
//color:颜色
void LCD_Fast_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{	   
	LCD_WR_REG(lcddev.setxcmd); 
	LCD_WR_DATA(x>>8);LCD_WR_DATA(x&0XFF);  			 
	LCD_WR_REG(lcddev.setycmd); 
	LCD_WR_DATA(y>>8);LCD_WR_DATA(y&0XFF);
	LCD_RS_CLR;
 	LCD_CS_CLR; 
	MakeData(lcddev.wramcmd); 
	LCD_WR_CLR; 
	LCD_WR_SET; 
 	LCD_CS_SET;
	
	LCD_WR_DATA(color);		
}

//设置LCD显示方向
//dir:0,竖屏；1,横屏
void LCD_Display_Dir(uint8_t dir)
{
	if(dir==0)			
	{
		lcddev.dir=0;	
		lcddev.width=240;
		lcddev.height=320;
		lcddev.wramcmd=0X2C;
	 	lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;  	 
		if(lcddev.id==0X6804||lcddev.id==0X5310)
		{
			lcddev.width=320;
			lcddev.height=480;
		}
		
	}else 				
	{	  				
		lcddev.dir=1;	
		lcddev.width=320;
		lcddev.height=240;
		lcddev.wramcmd=0X2C;
	 	lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;  
	} 
	LCD_Scan_Dir(DFT_SCAN_DIR);	
}	 

//设置窗口,并自动设置画点坐标到窗口左上角(sx,sy).
//sx,sy:窗口起始坐标(左上角)
//width,height:窗口宽度和高度,必须大于0!!
//窗体大小:width*height. 
void LCD_Set_Window(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height)
{    
	uint16_t twidth,theight;
	twidth=sx+width-1;
	theight=sy+height-1;
	LCD_WR_REG(lcddev.setxcmd); 
	LCD_WR_DATA(sx>>8); 
	LCD_WR_DATA(sx&0XFF);	 
	LCD_WR_DATA(twidth>>8); 
	LCD_WR_DATA(twidth&0XFF);  
	LCD_WR_REG(lcddev.setycmd); 
	LCD_WR_DATA(sy>>8); 
	LCD_WR_DATA(sy&0XFF); 
	LCD_WR_DATA(theight>>8); 
	LCD_WR_DATA(theight&0XFF); 
	
}

//清屏函数
//color:要清屏的填充色
void LCD_Clear(uint16_t color)
{
	uint32_t index=0;      
	uint32_t totalpoint=lcddev.width;
	totalpoint*=lcddev.height; 			
	if((lcddev.id==0X6804)&&(lcddev.dir==1))
	{						    
 		lcddev.dir=0;	 
 		lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;  	 			
		LCD_SetCursor(0x00,0x0000);		  
 		lcddev.dir=1;	 
  		lcddev.setxcmd=0X2B;
		lcddev.setycmd=0X2A;  	 
 	}else LCD_SetCursor(0x00,0x0000);	 
	LCD_WriteRAM_Prepare();     			  	  
	for(index=0;index<totalpoint;index++)LCD_WR_DATA(color);	
}  

void LCD_Init(void)
{ 
	delay(2500000); // delay 50 ms 
	
	LCD_WR_REG(0XD3);
	lcddev.id = LCD_RD_DATA();          //dummy read
	lcddev.id = LCD_RD_DATA();          //读到0X00
	lcddev.id = LCD_RD_DATA();          //读取0X93
	lcddev.id <<= 8;
	lcddev.id |= LCD_RD_DATA();         //读取0X41
	
	if (lcddev.id != 0X9341)            //不是 9341 , 尝试看看是不是 ST7789
	{
			LCD_WR_REG(0X04);
			lcddev.id = LCD_RD_DATA();      //dummy read
			lcddev.id = LCD_RD_DATA();      //读到0X85
			lcddev.id = LCD_RD_DATA();      //读取0X85
			lcddev.id <<= 8;
			lcddev.id |= LCD_RD_DATA();     //读取0X52
			
			if (lcddev.id == 0X8552)        //将8552的ID转换成7789
			{
					lcddev.id = 0x7789;
			}
			
			if (lcddev.id != 0x7789)        //也不是ST7789, 尝试是不是 NT35310
			{
					LCD_WR_REG(0XD4);
					lcddev.id = LCD_RD_DATA();  //dummy read
					lcddev.id = LCD_RD_DATA();  //读回0X01
					lcddev.id = LCD_RD_DATA();  //读回0X53
					lcddev.id <<= 8;
					lcddev.id |= LCD_RD_DATA(); //这里读回0X10

					if (lcddev.id != 0X5310)    //也不是NT35310,尝试看看是不是NT35510
					{
							//发送秘钥（厂家提供,照搬即可）
							LCD_WriteReg(0xF000, 0x0055);
							LCD_WriteReg(0xF001, 0x00AA);
							LCD_WriteReg(0xF002, 0x0052);
							LCD_WriteReg(0xF003, 0x0008);
							LCD_WriteReg(0xF004, 0x0001);

							LCD_WR_REG(0xC500);             //读取ID高8位
							lcddev.id = LCD_RD_DATA();      //读回0X55
							lcddev.id <<= 8;

							LCD_WR_REG(0xC501);             //读取ID低8位
							lcddev.id |= LCD_RD_DATA();     //读回0X10
							delay(250000);
							
							if (lcddev.id != 0X5510)        //也不是NT5510,尝试看看是不是SSD1963
							{
									LCD_WR_REG(0XA1);
									lcddev.id = LCD_RD_DATA();
									lcddev.id = LCD_RD_DATA();  //读回0X57
									lcddev.id <<= 8;
									lcddev.id |= LCD_RD_DATA(); //读回0X61

									if (lcddev.id == 0X5761)lcddev.id = 0X1963; //SSD1963读回的ID是5761H,为方便区分,我们强制设置为1963
							}
					}
			}
	}
	
	//printf("id: %#X",lcddev.id);
	
	lcddev.id = 0X9341;
	
	if(lcddev.id==0X9341)	//9341
	{	 
		LCD_WR_REG(0xCF);  
		LCD_WR_DATAX(0x00); 
		LCD_WR_DATAX(0xC1); 
		LCD_WR_DATAX(0X30); 
		LCD_WR_REG(0xED);  
		LCD_WR_DATAX(0x64); 
		LCD_WR_DATAX(0x03); 
		LCD_WR_DATAX(0X12); 
		LCD_WR_DATAX(0X81); 
		LCD_WR_REG(0xE8);  
		LCD_WR_DATAX(0x85); 
		LCD_WR_DATAX(0x10); 
		LCD_WR_DATAX(0x7A); 
		LCD_WR_REG(0xCB);  
		LCD_WR_DATAX(0x39); 
		LCD_WR_DATAX(0x2C); 
		LCD_WR_DATAX(0x00); 
		LCD_WR_DATAX(0x34); 
		LCD_WR_DATAX(0x02); 
		LCD_WR_REG(0xF7);  
		LCD_WR_DATAX(0x20); 
		LCD_WR_REG(0xEA);  
		LCD_WR_DATAX(0x00); 
		LCD_WR_DATAX(0x00); 
		LCD_WR_REG(0xC0);    //Power control 
		LCD_WR_DATAX(0x1B);   //VRH[5:0] 
		LCD_WR_REG(0xC1);    //Power control 
		LCD_WR_DATAX(0x01);   //SAP[2:0];BT[3:0] 
		LCD_WR_REG(0xC5);    //VCM control 
		LCD_WR_DATAX(0x30); 	 //3F
		LCD_WR_DATAX(0x30); 	 //3C
		LCD_WR_REG(0xC7);    //VCM control2 
		LCD_WR_DATAX(0XB7); 
		LCD_WR_REG(0x36);    // Memory Access Control 
		LCD_WR_DATAX(0x48); 
		LCD_WR_REG(0x3A);   
		LCD_WR_DATAX(0x55); 
		LCD_WR_REG(0xB1);   
		LCD_WR_DATAX(0x00);   
		LCD_WR_DATAX(0x1A); 
		LCD_WR_REG(0xB6);    // Display Function Control 
		LCD_WR_DATAX(0x0A); 
		LCD_WR_DATAX(0xA2); 
		LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
		LCD_WR_DATAX(0x00); 
		LCD_WR_REG(0x26);    //Gamma curve selected 
		LCD_WR_DATAX(0x01); 
		LCD_WR_REG(0xE0);    //Set Gamma 
		LCD_WR_DATAX(0x0F); 
		LCD_WR_DATAX(0x2A); 
		LCD_WR_DATAX(0x28); 
		LCD_WR_DATAX(0x08); 
		LCD_WR_DATAX(0x0E); 
		LCD_WR_DATAX(0x08); 
		LCD_WR_DATAX(0x54); 
		LCD_WR_DATAX(0XA9); 
		LCD_WR_DATAX(0x43); 
		LCD_WR_DATAX(0x0A); 
		LCD_WR_DATAX(0x0F); 
		LCD_WR_DATAX(0x00); 
		LCD_WR_DATAX(0x00); 
		LCD_WR_DATAX(0x00); 
		LCD_WR_DATAX(0x00); 		 
		LCD_WR_REG(0XE1);    //Set Gamma 
		LCD_WR_DATAX(0x00); 
		LCD_WR_DATAX(0x15); 
		LCD_WR_DATAX(0x17); 
		LCD_WR_DATAX(0x07); 
		LCD_WR_DATAX(0x11); 
		LCD_WR_DATAX(0x06); 
		LCD_WR_DATAX(0x2B); 
		LCD_WR_DATAX(0x56); 
		LCD_WR_DATAX(0x3C); 
		LCD_WR_DATAX(0x05); 
		LCD_WR_DATAX(0x10); 
		LCD_WR_DATAX(0x0F); 
		LCD_WR_DATAX(0x3F); 
		LCD_WR_DATAX(0x3F); 
		LCD_WR_DATAX(0x0F); 
		LCD_WR_REG(0x2B); 
		LCD_WR_DATAX(0x00);
		LCD_WR_DATAX(0x00);
		LCD_WR_DATAX(0x01);
		LCD_WR_DATAX(0x3f);
		LCD_WR_REG(0x2A); 
		LCD_WR_DATAX(0x00);
		LCD_WR_DATAX(0x00);
		LCD_WR_DATAX(0x00);
		LCD_WR_DATAX(0xef);	 
		LCD_WR_REG(0x11); //Exit Sleep
		delay(6000000);
		LCD_WR_REG(0x29); //display on	
	}
	LCD_Display_Dir(0);		 	// horizon or vertical
	LCD -> LCD_BL_CTR = 1;					// Back Light
	LCD_Clear(WHITE);
}  		  

//在指定区域内填充指定颜色
//区域大小:(xend-xsta+1)*(yend-ysta+1)
//xsta
//color:要填充的颜色
void LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color)
{          
	uint16_t i,j;
	uint16_t xlen=0;
	xlen=ex-sx+1;	 
	for(i=sy;i<=ey;i++)
	{
	 	LCD_SetCursor(sx,i);      				
		LCD_WriteRAM_Prepare();     				  
		for(j=0;j<xlen;j++) LCD_WR_DATA(color);		    
	}
}

//在指定区域内填充指定颜色块			 
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color)
{  
	uint16_t height,width;
	uint16_t i,j;
	width=ex-sx+1; 			
	height=ey-sy+1;			
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(sx,sy+i);   	 
		LCD_WriteRAM_Prepare();     
		for(j=0;j<width;j++)LCD_WR_DATA(color[i*width+j]);
	}	  
}

//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; 
	else if(delta_x==0)incx=0;
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )
	{  
		LCD_DrawPoint(uRow,uCol);
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}

//画线，指定颜色
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine_color(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; 
	else if(delta_x==0)incx=0;
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )
	{  
		LCD_Fast_DrawPoint(uRow,uCol,color);
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}

//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode)
{  							  
	uint8_t temp,t1,t;
	uint16_t y0=y;
	uint8_t csize=(size/8+((size%8)?1:0))*(size/2);			
 	num=num-' ';
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	
		else if(size==16)temp=asc2_1608[num][t];	
		else if(size==24)temp=asc2_2412[num][t];	
		else return;								
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,POINT_COLOR);
			else if(mode==0)LCD_Fast_DrawPoint(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(y>=lcddev.height)return;		
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=lcddev.width)return;	
				break;
			}
		}
	}
}

//在指定位置显示一个字符，指定颜色
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar_color(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode,uint16_t pointColor,uint16_t backColor)
{  							  
	uint8_t temp,t1,t;
	uint16_t y0=y;
	uint8_t csize=(size/8+((size%8)?1:0))*(size/2);			
 	num=num-' ';
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	
		else if(size==16)temp=asc2_1608[num][t];	
		else if(size==24)temp=asc2_2412[num][t];	
		else return;								
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,pointColor);
			else if(mode==0)LCD_Fast_DrawPoint(x,y,backColor);
			temp<<=1;
			y++;
			if(y>=lcddev.height)return;		
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=lcddev.width)return;	
				break;
			}
		}
	}
}

//m^n函数
//返回值:m^n次方.
uint32_t LCD_Pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}			 

//显示数字,高位为0,则不显示
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//color:颜色 
//num:数值(0~4294967295);	 
void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 

//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);	 
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
void LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode)
{  
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
}

//显示数字,高位为0,还是显示，指定颜色
//x,y:起点坐标
//num:数值(0~999999999);	 
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
void LCD_ShowxNum_color(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode,uint16_t pointColor,uint16_t backColor)
{  
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD_ShowChar_color(x+(size/2)*t,y,'0',size,mode&0X01,pointColor,backColor);  
				else LCD_ShowChar_color(x+(size/2)*t,y,' ',size,mode&0X01,pointColor,backColor);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar_color(x+(size/2)*t,y,temp+'0',size,mode&0X01,pointColor,backColor); 
	}
}

//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址		
void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,char *p)
{         
	uint8_t x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}

//显示字符串，指定颜色
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址		
void LCD_ShowString_color(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,char *p,uint16_t pointColor,uint16_t backColor)
{         
	uint8_t x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;
        LCD_ShowChar_color(x,y,*p,size,0,pointColor,backColor);
        x+=size/2;
        p++;
    }  
}


/************************Key***************************************************/
// 按下为低电平
// 注意此函数有响应优先级,KEY1>KEY0>KEY2
uint8_t getKey(void){
  if (((Key->key>>1) & 0x01) == 0) return 1;
  else if (((Key->key>>0) & 0x01) == 0) return 0;
  else if (((Key->key>>2) & 0x01) == 0) return 2;
	else return 0xff;
}

// mode:0,不支持连续按;1,支持连续按
uint8_t keyScan(uint8_t mode){
	static uint8_t key_up=1;  //按键按松开标志
	uint8_t KEY;
	
	KEY = getKey();
	if(mode) key_up=1;  //支持连按		  
	if(key_up && KEY!=0xff)
	{
		delay_ms(10);  //去抖动 
		key_up=0;
		if(KEY==1) return KEY1_PRES;
		else if(KEY==0) return KEY0_PRES;
		else if(KEY==2) return KEY2_PRES;
	}
	else if(KEY==0xff) key_up=1; 	    
 	return NO_KEY_PRES;  // 无按键按下
}
