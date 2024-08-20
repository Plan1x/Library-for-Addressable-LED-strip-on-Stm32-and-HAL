/*
 * ws2812b.c
 *
 *  Created on: Jul 26, 2024
 *      Author: irubt
 */

#include "ws2812b.h"

static WS2812_hw WS2812b; // operational struct

/*
 * STATIC FUNCTIONS
 */

static RGB Hsv_to_rgb(HSV *Col);
static void DWT_Init(void);
static void init(void);
static void set_bit(U8 *buffer, U8 Byte); // Sets selected 8 bits in buffer
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

static void init(void) // Filling whole strip with PWM Logic zero`s signal
{
	for (U16 i = 0; i < LEDS_COUNT; ++i)
	{
		if (i < TIME_TO_RST) WS2812b.buffer[i] = 0; else WS2812b.buffer[i] = LOGIC_ZERO;
	}
}

static void set_bit(U8 *buffer, U8 Byte)
{
	for (U8 i = 0; i < BYTE; ++i)
	{
		if ((Byte & 0x01) == 0x01) buffer[i] = LOGIC_ONE; else buffer[i] = LOGIC_ZERO;
		Byte >>= 0x01;
	}
}

static RGB Hsv_to_rgb(HSV *Col)
{
	RGB temp;

	float S, V, C, X, M;
	float res, Red, Green, Blue;

	S = Col -> Saturation / 100.0;
	V = Col -> Value / 100.0;

	C = V * S;
	res = fmod((Col -> Hue / 60.0), 2.0);

	X = C * (1.0 - fabs(res - 1.0));

	M = V - C;

	if (Col -> Hue >= 0 && Col -> Hue < 60)
	{
		Red = C;
		Green = X;
		Blue = 0;
	} else if (Col -> Hue >= 60 && Col -> Hue < 120)
	{
		Red = X;
		Green = C;
		Blue = 0;
	} else if (Col -> Hue >= 120 && Col -> Hue < 180)
	{
		Red = 0;
		Green = C;
		Blue = X;
	} else if (Col -> Hue >= 180 && Col -> Hue < 240)
	{
		Red = 0;
		Green = X;
		Blue = C;
	} else if (Col -> Hue >= 240 && Col -> Hue < 300)
	{
		Red = X;
		Green = 0;
		Blue = C;
	} else if (Col -> Hue >= 300 && Col -> Hue < 360)
	{
		Red = C;
		Green = 0;
		Blue = X;
	}

	Red += M;
	Green += M;
	Blue += M;

	temp.Red = (U8) (Red * 255.0);
	temp.Green = (U8) (Green * 255.0);
	temp.Blue = (U8) (Blue * 255.0);

	return temp;

}

/*
 * STATIC FUNCTIONS
 */

/*
 * Operational functions
 */

void Ws2812b_Delay_In_Microseconds(U32 us)
{
	U32 us_count_tic = us * (SystemCoreClock / 1000000);
	DWT -> CYCCNT = 0U;
	while (DWT->CYCCNT < us_count_tic);
}

void Ws2812b_Show(U16 Delay)
{
	ws2812b_light_on();
	HAL_Delay(Delay);
}

void Ws2812b_Show_without_Delay(void)
{
	ws2812b_light_on();
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) // DMA END TRANSFER CALLBACK
{
	if (htim -> Instance == WS2812b.TIM) ws2812b_light_off();
}

void Ws2812b_Init(TIM_HandleTypeDef *htim, TIM_TypeDef *TIM)
{
	WS2812b.htim1 = htim;
	WS2812b.TIM = TIM;
	init();
	DWT_Init();
}

WS2812 New_Strip(WS2812 *_Strip) // Constructor
{
	_Strip -> Init = Ws2812b_Init;
	_Strip -> Set_pixel_RGB = Ws2812b_SetPixel_RGB;
	_Strip -> Set_pixel_HSV = Ws2812b_SetPixel_HSV;
	_Strip -> Setstrip_RGB = Ws2812b_SetStrip_RGB;
	_Strip -> SetStrip_HSV = Ws2812b_SetStrip_HSV;
	_Strip -> Moving_effect_two_colors = Ws2812b_Moving_Effect_Two_Colors;
	//_Strip -> Moving_effect_three_colors = Ws2812b_Moving_Effect_Three_Colors;
	_Strip -> Sliding_effect = Ws2812b_Sliding_Effect;
	_Strip -> Moving_and_vanishing_effect = Ws2812b_Moving_And_Vanishing_Effect;
	_Strip -> Custom_Palette_RGB = Ws2812_Custom_Palette_RGB;
	_Strip -> Custom_Palette_HSV = Ws2812_Custom_Palette_HSV;
	_Strip -> Delay_in_us = Ws2812b_Delay_In_Microseconds;
	_Strip -> Show = Ws2812b_Show;
	_Strip -> Show_without_Delay = Ws2812b_Show_without_Delay;

	return *(_Strip);
}

/*
 * Operational functions
 */

/*
 *  WS2812 Effects
 */

void Ws2812b_SetPixel_HSV(HSV * _Col, U16 Pixelnum)
{
	RGB temp = Hsv_to_rgb(_Col);
	Ws2812b_SetPixel_RGB(&temp, Pixelnum);
}

void Ws2812b_SetPixel_RGB(RGB * _Col, U16 Pixelnum) // GRB Row
{
	set_bit(((WS2812b.buffer + TIME_TO_RST) + (Pixelnum * BITS_IN_PIXEL)), _Col -> Green);
	set_bit(((WS2812b.buffer + TIME_TO_RST) + ((Pixelnum * BITS_IN_PIXEL) + BYTE)), _Col -> Red);
	set_bit(((WS2812b.buffer + TIME_TO_RST) + ((Pixelnum * BITS_IN_PIXEL) + (2 * BYTE))), _Col -> Blue);
}

void Ws2812b_SetStrip_RGB(RGB *_Col)
{
	for (U16 i = 0; i < PIXELS_COUNT; ++i) Ws2812b_SetPixel_RGB(_Col, i);
}

void Ws2812b_SetStrip_HSV(HSV * _Col)
{
	for(U16 i = 0; i < PIXELS_COUNT; ++i) Ws2812b_SetPixel_HSV(_Col, i);

}

void Ws2812b_Moving_And_Vanishing_Effect(RGB *_Col_1, RGB *_Col_2, U16 Delay)
{

	RGB empty;

	empty.Green = 0;
	empty.Red = 0;
	empty.Blue = 0;

	for (U16 i = 0; i < PIXELS_COUNT; ++i)
	{
		if (i % 2 == 0) Ws2812b_SetPixel_RGB(_Col_1, i); else Ws2812b_SetPixel_RGB(_Col_2, i);

		Ws2812b_Show(Delay);
	}

	for (U16 i = (PIXELS_COUNT - 1); i > 0; --i)
	{
		Ws2812b_SetPixel_RGB(&empty, i);
		Ws2812b_Show(Delay);
	}

}

void Ws2812b_Sliding_Effect(RGB *_Col_1, RGB *_Col_2, RGB *_Col_3, U16 Delay)
{

	for (U8 i = 0; i < 3; ++i)
	{

		for (U16 k = 0; k < PIXELS_COUNT; ++k)
		{

			if (i == 0) Ws2812b_SetPixel_RGB(_Col_1, k);

			else if (i == 1) Ws2812b_SetPixel_RGB(_Col_2, k);

			else if (i == 2) Ws2812b_SetPixel_RGB(_Col_3, k);



			Ws2812b_Show(Delay);

		}
	}
}

void Ws2812b_Moving_Effect_Two_Colors(RGB *_Col_1, RGB *_Col_2, U16 Delay)
{
	for (U8 i = 0; i < 2; ++i)
	{
		for (U16 j = 0; j < PIXELS_COUNT; ++j)
		{

			if (i == 0)
			{
				if (j % 2 == 0) Ws2812b_SetPixel_RGB(_Col_1, j);

				else Ws2812b_SetPixel_RGB(_Col_2, j);


			} else if (i == 1)
			{

				if (j % 2 == 0) Ws2812b_SetPixel_RGB(_Col_2, j);

				else Ws2812b_SetPixel_RGB(_Col_1, j);


			}

		}
		Ws2812b_Show(Delay);
	}
}

void Ws2812_Custom_Palette_HSV(Palette_HSV *_palette, U8 palettes_num, U16 Delay)
{
	U16 n = 0; // Led offset
	U16 m = 0; // Led sweeper
	U16 Strip_size;
	U16 Pixels = PIXELS_COUNT;
	Palette_HSV temp;
	HSV arr[palettes_num * _palette -> size];

	if (palettes_num == 1) while (Pixels % _palette -> size != 0) Pixels--; // Protection against non-optimal palettes_num variable selection

	else while (Pixels % (palettes_num * _palette -> size) != 0) Pixels--;





	if (palettes_num == 1) Strip_size = Pixels / _palette -> size;

	else
	{

		for (U8 i = 0; i < palettes_num; ++i) memcpy((void*) (arr + (i * _palette->size)),(void*) (_palette -> parr), _palette -> size * sizeof(HSV));

		temp.parr = arr;
		temp.size = sizeof(arr) / sizeof(HSV);
		Strip_size = Pixels / temp.size;

	}

	for (U16 i = 0; i < Pixels; ++i)
	{
		if (palettes_num == 1)
		{
			for (U16 j = 0; j < _palette -> size; ++j)
			{
				for (U16 k = 0; k < Strip_size; ++k) Ws2812b_SetPixel_HSV((HSV*) &_palette -> parr[j % _palette -> size], (k + n + m) % Pixels);
				m += Strip_size;
			}
		} else
		{
			for (U16 j = 0; j < temp.size; ++j)
			{
				for (U16 k = 0; k < Strip_size; ++k) Ws2812b_SetPixel_HSV((HSV*) &temp.parr[j % temp.size], (k + n + m) % Pixels);
				m += Strip_size;
			}
		}
		n++;
		m = 0;
		Ws2812b_Show(Delay);
	}

}

void Ws2812_Custom_Palette_RGB(Palette_RGB *_palette, U8 palettes_num, U16 Delay)
{
	U16 n = 0; // Led offset
	U16 m = 0; // Led sweeper
	U16 Strip_size;
	U16 Pixels = PIXELS_COUNT;
	Palette_RGB temp;
	RGB arr[palettes_num * _palette -> size];

	if (palettes_num == 1) while (Pixels % _palette -> size != 0) Pixels--; // Protection against non-optimal palettes_num variable selection

	else while (Pixels % (palettes_num * _palette -> size) != 0) Pixels--;



	if (palettes_num == 1) Strip_size = Pixels / _palette -> size;

	else
	{

		for (U8 i = 0; i < palettes_num; ++i) memcpy((void *) (arr + (i * _palette -> size)),(void *) (_palette -> parr), _palette -> size * sizeof(HSV));

		temp.parr = arr;
		temp.size = sizeof(arr) / sizeof(RGB);
		Strip_size = Pixels / temp.size;

	}

	for (U16 i = 0; i < Pixels; ++i)
	{
		if (palettes_num == 1)
		{

			for (U16 j = 0; j < _palette -> size; ++j)
			{
				for (U16 k = 0; k < Strip_size; ++k) Ws2812b_SetPixel_RGB((RGB *) &_palette -> parr[j % _palette -> size],(k + n + m) % Pixels);
				m += Strip_size;
			}
		} else
		{
			for (U16 j = 0; j < temp.size; ++j)
			{
				for (U16 k = 0; k < Strip_size; ++k) Ws2812b_SetPixel_RGB((RGB *) &temp.parr[j % temp.size], (k + n + m) % Pixels);
				m += Strip_size;
			}
		}

		n++;
		m = 0;
		Ws2812b_Show(Delay);
	}

}

/*void Ws2812b_Moving_Effect_Three_Colors(RGB *_Col_1, RGB * _Col_2, RGB * _Col_3, U16 Delay)
 {



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

 for (U16 j = 0; j < Pixels; ++j) // Each of 120 pixels
 {

 for (U16 i = 0; i < Pixels; ++i) // Each of 120 pixels

 {

 if (counter == 0) // first iteration(fill strip with default template)
 {

 if (i >= Red_pos_start && i < Red_pos_end)
 {

 Ws2812b_SetPixel_RGB(_Col_1, i); // R

 } else if (i >= Green_pos_start && i < Green_pos_end)
 {
 Ws2812b_SetPixel_RGB(_Col_2, i); // G
 } else if (i >= Blue_pos_start && i < Blue_pos_end)
 {

 Ws2812b_SetPixel_RGB(_Col_3, i); // B
 }

 } else
 {

 if (Blue_pos_end >= 0 && Blue_pos_end < temp_cur && Blue_pos_start > (2 * temp_cur) && Blue_pos_start <= (3 * temp_cur)) // Blue end begin transfer
 {
 if (i >= 0 && i < Red_pos_start) //B
 {
 Ws2812b_SetPixel_RGB(_Col_3, i);//ws2812b_setpixel(0, 0, Blue, i);
 } else if (i >= Red_pos_start && i < Red_pos_end) //R
 {
 Ws2812b_SetPixel_RGB(_Col_1, i);//ws2812b_setpixel(0, Red, 0, i);
 } else if (i >= Green_pos_start && i < Green_pos_end) //G
 {
 Ws2812b_SetPixel_RGB(_Col_2, i);//ws2812b_setpixel(Green, 0, 0, i);
 } else if (i >= Blue_pos_start && i < Pixels) //B
 {
 Ws2812b_SetPixel_RGB(_Col_3, i);//ws2812b_setpixel(0, 0, Blue, i);
 }

 } else if (Blue_pos_start >= 0 && Blue_pos_end > 0 && Blue_pos_end <= temp_cur && Green_pos_end <= (3 * temp_cur)) // Blue start begin transfer
 {

 if (i >= 0 && i < Blue_pos_end) //B
 {

 Ws2812b_SetPixel_RGB(_Col_3, i);//ws2812b_setpixel(0, 0, Blue, i);

 } else if (i >= Red_pos_start && i < Red_pos_end) // R
 {
 Ws2812b_SetPixel_RGB(_Col_1, i);//ws2812b_setpixel(0, Red, 0, i);

 } else if (i >= Green_pos_start && i < Pixels) // G
 {
 Ws2812b_SetPixel_RGB(_Col_2, i);//ws2812b_setpixel(Green, 0, 0, i);
 }

 } else if (Green_pos_end >= 0 && Green_pos_end < temp_cur && Green_pos_start > (2 * temp_cur) && Green_pos_start <= (3 * temp_cur) && Red_pos_end < (3 * temp_cur)) // green end transfer begin
 {
 if (i >= 0 && i < Blue_pos_start) // G
 {
 Ws2812b_SetPixel_RGB(_Col_2, i);//ws2812b_setpixel(Green, 0, 0, i);
 } else if (i >= Blue_pos_start && i < Blue_pos_end) // B
 {
 Ws2812b_SetPixel_RGB(_Col_3, i);//ws2812b_setpixel(0, 0, Blue, i);
 } else if (i >= Red_pos_start && i < Red_pos_end) // R
 {
 Ws2812b_SetPixel_RGB(_Col_1, i);//ws2812b_setpixel(0, Red, 0, i);
 }

 } else if (Green_pos_start >= 0 && Green_pos_end <= temp_cur && Red_pos_end <= (3 * temp_cur)) // Transfering Green start to begin
 {
 if (i >= 0 && i < Green_pos_end) // G
 {
 Ws2812b_SetPixel_RGB(_Col_2, i);//ws2812b_setpixel(Green, 0, 0, i);
 } else if (i >= Blue_pos_start && i < Blue_pos_end) // B
 {
 Ws2812b_SetPixel_RGB(_Col_3, i);//ws2812b_setpixel(0, 0, Blue, i);
 } else if (i >= Red_pos_start && i < Red_pos_end) // R
 {
 Ws2812b_SetPixel_RGB(_Col_1, i);//ws2812b_setpixel(0, Red, 0, i);
 }

 } else if (Red_pos_end >= 0 && Red_pos_end <= temp_cur && Red_pos_start <= (3 * temp_cur)) // Transfering Red end and Red start to begin
 {
 if (i >= 0 && i < Red_pos_end) // R
 {
 Ws2812b_SetPixel_RGB(_Col_1, i);//ws2812b_setpixel(0, Red, 0, i);
 } else if (i >= Green_pos_start && i < Green_pos_end) // G
 {
 Ws2812b_SetPixel_RGB(_Col_2, i);//ws2812b_setpixel(Green, 0, 0, i);
 } else if (i >= Blue_pos_start && i < Blue_pos_end) //B
 {
 Ws2812b_SetPixel_RGB(_Col_3, i);//ws2812b_setpixel(0, 0, Blue, i);
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

 Ws2812b_Show(Delay);
 counter = 1;

 }
 } */

/*
 *  WS2812 Effects
 */

