#include <REGX52.H>
/**
  * @brief  串口初始化，4800bps@11.0592MHz
  * @param  无
  * @retval 无
  */
void UART_Init()
{
	 SCON = 0x50;    // 8位数据，可变波特率
   PCON |= 0x80;   // SMOD=1，倍增波特率
   TMOD &= 0x0F;   // 清除定时器1模式
   TMOD |= 0x20;   // 定时器1，模式2（8位自动重装）
   TL1 = 0xF4;     // 4800bps@11.0592MHz
   TH1 = 0xF4;     // 重装值
   ET1 = 0;        // 禁用定时器1中断
   TR1 = 1;        // 启动定时器1
	 //启动串口中断
	 EA=1;
	 ES=1;
}



/**
  * @brief  串口发送一个字节数据
  * @param  Byte 要发送的一个字节数据
  * @retval 无
  */
void UART_SendByte(unsigned char Byte)
{
	SBUF=Byte;
	while(TI==0);
	TI=0;
}



/*串口中断函数模板
void UART_Routine() interrupt 4
{
	if(RI==1)
	{
		
		RI=0;
	}
}
*/
