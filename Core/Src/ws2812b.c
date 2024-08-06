/*
 * ws2812b.c
 *
 *  Created on: Jul 26, 2024
 *      Author: irubt
 */

#include "ws2812b.h"

Strip WS2812b;

U8 Blue_pos_start, Green_pos_start, Red_pos_start;
U8 Blue_pos_end, Green_pos_end, Red_pos_end;


static void set_bit(U16 *buffer, uint8_t Byte);
static void set_pixels_for_moving_effect(void);
static void ws2812b_light_on(void); // starts DMA
static void ws2812b_light_off(void); // stops DMA

void ws2812b_show(void) {

	HAL_TIM_PWM_Start_DMA(&WS2812b.htim1, TIM_CHANNEL_1, (U32*) WS2812b.buffer,
	LEDS_COUNT);
	HAL_Delay(40);
	HAL_TIM_PWM_Stop_DMA(&WS2812b.htim1, TIM_CHANNEL_1);
	HAL_Delay(10);
}
static void ws2812b_light_on(void) {
	HAL_TIM_PWM_Start_DMA(&WS2812b.htim1, TIM_CHANNEL_1, (U32*) WS2812b.buffer,
	LEDS_COUNT);
}
static void ws2812b_light_off(void) {
	HAL_TIM_PWM_Stop_DMA(&WS2812b.htim1, TIM_CHANNEL_1);

}
void ws2812b_init(void) // Filling whole strip with PWM Logic zero`s signal
{
	for (U16 i = 0; i < LEDS_COUNT; i++)
		WS2812b.buffer[i] = LOGIC_ZERO;
}

static void set_bit(U16 *buffer, uint8_t Byte) {
	for (U8 i = 0; i < BYTE; i++) {
		if ((Byte & 1) == 1) {
			buffer[i] = LOGIC_ONE;

		} else {
			buffer[i] = LOGIC_ZERO;
		}
		Byte >>= 1;
	}
}

static void set_pixels_for_moving_effect(void) {
	for (U8 i = 0; i < 3; i++) {

		for (U8 j = 0; j < 40; j++) {
			switch (i) {
			case 0:
				ws2812b_setpixel(0, 127, 0, (40 * i) + j); //R
				break;

			case 1:
				ws2812b_setpixel(127, 0, 0, (40 * i) + j); //G
				break;
			case 2:
				ws2812b_setpixel(0, 0, 127, (40 * i) + j); //B
				break;
			}

		}

	}
}

void ws2812b_setpixel(uint8_t Green, uint8_t Red, uint8_t Blue,
		uint8_t Pixelnum) // GRB Row
{
	set_bit((WS2812b.buffer + (Pixelnum * BITS_IN_PIXEL)), Green);
	set_bit((WS2812b.buffer + ((Pixelnum * BITS_IN_PIXEL) + BYTE)), Red);
	set_bit((WS2812b.buffer + ((Pixelnum * BITS_IN_PIXEL) + (2 * BYTE))), Blue);
}

void ws2812b_setstrip(uint8_t Green, uint8_t Red, uint8_t Blue) {
	for (U8 i = 0; i < PIXELS_COUNT; i++) {
		ws2812b_setpixel(Green, Red, Blue, i);

	}
}
//TODO Fading effect
void ws2812b_fading(uint8_t Color) {
	for (U8 i = 0; i < 255; i += 15) {

		ws2812b_setstrip(0, 0, i);
		HAL_TIM_PWM_Start_DMA(&WS2812b.htim1, TIM_CHANNEL_1,
				(U32*) WS2812b.buffer, LEDS_COUNT);
		HAL_Delay(500);
		HAL_TIM_PWM_Stop_DMA(&WS2812b.htim1, TIM_CHANNEL_1);
	}

	for (U8 j = 255; j > 0; j -= 15) {

		ws2812b_setstrip(0, 0, j);
		HAL_TIM_PWM_Start_DMA(&WS2812b.htim1, TIM_CHANNEL_1,
				(U32*) WS2812b.buffer, LEDS_COUNT);
		HAL_Delay(500);
		HAL_TIM_PWM_Stop_DMA(&WS2812b.htim1, TIM_CHANNEL_1);
	}

}

void ws2812b_moving_effect(void) {


	U8 index, counter;

	counter = 0;

	Red_pos_start = 0;
	Green_pos_start = 40;
	Blue_pos_start = 80;

	Red_pos_end = 40;
	Green_pos_end = 80;
	Blue_pos_end = 120;



	for (U8 j = 0; j < PIXELS_COUNT; ++j) // Each of 120 pixels
			{



		for (U8 i = 0; i < PIXELS_COUNT; ++i) // Each of 120 pixels

				{

			if (counter == 0)
			{

				if (i >= Red_pos_start && i < Red_pos_end)
				{
					ws2812b_setpixel(0, 127, 0, i);

				} else if (i >= Green_pos_start && i < Green_pos_end)
				{
					ws2812b_setpixel(127, 0, 0, i);
				} else if (i >= Blue_pos_start && i < Blue_pos_end)
				{
					ws2812b_setpixel(0, 0, 127, i);

				}

			} else
			{


				if(Blue_pos_end >= 0 && Blue_pos_end < 40 && Blue_pos_start > 80 && Blue_pos_start < 120 ) // Blue end begin transfer
				{
					if(i >= Blue_pos_end && i < Red_pos_start)
					{
						ws2812b_setpixel(0, 0, 127, i);
					}else if(i >= Red_pos_start && i < Red_pos_end)
					{
						ws2812b_setpixel(0, 127, 0, i);
					}else if(i >= Green_pos_start && i < Green_pos_end)
					{
						ws2812b_setpixel(127, 0, 0, i);
					}else if(i >= Blue_pos_start && i < PIXELS_COUNT)
					{
						ws2812b_setpixel(0, 0, 127, i);
					}



				}else if( Blue_pos_end > 0 && Blue_pos_end < 40 && Green_pos_end < 120) // Blue start begin transfer and green end transfer begin
				{


					if(i >= 0 && i < Blue_pos_end)
					{
						//Blue_pos_start = 0;
						//Green_pos_end = 0;
						ws2812b_setpixel(0, 0, 127, i);

					}else if(i >= Red_pos_start && i < Red_pos_end)
					{
						ws2812b_setpixel(0, 127, 0, i);

					}else if(i >= Green_pos_start && i < 120)
					{
						ws2812b_setpixel(127, 0, 0, i);
					}



				}else if(Green_pos_end > 0 && Green_pos_end < 40 && Green_pos_start > 80 && Green_pos_start < 120) // green end transfer begin
				{
					if(i >= 0 && i < Blue_pos_start)
					{
//						Green_pos_end = 0;
						ws2812b_setpixel(127, 0, 0, i);
					}else if(i >= Blue_pos_start && i < Blue_pos_end)
					{
						ws2812b_setpixel(0, 0, 127, i);
					}else if(i >= Red_pos_start && i < Red_pos_end)
					{
						ws2812b_setpixel(0, 127, 0, i);
					}else if(i >= Green_pos_start && i < PIXELS_COUNT)
					{
						ws2812b_setpixel(127, 0, 0, i);
					}
				}


			}



		}

		//HAL_Delay(500);
		///////////////Если какой то из курсоров превысил количество пикселей, то возвращаем в начало
		Red_pos_end = ((Red_pos_end + 1) >= 120) ? 0 : (Red_pos_end + 1);
		Red_pos_start = ((Red_pos_start + 1) >= 120) ? 0 : (Red_pos_start + 1);
		Green_pos_start = ((Green_pos_start + 1) >= 120) ? 0 : (Green_pos_start + 1);
		Green_pos_end = ((Green_pos_end + 1) >= 120) ? 0 : (Green_pos_end + 1);
		Blue_pos_start = ((Blue_pos_start + 1) >= 120) ? 0 : (Blue_pos_start + 1);
		Blue_pos_end = ((Blue_pos_end + 1) >= 120) ? 0 : (Blue_pos_end + 1);
		///////////////Если какой то из курсоров превысил количество пикселей, то возвращаем в начало

		counter = 1;
		ws2812b_show();




	}
}

void ws2812b_sliding_effect(void) {

	for (U8 i = 0; i < 3; i++) {

		for (U8 k = 0; k < PIXELS_COUNT; k++) {

			if (i == 0) {
				ws2812b_setpixel(0, 0, 127, k);
			} else if (i == 1) {
				ws2812b_setpixel(0, 127, 0, k);
			} else if (i == 2) {
				ws2812b_setpixel(127, 0, 0, k);
			}

			ws2812b_show();

		}
	}
}
