#include "RTX51TNY.h"
#include <reg52.h>
#include "LCD1602.h"
#include "DS18B20.h"
#include "OneWire.h"
#include "Buzzer.h"

/*******************************************************************************
* 硬 件 定 义
*******************************************************************************/
sbit FAN = P1^0;        // 风扇控制引脚（PWM输出）
sbit LED_MODE = P2^0;   // 模式指示灯（自动模式亮）
sbit ALARM_LED = P2^1;  // 报警指示灯（手动模式亮）
sbit BUZZER = P2^5;     // 蜂鸣器控制引脚
sbit KEY_MODE = P3^1;   // 模式切换按键（P3.1）
sbit KEY_FAN  = P3^0;   // 风扇控制按键（P3.0）

/*******************************************************************************
* 全 局 变 量
*******************************************************************************/
volatile bit manual_fan_on = 0;    // 手动模式风扇状态
volatile float current_temp = 25.0;// 当前温度值（浮点型）
volatile bit system_mode = 0;      // 系统模式 0-自动/1-手动
volatile bit alarm_flag = 0;       // 报警标志位
volatile bit emergency_alarm = 0;  // 紧急报警标志位

/*******************************************************************************
* 外 用 函 数: UART_SendString（用于调试和状态显示）
*******************************************************************************/
void UART_SendString(char *str) {
    EA = 0;  // 关闭中断
    while (*str) {
        SBUF = *str++;
        while (!TI);
        TI = 0;
    }
    EA = 1;  // 恢复中断
}

/*******************************************************************************
* 函 数 名       : Init_System
* 函数功能     : 系统初始化任务（任务0）
* 输    入       : 无
* 输    出       : 无
*******************************************************************************/
void Init_System(void) _task_ 0 {
    LCD_Init();         // 液晶显示器初始化
    LED_MODE = 0;       // 初始自动模式指示灯亮
    ALARM_LED = 0;      // 初始报警指示灯灭
    
    // 串口配置（用于调试）
    TMOD = 0x21;        // 定时器1模式2（自动重装）
    SCON = 0x50;        // 串口模式1
    TH1 = 0xFA;         // 波特率9600（12MHz）
    TR1 = 1;            // 启动定时器1

    // 创建系统任务
    os_create_task(1);  // 温度监测任务
    os_create_task(2);  // 风扇控制任务
    os_create_task(3);  // 按键检测任务
    os_create_task(4);  // 模式切换任务
    os_create_task(5);  // LCD显示任务
    os_create_task(6);  // 报警处理任务
    
    os_delete_task(0);  // 删除初始化任务
}

/*******************************************************************************
* 函 数 名       : Task_Temperature
* 函数功能     : 温度监测任务（任务1）
* 输    入       : 无
* 输    出       : 无
*******************************************************************************/
void Task_Temperature(void) _task_ 1 {
    while(1) {
        DS18B20_ConvertT();      // 启动温度转换
        os_wait2(K_TMO, 69);     // 等待750ms转换时间
        current_temp = DS18B20_ReadT();  // 读取温度值

        /* 温度报警逻辑处理 */
        if(current_temp >= 30.0) {  // 紧急报警触发
            if(!emergency_alarm) {
                emergency_alarm = 1;
                alarm_flag = 1;
                os_send_signal(6);
                UART_SendString("Emergency Mode!\r\n");
            }
        } 
        else if(current_temp <= 28.0 && emergency_alarm) { // 解除紧急报警
            emergency_alarm = 0;
            alarm_flag = 0;
            os_send_signal(6);
            UART_SendString("Emergency Cleared\r\n");
        }
        else if(system_mode == 0) {  // 自动模式普通报警
            if(current_temp > 26.0 && !alarm_flag) {
                alarm_flag = 1;
                os_send_signal(6);
                UART_SendString("Temperature Alarm!\r\n");
            }
            else if(current_temp <= 26.0 && alarm_flag) {
                alarm_flag = 0;
                os_send_signal(6);
                UART_SendString("Alarm Cleared\r\n");
            }
        }

        os_send_signal(2);      // 通知风扇控制任务
        os_wait2(K_TMO, 108);   // 1秒采集周期（108*10ms）
    }
}

/*******************************************************************************
* 函 数 名       : Task_FanControl
* 函数功能     : 风扇控制任务（任务2）
* 输    入       : 无
* 输    出       : 无
*******************************************************************************/
void Task_FanControl(void) _task_ 2 {
    unsigned char pwm_phase = 0;       // PWM相位计数器
    const unsigned char pwm_table[] = {1,0,1,0,1,0,1,0}; // 62.5%占空比

    while(1) {
        if(emergency_alarm) {  // 紧急模式最高优先级
            FAN = 1;           // 全速运行
            os_wait2(K_IVL, 10);
        }
        else if(system_mode == 0) {  // 自动模式
            if(current_temp >= 30.0) {
                FAN = 1;  // 全速
            }
            else if(current_temp >= 26.0) {  // 半速PWM
                FAN = pwm_table[pwm_phase];
                if(++pwm_phase >= 8) pwm_phase = 0;
                os_wait2(K_IVL, 2);  // PWM周期控制
            }
            else {  // 关闭风扇
                FAN = 0;
                os_wait(K_SIG, 0, 0);  // 等待温度更新信号
            }
        }
        else {  // 手动模式
            FAN = (manual_fan_on || current_temp >= 30.0) ? 1 : 0;
            os_wait(K_SIG, 0, 0);  // 等待按键信号
        }
    }
}

/*******************************************************************************
* 函 数 名       : Task_KeyScan
* 函数功能     : 按键检测任务（任务3）
* 输    入       : 无
* 输    出       : 无
*******************************************************************************/
void Task_KeyScan(void) _task_ 3 {
    bit key_state = 1;  // 按键状态寄存器

    while(1) {
        /* 模式切换按键检测 */
        if(KEY_MODE == 0 && key_state) {  // 检测下降沿
            os_wait2(K_TMO, 5);  // 50ms消抖
            if(KEY_MODE == 0) {
                os_send_signal(4);  // 发送模式切换信号
                UART_SendString("Mode Changed\r\n");
            }
            key_state = 0;
        }
        else if(KEY_MODE == 1) {
            key_state = 1;
        }

        /* 风扇控制按键检测（仅在手动模式有效） */
        if(system_mode && KEY_FAN == 0) {
            os_wait2(K_TMO, 5);  // 50ms消抖
            if(KEY_FAN == 0) {
                manual_fan_on = !manual_fan_on;
                os_send_signal(2);  // 更新风扇状态
                UART_SendString(manual_fan_on ? "Fan ON\r\n" : "Fan OFF\r\n");
            }
            while(KEY_FAN == 0);  // 等待按键释放
        }
        
        os_switch_task();  // 主动切换任务
    }
}

/*******************************************************************************
* 函 数 名       : Task_ModeSwitch
* 函数功能     : 模式切换任务（任务4）
* 输    入       : 无
* 输    出       : 无
*******************************************************************************/
void Task_ModeSwitch(void) _task_ 4 {
    while(1) {
        os_wait(K_SIG, 0, 0);  // 等待按键信号
        
        system_mode ^= 1;          // 切换系统模式
        LED_MODE = !system_mode;   // 更新模式指示灯
        ALARM_LED = system_mode;   // 更新报警指示灯
        manual_fan_on = 0;         // 切换模式时重置风扇状态
        
        //-- 新增代码：切换到手动模式时强制清除所有报警状态 --//
        if (system_mode == 1) {    // 手动模式
            alarm_flag = 0;        // 清除普通报警标志
            emergency_alarm = 0;   // 清除紧急报警标志
            os_send_signal(6);     // 强制通知报警处理任务更新状态
            BUZZER = 0;            // 立即关闭蜂鸣器
        }
        //--------------------------------------------//
        
        UART_SendString(system_mode ? "Manual Mode\r\n" : "Auto Mode\r\n");
    }
}


/*******************************************************************************
* 函 数 名       : Task_LCDDisplay
* 函数功能     : LCD显示任务（任务5）
* 输    入       : 无
* 输    出       : 无
*******************************************************************************/
void Task_LCDDisplay(void) _task_ 5 {
    int temp_scaled;     // 温度缩放值（放大100倍）
    int temp_int;        // 温度整数部分
    unsigned char temp_frac; // 温度小数部分
    bit is_negative = 0; // 负温度标志

    while(1) {
        /* 温度值处理 */
        temp_scaled = (int)(current_temp * 100 + (current_temp < 0 ? -0.5 : 0.5));
        is_negative = (temp_scaled < 0);
        if(is_negative) temp_scaled = -temp_scaled;
        
        temp_int = temp_scaled / 100;    // 获取整数部分
        temp_frac = temp_scaled % 100;   // 获取小数部分

        /* 第一行：温度显示 */
        LCD_ShowString(1, 1, "T:");               // 固定标签
        LCD_ShowChar(1, 3, is_negative ? '-' : ' '); // 符号位
        LCD_ShowNum(1, 4, temp_int, 3);          // 显示3位整数
        LCD_ShowChar(1, 7, '.');                 // 小数点
        LCD_ShowNum(1, 8, temp_frac / 10, 1);    // 十分位
        LCD_ShowNum(1, 9, temp_frac % 10, 1);    // 百分位
        LCD_ShowChar(1,10,'C');                  // 单位符号

        /* 第二行：状态显示 */
        LCD_ShowString(2, 1, system_mode ? "Mode:M " : "Mode:A ");  // 模式显示
        LCD_ShowString(2,10, emergency_alarm ? "[EMERG] " :        // 紧急状态
                               alarm_flag ? "[ALARM] " : "[NORMAL]"); // 普通状态

        os_wait2(K_TMO, 30);  // 300ms刷新周期
    }
}

/*******************************************************************************
* 函 数 名       : Task_AlarmHandler
* 函数功能     : 报警处理任务（任务6）
* 输    入       : 无
* 输    出       : 无
*******************************************************************************/
void Task_AlarmHandler(void) _task_ 6 {
    static unsigned char alarm_pattern = 0xAA;  // 报警模式寄存器

    while(1) {
        os_wait(K_SIG, 0, 0);  // 等待报警信号
        
        if(alarm_flag) {  
            while(alarm_flag) {  
                //-- 新增：检查当前模式，手动模式下直接退出报警 --//
                if (system_mode == 1) break;  // 手动模式不处理报警
                //---------------------------------------------//
                
                if(emergency_alarm) {  // 紧急报警处理
                    LED_MODE = !LED_MODE;
                    ALARM_LED = !ALARM_LED;
                    Buzzer_Time(100); 
                    os_wait2(K_TMO, 5); 
                }
                else {  // 普通报警处理
                    ALARM_LED = !ALARM_LED;
                    Buzzer_Time(200);  
                    os_wait2(K_TMO, 20); 
                }
            }
        }
        else {  
            // 恢复指示灯和蜂鸣器状态
            LED_MODE = !system_mode;  
            ALARM_LED = system_mode;  
            BUZZER = 0;               
        }
    }
}