/*
 * ws2812b.c
 *
 *  Created on: Jul 26, 2024
 *      Author: irubt
 */

#include "ws2812b.h"

Strip WS2812b;




static void set_bit(U16 *buffer, uint8_t Byte);
static void ws2812b_light_on(void); // starts DMA
static void ws2812b_light_off(void); // stops DMA

void ws2812b_show(U8 Delay) {

	HAL_TIM_PWM_Start_DMA(&WS2812b.htim1, TIM_CHANNEL_1, (U32*) WS2812b.buffer,
	LEDS_COUNT);
	HAL_Delay(Delay);
	HAL_TIM_PWM_Stop_DMA(&WS2812b.htim1, TIM_CHANNEL_1);
	HAL_Delay(1);
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

void ws2812b_moving_effect_three_colors(U8 Red, U8 Green, U8 Blue,U8 Delay)
{


	U8 counter;
	U8 Blue_pos_start, Green_pos_start, Red_pos_start;
	U8 Blue_pos_end, Green_pos_end, Red_pos_end;
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

			if (counter == 0) // first iteration(fill strip with default template)
			{

				if (i >= Red_pos_start && i < Red_pos_end)
				{

					ws2812b_setpixel(0, Red, 0, i);// R

				} else if (i >= Green_pos_start && i < Green_pos_end)
				{
					ws2812b_setpixel(Green,0,0, i); // G
				} else if (i >= Blue_pos_start && i < Blue_pos_end)
				{

					ws2812b_setpixel(0, 0, Blue, i); // B
				}

			} else
			{


				if(Blue_pos_end >= 0 && Blue_pos_end < 40 && Blue_pos_start > 80 && Blue_pos_start <= 120) // Blue end begin transfer
				{
					if(i >= 0 && i < Red_pos_start)//B
					{
						ws2812b_setpixel(0, 0, Blue, i);
					}else if(i >= Red_pos_start && i < Red_pos_end)//R
					{
						ws2812b_setpixel(0, Red, 0, i);
					}else if(i >= Green_pos_start && i < Green_pos_end)//G
					{
						ws2812b_setpixel(Green, 0, 0, i);
					}else if(i >= Blue_pos_start && i < PIXELS_COUNT)//B
					{
						ws2812b_setpixel(0, 0, Blue, i);
					}



				}else if(Blue_pos_start >= 0 && Blue_pos_end > 0 && Blue_pos_end <= 40 && Green_pos_end <= 120) // Blue start begin transfer
				{


					if(i >= 0 && i < Blue_pos_end) //B
					{

						ws2812b_setpixel(0, 0, Blue, i);

					}else if(i >= Red_pos_start && i < Red_pos_end) // R
					{
						ws2812b_setpixel(0, Red, 0, i);

					}else if(i >= Green_pos_start && i < PIXELS_COUNT)// G
					{
						ws2812b_setpixel(Green, 0, 0, i);
					}



				}
				else if(Green_pos_end >= 0 && Green_pos_end < 40 && Green_pos_start > 80 && Green_pos_start <= 120 && Red_pos_end < 120) // green end transfer begin
				{
					if(i >= 0 && i < Blue_pos_start) // G
					{
						ws2812b_setpixel(Green, 0, 0, i);
					}else if(i >= Blue_pos_start && i < Blue_pos_end) // B
					{
						ws2812b_setpixel(0, 0, Blue, i);
					}else if(i >= Red_pos_start && i < Red_pos_end) // R
					{
						ws2812b_setpixel(0, Red, 0, i);
					}

				}else if(Green_pos_start >= 0 && Green_pos_end <= 40 && Red_pos_end <= 120) // Transfering Green start to begin
				{
					if(i >= 0 && i < Green_pos_end)// G
					{
						ws2812b_setpixel(Green, 0, 0, i);
					}else if(i >= Blue_pos_start && i < Blue_pos_end) // B
					{
						ws2812b_setpixel(0, 0, Blue, i);
					}else if(i >= Red_pos_start && i < Red_pos_end) // R
					{
						ws2812b_setpixel(0, Red, 0, i);
					}

				}
				else if(Red_pos_end >= 0 && Red_pos_end <= 40 && Red_pos_start <= 120)// Transfering Red end and Red start to begin
				{
					if(i >= 0 && i < Red_pos_end) // R
					{
						ws2812b_setpixel(0, Red, 0, i);
					}else if(i >= Green_pos_start && i < Green_pos_end)// G
					{
						ws2812b_setpixel(Green,0,0, i);
					}else if(i >= Blue_pos_start && i < Blue_pos_end)//B
					{
						ws2812b_setpixel(0, 0, Blue, i);
					}
				}


			}



		}


		/////////////// If any of the cursors exceeded the number of pixels, then return to the beginning
		Red_pos_end = ((Red_pos_end + 1) > 120) ? 0 : (Red_pos_end + 1);
		Red_pos_start = ((Red_pos_start + 1) > 120) ? 0 : (Red_pos_start + 1);
		Green_pos_start = ((Green_pos_start + 1) > 120) ? 0 : (Green_pos_start + 1);
		Green_pos_end = ((Green_pos_end + 1) > 120) ? 0 : (Green_pos_end + 1);
		Blue_pos_start = ((Blue_pos_start + 1) > 120) ? 0 : (Blue_pos_start + 1);
		Blue_pos_end = ((Blue_pos_end + 1) > 120) ? 0 : (Blue_pos_end + 1);
		///////////////If any of the cursors exceeded the number of pixels, then return to the beginning


		ws2812b_show(Delay);
		counter = 1;



	}
}

void ws2812b_sliding_effect(U8 Red, U8 Green, U8 Blue, U8 Delay) {

	for (U8 i = 0; i < 3; i++) {

		for (U8 k = 0; k < PIXELS_COUNT; k++) {

			if (i == 0) {
				ws2812b_setpixel(0, 0, Blue, k);
			} else if (i == 1) {
				ws2812b_setpixel(0, Red, 0, k);
			} else if (i == 2) {
				ws2812b_setpixel(Green, 0, 0, k);
			}

			ws2812b_show(Delay);

		}
	}
}
