#include <REGX52.H>
#include "Delay.h"
#include "UART.h"
#include "Nixie.h"

unsigned char buffer[8] = {0};

void main()
{
	UART_Init();		//串口初始化
	while(1)
	{
	 unsigned char n;	
   // 动态扫描数码管     
   for(n=0; n<8; n++)      
   {     
     Nixie(n+1, buffer[n]);
   }       
	}
}

void UART_Routine() interrupt 4
{
	if(RI==1)					//如果接收标志位为1，接收到了数据
	{
		unsigned char temp;
		unsigned char i;
		temp= SBUF;// 获取接收的字符
		 		
	 
    // 左移缓冲区，新数字插入最右侧
    for(i=0; i<7; i++)
    {
     buffer[i] = buffer[i+1];
    }
     buffer[7] = temp-'0';// 转换ASCII为数字 
    	   
    UART_SendByte(temp); // 回传接收的字符	

		RI=0;					//接收标志位清0
	}
}