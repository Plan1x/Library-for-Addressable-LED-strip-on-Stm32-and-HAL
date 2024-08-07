/*
 * ws2812b.h
 *
 *  Created on: Jul 26, 2024
 *      Author: irubt
 */

#ifndef INC_WS2812B_H_
#define INC_WS2812B_H_

#include "main.h"


#define PIXELS_COUNT 120 // Changeable parameter

#define BITS_IN_PIXEL 24
#define LEDS_COUNT (PIXELS_COUNT * BITS_IN_PIXEL)
#define BYTE 8

typedef enum
{
	GREEN = 0,
	RED = 1,
	BLUE = 2

}Colors;

typedef struct
{
	U16 buffer[LEDS_COUNT];
	TIM_HandleTypeDef  htim1;

}Strip;






void ws2812b_init(void); // Init`s the led strip(filling pwm dma buffer with logic zeros(ws2812b black color))
void ws2812b_setpixel(uint8_t Green, uint8_t Red,  uint8_t Blue, uint8_t Pixelnum); //Lights specified pixel
void ws2812b_setstrip(uint8_t Green, uint8_t Red,   uint8_t Blue); // Lights the whole strip
void ws2812b_moving_effect_three_colors(U8 Red, U8 Green, U8 Blue, U8 Delay);
void ws2812b_sliding_effect(U8 Red, U8 Green, U8 Blue, U8 Delay);
void ws2812b_show(U8 Delay);
#endif /* INC_WS2812B_H_ */
