/*
  August 2013

  Focused Flight32 Rev -

  Copyright (c) 2013 John Ihlein.  All rights reserved.

  Open Source STM32 Based Multicopter Controller Software

  Designed to run on the Naze32Pro Flight Control Board

  Includes code and/or ideas from:

  1)AeroQuad
  2)BaseFlight
  3)CH Robotics
  4)MultiWii
  5)Paparazzi UAV
  5)S.O.H. Madgwick
  6)UAVX

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

///////////////////////////////////////////////////////////////////////////////

#include "board.h"

///////////////////////////////////////////////////////////////////////////////
// PWM ESC Defines and Variables
///////////////////////////////////////////////////////////////////////////////

#define ESC_PULSE_1MS    2000  // 1ms pulse width
#define ESC_PULSE_PERIOD 5000  // pulse period (400Hz)

static volatile uint32_t *OutputChannels[] = { &(TIM15->CCR1),
	                                           &(TIM15->CCR2),
	                                           &(TIM3->CCR1),
	                                           &(TIM3->CCR2),
	                                           &(TIM4->CCR1),
	                                           &(TIM4->CCR2),
                                               &(TIM4->CCR3),
                                               &(TIM4->CCR4),};

///////////////////////////////////////////////////////////////////////////////
// PWM ESC Initialization
///////////////////////////////////////////////////////////////////////////////

void pwmEscInit(uint16_t escPwmRate)
{
    GPIO_InitTypeDef         GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef        TIM_OCInitStructure;

    // Outputs
    // ESC PWM1  TIM15_CH1  PA2
    // ESC PWM2  TIM15_CH2  PA3
    // ESC PWM3  TIM3_CH1   PA6
    // ESC PWM4  TIM3_CH2   PA7
    // ESC PWM5  TIM4_CH1   PB6
    // ESC PWM6  TIM4_CH2   PB7
    // ESC PWM7  TIM4_CH3   PB8
    // ESC PWM8  TIM4_CH4   PB9

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

 	GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;

 	GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_9);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_9);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_2);

	///////////////////////////////////

    // Output timers

    TIM_TimeBaseStructure.TIM_Period            = (uint16_t)(2000000 / escPwmRate) - 1;
    TIM_TimeBaseStructure.TIM_Prescaler         = 36 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0000;

    TIM_TimeBaseInit(TIM3,  &TIM_TimeBaseStructure);
    TIM_TimeBaseInit(TIM4,  &TIM_TimeBaseStructure);
    TIM_TimeBaseInit(TIM15, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_Pulse        = ESC_PULSE_1MS;
    TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity  = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;

    TIM_OC1Init(TIM15, &TIM_OCInitStructure);
    TIM_OC2Init(TIM15, &TIM_OCInitStructure);

    TIM_OC1Init(TIM3,  &TIM_OCInitStructure);
    TIM_OC2Init(TIM3,  &TIM_OCInitStructure);

    TIM_OC1Init(TIM4,  &TIM_OCInitStructure);
    TIM_OC2Init(TIM4,  &TIM_OCInitStructure);
    TIM_OC3Init(TIM4,  &TIM_OCInitStructure);
    TIM_OC4Init(TIM4,  &TIM_OCInitStructure);

    TIM_Cmd(TIM3,  ENABLE);
    TIM_Cmd(TIM4,  ENABLE);
    TIM_Cmd(TIM15, ENABLE);

    TIM_CtrlPWMOutputs(TIM15, ENABLE);
}

///////////////////////////////////////////////////////////////////////////////
// PWM ESC Write
///////////////////////////////////////////////////////////////////////////////

void pwmEscWrite(uint8_t channel, uint16_t value)
{
    *OutputChannels[channel] = value;
}

///////////////////////////////////////////////////////////////////////////////
