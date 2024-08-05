/*
 * ws2812b.h
 *
 *  Created on: Jul 26, 2024
 *      Author: irubt
 */

#ifndef INC_WS2812B_H_
#define INC_WS2812B_H_

#include "main.h"

#define PIXELS_COUNT 60
#define LEDS_COUNT 1440
#define BITS_IN_PIXEL 24
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
	U8 count;

}Strip;

void ws2812b_init(void);
void ws2812b_setpixel(uint8_t Green, uint8_t Red,  uint8_t Blue, uint8_t Pixelnum);
void ws2812b_setstrip(uint8_t Green, uint8_t Red,   uint8_t Blue);
void ws2812b_fading(uint8_t Color);
void ws2812b_light_on(void); // starts DMA
void ws2812b_light_off(void); // stops DMA
#endif /* INC_WS2812B_H_ */
