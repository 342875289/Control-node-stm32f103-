#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/4
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//********************************************************************************
void TIM1_SPWM_Init(unsigned int Prescaler);
void 	TIM8_PWM_Init(void );
void TIM3_Int_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM3_SPWM_Init(u16 squareFrequence ,u16 psc);
void TIM_MasterSlaveModeInit(void);
void LED_GPIO_Config(void);
void TIM2_Configuration(void);
void TIM2_NVIC_Configuration(void);
#endif
