#include <REGX52.H>
#include "Timer0.h"


unsigned char LEDNum;

void main()
{
	P2=0xFE;
	Timer0_Init();
	while(1);
}

void Timer0_Routine() interrupt 1
{
	static unsigned int T0Count;
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	T0Count++;		//T0Count计次，对中断频率进行分频
	if(T0Count>=1000)//1s
	{
		T0Count = 0;
    LEDNum++;		//LEDNum自增
		if(LEDNum>=8)	//限制LEDNum自增范围
		LEDNum=0;
		P2=~(0x01<<LEDNum);	//LED的第LEDNum位点亮   
	}
}
