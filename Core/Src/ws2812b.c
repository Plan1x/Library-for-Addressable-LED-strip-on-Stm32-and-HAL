/*
 * ws2812b.c
 *
 *  Created on: Jul 26, 2024
 *      Author: irubt
 */

#include "ws2812b.h"

Strip WS2812b;

static void set_bit(U16 *buffer, uint8_t Byte);

void ws2812b_init(void) // Filling whole strip with PWM Logic zero`s signal
{
	for (U16 i = 0; i < LEDS_COUNT; i++) WS2812b.buffer[i] = LOGIC_ZERO;
}

static void set_bit(U16 *buffer, uint8_t Byte)
{
	for (U8 i = 0; i < BYTE; i++)
	{
		if ((Byte & 1) == 1)
		{
			buffer[i] = LOGIC_ONE;

		} else
		{
			buffer[i] = LOGIC_ZERO;
		}
		Byte >>= 1;
	}
}
void ws2812b_setpixel(uint8_t Green, uint8_t Red,   uint8_t Blue, uint8_t Pixelnum) // GRB Row
{
	set_bit((WS2812b.buffer + (Pixelnum * BITS_IN_PIXEL)), Green);
	set_bit((WS2812b.buffer + ((Pixelnum * BITS_IN_PIXEL) + BYTE)), Red);
	set_bit((WS2812b.buffer + ((Pixelnum * BITS_IN_PIXEL) + (2 * BYTE))), Blue);
}

void ws2812b_setstrip(uint8_t Green, uint8_t Red,  uint8_t Blue)
{
	for (U8 i = 0; i < PIXELS_COUNT; i++)
	{
		ws2812b_setpixel(Green, Red,  Blue, i);

	}
}
//TODO Fading effect
void ws2812b_fading(uint8_t Color)
{
	for (U8 i = 0; i < 255; i += 15)
	{

		ws2812b_setstrip(0, 0, i);
		HAL_TIM_PWM_Start_DMA(&WS2812b.htim1, TIM_CHANNEL_1, (U32 *)WS2812b.buffer, LEDS_COUNT);
		HAL_Delay(500);
		HAL_TIM_PWM_Stop_DMA(&WS2812b.htim1, TIM_CHANNEL_1);
	}

	for (U8 j = 255; j > 0; j-=15)
	{

		ws2812b_setstrip(0, 0, j);
		HAL_TIM_PWM_Start_DMA(&WS2812b.htim1, TIM_CHANNEL_1, (U32 *)WS2812b.buffer, LEDS_COUNT);
		HAL_Delay(500);
		HAL_TIM_PWM_Stop_DMA(&WS2812b.htim1, TIM_CHANNEL_1);
	}

}
void ws2812b_light_on(void)
{
	HAL_TIM_PWM_Start_DMA(&WS2812b.htim1, TIM_CHANNEL_1, (U32*)WS2812b.buffer, LEDS_COUNT);
}
void ws2812b_light_off(void)
{
	HAL_TIM_PWM_Stop_DMA(&WS2812b.htim1, TIM_CHANNEL_1);

}
