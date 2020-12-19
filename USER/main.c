#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "beep.h"
#include "key.h"


void Paint(){
	u8 i, j;
	for (i = 0; i < 200; ++i)
		for (j = 0; j < 240; ++j)
			LCD_DrawPoint(i, j);
}


int main(void)
{ 
 
	u8 t;
	u8 len;	
	u16 times=0;  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);	
	uart_init(115200);	
	LED_Init();		  	
	LCD_Init();
	POINT_COLOR=RED;
	while(1)
	{ 
		if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			printf("\r\nYour Message\r\n");
			for(t=0;t<len;t++)
			{
				USART_SendData(USART1, USART_RX_BUF[t]);         //向串口1发送数据
				
				Paint();	
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
			}
			printf("size:%d", strlen(USART_RX_BUF));
			printf("\r\n\r\n");//插入换行
			USART_RX_STA=0;
		}else
		{
			times++;
			if(times%5000==0)
			{
				printf("Hello\r\n\r\n\r\n");
			}
			if(times%200==0)printf("Please input data ending with Enter\r\n");  
			if(times%30==0)LED0=!LED0;//闪烁LED,提示系统正在运行.
			delay_ms(10);   
		}
	}
}

