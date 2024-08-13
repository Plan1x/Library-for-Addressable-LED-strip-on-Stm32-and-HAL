/*
 * ws2812b.c
 *
 *  Created on: Jul 26, 2024
 *      Author: irubt
 */

#include "ws2812b.h"

Strip WS2812b; // operational struct


/*
 * STATIC FUNCTIONS
 */
static void delay_micros(uint32_t us);
static void DWT_Init(void);
static void init(void);
static void set_bit(U16 *buffer, uint8_t Byte);
static void ws2812b_light_on(void); // starts DMA
static void ws2812b_light_off(void); // stops DMA



static void ws2812b_light_on(void)
{
	HAL_TIM_PWM_Start_DMA(WS2812b.htim1, TIM_CHANNEL_1, (U32*) WS2812b.buffer, LEDS_COUNT);
}
static void ws2812b_light_off(void)
{
	HAL_TIM_PWM_Stop_DMA(WS2812b.htim1, TIM_CHANNEL_1);

}
static void DWT_Init(void)
{
	  SCB_DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	  DWT_CONTROL |= DWT_CTRL_CYCCNTENA_Msk;
}

static void delay_micros(U32 us)
{
    U32 us_count_tic =  us * (SystemCoreClock / 1000000);
    DWT -> CYCCNT = 0U;
    while(DWT -> CYCCNT < us_count_tic);
}

static void init(void) // Filling whole strip with PWM Logic zero`s signal
{
	for (U16 i = 0; i < LEDS_COUNT; i++)
	{
		if(i < TIME_TO_RST)
		{
			WS2812b.buffer[i] = 0;
		}else
		{
			WS2812b.buffer[i] = LOGIC_ZERO;
		}

	}

}

static void set_bit(U16 *buffer, uint8_t Byte)
{
	for (U8 i = 0; i < BYTE; ++i)
	{
		if ((Byte & 0x01) == 0x01)
		{
			buffer[i] = LOGIC_ONE;

		} else
		{
			buffer[i] = LOGIC_ZERO;
		}
		Byte >>= 0x01;
	}
}
/*
 * STATIC FUNCTIONS
 */

void ws2812b_show(U16 Delay)
{



	ws2812b_light_on();
	//delay_micros((U32)(LEDS_COUNT * 200));
	//HAL_Delay(400);
	//HAL_Delay(10);
	HAL_Delay(Delay);
	ws2812b_light_off();
	//

}

void ws2812b_init(TIM_HandleTypeDef *htim, TIM_TypeDef * TIM ,void (*timer_callback)(TIM_HandleTypeDef * htim))
{
	WS2812b.htim1 = htim;
	WS2812b.TIM = TIM;
	init();
	DWT_Init();
	HAL_TIM_Base_Start_IT(htim);// Starting and allowing interrupts on Timer
	WS2812b.timer_update_interrupt_handler = timer_callback; // transfering ptr to local func
}



void ws2812b_setpixel(U8 Red, U8 Green,  U8 Blue, U8 Pixelnum) // GRB Row
{
	set_bit(((WS2812b.buffer + TIME_TO_RST) + (Pixelnum * BITS_IN_PIXEL)), Green);
	set_bit(((WS2812b.buffer + TIME_TO_RST) + ((Pixelnum * BITS_IN_PIXEL) + BYTE)), Red);
	set_bit(((WS2812b.buffer + TIME_TO_RST) + ((Pixelnum * BITS_IN_PIXEL) + (2 * BYTE))), Blue);
}

void ws2812b_setstrip(Color *_Col)
{
	for (U8 i = 0; i < PIXELS_COUNT; i++) ws2812b_setpixel(_Col -> Red, _Col -> Green, _Col -> Blue, i);
}

void ws2812b_moving_effect_three_colors(Color *_Col_1, Color * _Col_2, Color * _Col_3, U16 Delay)
{


	//U8 Red, U8 Green, U8 Blue, U16 Delay
	U16 counter, temp_cur, Pixels;
	U16 Blue_pos_start, Green_pos_start, Red_pos_start;
	U16 Blue_pos_end, Green_pos_end, Red_pos_end;

	counter = 0;
	Pixels = PIXELS_COUNT;



	while(Pixels % 3 != 0) Pixels--;




	temp_cur = Pixels / 3;



	Red_pos_start = 0;
	Green_pos_start = temp_cur;
	Blue_pos_start = 2 * temp_cur;

	Red_pos_end = temp_cur;
	Green_pos_end = 2 * temp_cur;
	Blue_pos_end = 3 * temp_cur;

	for (U8 j = 0; j < Pixels; ++j) // Each of 120 pixels
			{

		for (U8 i = 0; i < Pixels; ++i) // Each of 120 pixels

				{

			if (counter == 0) // first iteration(fill strip with default template)
					{

				if (i >= Red_pos_start && i < Red_pos_end)
				{

					ws2812b_setpixel(_Col_1 -> Red, _Col_1 -> Green, _Col_1 -> Blue, i); // R

				} else if (i >= Green_pos_start && i < Green_pos_end)
				{
					ws2812b_setpixel(_Col_2 -> Red, _Col_2 -> Green, _Col_2 -> Blue, i); // G
				} else if (i >= Blue_pos_start && i < Blue_pos_end)
				{

					ws2812b_setpixel(_Col_3 -> Red, _Col_3 -> Green, _Col_3 -> Blue, i); // B
				}

			} else
			{

				if (Blue_pos_end >= 0 && Blue_pos_end < temp_cur && Blue_pos_start > (2 * temp_cur) && Blue_pos_start <= (3 * temp_cur)) // Blue end begin transfer
						{
					if (i >= 0 && i < Red_pos_start) //B
							{
						ws2812b_setpixel(_Col_3 -> Red, _Col_3 -> Green, _Col_3 -> Blue, i);//ws2812b_setpixel(0, 0, Blue, i);
					} else if (i >= Red_pos_start && i < Red_pos_end) //R
							{
						ws2812b_setpixel(_Col_1 -> Red, _Col_1 -> Green, _Col_1 -> Blue, i);//ws2812b_setpixel(0, Red, 0, i);
					} else if (i >= Green_pos_start && i < Green_pos_end) //G
							{
						ws2812b_setpixel(_Col_2 -> Red, _Col_2 -> Green, _Col_2 -> Blue, i);//ws2812b_setpixel(Green, 0, 0, i);
					} else if (i >= Blue_pos_start && i < Pixels) //B
					{
						ws2812b_setpixel(_Col_3 -> Red, _Col_3 -> Green, _Col_3 -> Blue, i);//ws2812b_setpixel(0, 0, Blue, i);
					}

				} else if (Blue_pos_start >= 0 && Blue_pos_end > 0 && Blue_pos_end <= temp_cur && Green_pos_end <= (3 * temp_cur)) // Blue start begin transfer
							{

					if (i >= 0 && i < Blue_pos_end) //B
						    {

						ws2812b_setpixel(_Col_3 -> Red, _Col_3 -> Green, _Col_3 -> Blue, i);//ws2812b_setpixel(0, 0, Blue, i);

					} else if (i >= Red_pos_start && i < Red_pos_end) // R
							{
						ws2812b_setpixel(_Col_1 -> Red, _Col_1 -> Green, _Col_1 -> Blue, i);//ws2812b_setpixel(0, Red, 0, i);

					} else if (i >= Green_pos_start && i < Pixels) // G
					{
						ws2812b_setpixel(_Col_2 -> Red, _Col_2 -> Green, _Col_2 -> Blue, i);//ws2812b_setpixel(Green, 0, 0, i);
					}

				} else if (Green_pos_end >= 0 && Green_pos_end < temp_cur && Green_pos_start > (2 * temp_cur) && Green_pos_start <= (3 * temp_cur) && Red_pos_end < (3 * temp_cur)) // green end transfer begin
								{
					if (i >= 0 && i < Blue_pos_start) // G
							{
						ws2812b_setpixel(_Col_2 -> Red, _Col_2 -> Green, _Col_2 -> Blue, i);//ws2812b_setpixel(Green, 0, 0, i);
					} else if (i >= Blue_pos_start && i < Blue_pos_end) // B
							{
						ws2812b_setpixel(_Col_3 -> Red, _Col_3 -> Green, _Col_3 -> Blue, i);//ws2812b_setpixel(0, 0, Blue, i);
					} else if (i >= Red_pos_start && i < Red_pos_end) // R
							{
						ws2812b_setpixel(_Col_1 -> Red, _Col_1 -> Green, _Col_1 -> Blue, i);//ws2812b_setpixel(0, Red, 0, i);
					}

				} else if (Green_pos_start >= 0 && Green_pos_end <= temp_cur && Red_pos_end <= (3 * temp_cur)) // Transfering Green start to begin
							{
					if (i >= 0 && i < Green_pos_end) // G
							{
						ws2812b_setpixel(_Col_2 -> Red, _Col_2 -> Green, _Col_2 -> Blue, i);//ws2812b_setpixel(Green, 0, 0, i);
					} else if (i >= Blue_pos_start && i < Blue_pos_end) // B
							{
						ws2812b_setpixel(_Col_3 -> Red, _Col_3 -> Green, _Col_3 -> Blue, i);//ws2812b_setpixel(0, 0, Blue, i);
					} else if (i >= Red_pos_start && i < Red_pos_end) // R
							{
						ws2812b_setpixel(_Col_1 -> Red, _Col_1 -> Green, _Col_1 -> Blue, i);//ws2812b_setpixel(0, Red, 0, i);
					}

				} else if (Red_pos_end >= 0 && Red_pos_end <= temp_cur && Red_pos_start <= (3 * temp_cur)) // Transfering Red end and Red start to begin
							{
					if (i >= 0 && i < Red_pos_end) // R
							{
						ws2812b_setpixel(_Col_1 -> Red, _Col_1 -> Green, _Col_1 -> Blue, i);//ws2812b_setpixel(0, Red, 0, i);
					} else if (i >= Green_pos_start && i < Green_pos_end) // G
							{
						ws2812b_setpixel(_Col_2 -> Red, _Col_2 -> Green, _Col_2 -> Blue, i);//ws2812b_setpixel(Green, 0, 0, i);
					} else if (i >= Blue_pos_start && i < Blue_pos_end) //B
							{
						ws2812b_setpixel(_Col_3 -> Red, _Col_3 -> Green, _Col_3 -> Blue, i);//ws2812b_setpixel(0, 0, Blue, i);
					}
				}

			}

		}

		/////////////// If any of the cursors exceeded the number of pixels, then return to the beginning
		Red_pos_end = ((Red_pos_end + 1) > Pixels) ? 0 : (Red_pos_end + 1);
		Red_pos_start = ((Red_pos_start + 1) > Pixels) ? 0 : (Red_pos_start + 1);
		Green_pos_start = ((Green_pos_start + 1) > Pixels) ? 0 : (Green_pos_start + 1);
		Green_pos_end = ((Green_pos_end + 1) > Pixels) ? 0 : (Green_pos_end + 1);
		Blue_pos_start = ((Blue_pos_start + 1) > Pixels) ? 0 : (Blue_pos_start + 1);
		Blue_pos_end = ((Blue_pos_end + 1) > Pixels) ? 0 : (Blue_pos_end + 1);
		///////////////If any of the cursors exceeded the number of pixels, then return to the beginning

		ws2812b_show(Delay);
		counter = 1;

	}
}
void ws2812b_moving_and_vanishing_effect(U8 Color_1_brightness, U8 Color_2_brightness, U8 Color_1, U8 Color_2, U16 Delay)
{
	for (U16 i = 0; i < PIXELS_COUNT; ++i)
	{
		if (i % 2 == 0)
		{
			switch (Color_1)
			{

			case GREEN:
				ws2812b_setpixel(Color_1_brightness, 0, 0, i);
				break;

			case RED:
				ws2812b_setpixel(0, Color_1_brightness, 0, i);
				break;

			case BLUE:
				ws2812b_setpixel(0, 0, Color_1_brightness, i);
				break;
			}

		} else
		{

			switch (Color_2)
			{

			case GREEN:
				ws2812b_setpixel(Color_2_brightness, 0, 0, i);
				break;

			case RED:
				ws2812b_setpixel(0, Color_2_brightness, 0, i);
				break;

			case BLUE:
				ws2812b_setpixel(0, 0, Color_2_brightness, i);
				break;
			}
		}
		ws2812b_show(Delay);

	}
	for (U16 i = (PIXELS_COUNT - 1); i > 0; --i)
	{
		ws2812b_setpixel(0, 0, 0, i);
		ws2812b_show(Delay);
	}
}
void ws2812b_sliding_effect(U8 Red, U8 Green, U8 Blue, U16 Delay)
{

	for (U8 i = 0; i < 3; i++)
	{

		for (U8 k = 0; k < PIXELS_COUNT; k++)
		{

			if (i == 0)
			{
				ws2812b_setpixel(0, 0, Blue, k);
			} else if (i == 1)
			{
				ws2812b_setpixel(0, Red, 0, k);
			} else if (i == 2)
			{
				ws2812b_setpixel(Green, 0, 0, k);
			}

			ws2812b_show(Delay);

		}
	}
}

void ws2812b_moving_effect_two_colors(Color *_Col_1, Color * _Col_2,  U16 Delay)
{
	for (U8 i = 0; i < 2; ++i)
	{
		for (U16 j = 0; j < PIXELS_COUNT; ++j)
		{

			if (i == 0)
			{
				if (j % 2 == 0) ws2812b_setpixel(_Col_1 -> Red, _Col_1 -> Green, _Col_1 -> Blue, j); else ws2812b_setpixel(_Col_2 -> Red, _Col_2 -> Green, _Col_2 -> Blue, j);

			} else if (i == 1)
			{

				if (j % 2 == 0) ws2812b_setpixel(_Col_2 -> Red, _Col_2 -> Green, _Col_2 -> Blue, j); else ws2812b_setpixel(_Col_1 -> Red, _Col_1 -> Green, _Col_1 -> Blue, j);

			}

		}
		ws2812b_show(Delay);
	}
}


WS2812 new_Strip(WS2812 * _Strip) // Constructor
{
		_Strip -> init = ws2812b_init;
		_Strip -> moving_effect_three_colors = ws2812b_moving_effect_three_colors;
		_Strip -> moving_effect_two_colors = ws2812b_moving_effect_two_colors;
		_Strip -> set_pixel = ws2812b_setpixel;
		_Strip -> setstrip = ws2812b_setstrip;
		_Strip -> show = ws2812b_show;



	return *(_Strip);
}


void handler(TIM_HandleTypeDef * htim)
{
	if(htim -> Instance == WS2812b.TIM)
	{
		static U16 counter;



	}
}
