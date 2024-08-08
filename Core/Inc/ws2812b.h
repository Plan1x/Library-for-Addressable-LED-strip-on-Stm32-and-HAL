/*
 * ws2812b.h
 *
 *  Created on: Jul 26, 2024
 *      Author: irubt
 */

#ifndef INC_WS2812B_H_
#define INC_WS2812B_H_

#include "main.h"

typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;

#define PIXELS_COUNT 48

#define DWT_CONTROL *(volatile unsigned long *)0xE0001000
#define SCB_DEMCR   *(volatile unsigned long *)0xE000EDFC

#ifdef MATRIX_16x16

#define PIXELS_COUNT 255

#endif

#ifdef WS2812b_120

#define PIXELS_COUNT 120

#endif
// Changeable parameter(number of pixels in your led strip)


#define BITS_IN_PIXEL 24
#define BYTE 8

#define TIME_TO_RST 50 // Must be greater than 50 us

#define LEDS_COUNT (PIXELS_COUNT * BITS_IN_PIXEL) + TIME_TO_RST


typedef enum
{
	GREEN = 0,
	RED = 1,
	BLUE = 2

}Colors;


typedef struct
{
	U16 buffer[LEDS_COUNT];
	TIM_HandleTypeDef * htim1;

}Strip;



/*
 * Description : Initializes the led strip by filling it with Black color
 *
 * Prototype : void ws2812b_init(void);
 *
 *
 *
 *
 */


void ws2812b_init(void);

/*
 * Description : Lights up specified pixel with specified color
 *
 * Prototype : void ws2812b_setpixel(U8 Green, U8 Red, U8 Blue, U8 Pixelnum);
 *
 * Parameters :
 *
 * U8 Green - Sets the brightness of Green color led from 0 to 255
 *
 * U8 Red - Sets the brightness of Red color led from 0 to 255
 *
 * U8 Blue - Sets the brightness of Blue color led from 0 to 255
 *
 * U8 Pixelnum - Pixel number that you want to light up
 *
 * !! Pixelnum parameter starts from zero, that means that if you want to light up the first pixel, you should set Pixelnum as 0 !!
 * !! Last pixel index is Pixelnum = (PIXELS_COUNT - 1) !!
 */

void ws2812b_setpixel(U8 Green, U8 Red,  U8 Blue, U8 Pixelnum);

/*
 * Description : Initializes the Strip struct fields(constructor)
 *
 * Prototype ws2812b_new_strip(TIM_HandleTypeDef *  htim);
 *
 * Parameters :
 *
 * TIM_HandleTypeDef *  htim - pointer on object of TIM_HandleTypeDef struct
 *
 */

void ws2812b_new_strip(TIM_HandleTypeDef *  htim);

 /*
  * Description : Lights up the whole strip with specified color
  *
  * Prototype : void ws2812b_setstrip(U8 Green, U8 Red, U8 Blue);
  *
  * Parameters :
  *
  * U8 Green - Sets the brightness of Green color in the whole stripe from 0 to 255
  *
  * U8 Red - Sets the brightness of Red color in the whole stripe from 0 to 255
  *
  * U8 Blue - Sets the brightness of Blue color in the whole stripe from 0 to 255
  */
void ws2812b_setstrip(U8 Green, U8 Red, U8 Blue);

/*
 * Description : Sends the specified effect to strip
 *
 * Prototype : void ws2812b_show(U8 Delay);
 *
 * Parameters :
 *
 * U8 Delay - transferring data speed
 */

void ws2812b_show(U16 Delay);


/*
 *
 * EFFECTS FUNCTIONS
 *
 */

/* Description : Dynamic effect #1
 *
 * Prototype : void ws2812b_moving_effect_two_colors(U8 Color_1, U8 Color_2, Colors * Color_palette, U16 Delay);
 *
 *
 * Parameters :
 *
 * U8 Color_1 - Sets specified color #1 brightness from 0 to 255
 *
 * U8 Color_2 - Sets specified color #2 brightness from 0 to 255
 *
 * Colors * Color_palette - Pointer to Colors array
 *
 * U8 Delay - Sets the moving speed(recommended minimum is 10)
 *
 */

void ws2812b_moving_effect_two_colors(U8 Color_1_brightness, U8 Color_2_brightness, U8 Color_1, U8 Color_2, U16 Delay);

/*
 * Description : Dynamic effect #2
 *
 * Prototype : void ws2812b_moving_effect_three_colors(U8 Red, U8 Green, U8 Blue, U16 Delay);
 *
 * Parameters :
 *
 * U8 Red - Sets the brightness of Red color from 0 to 255
 *
 * U8 Green - Sets the brightness of Green color from 0 to 255
 *
 * U8 Blue - Sets the brightness of Blue color from 0 to 255
 *
 * U8 Delay - Sets the moving speed(recommended minimum is 10)
 *
 */

void ws2812b_moving_effect_three_colors(U8 Red, U8 Green, U8 Blue, U16 Delay);

/* Description : Dynamic effect #3
 *
 * Prototype : void ws2812b_sliding_effect(U8 Red, U8 Green, U8 Blue, U16 Delay);
 *
 * Parameters :
 *
 * U8 Red - Sets the brightness of Red color from 0 to 255
 *
 * U8 Green - Sets the brightness of Green color from 0 to 255
 *
 * U8 Blue - Sets the brightness of Blue color from 0 to 255
 *
 * U8 Delay - Sets the moving speed(recommended minimum is 10)
 *
 */

void ws2812b_sliding_effect(U8 Red, U8 Green, U8 Blue, U16 Delay);



/* Description : Dynamic effect #4
 *
 * Prototype : ws2812b_moving_and_vanishing_effect(U8 Color_1, U8 Color_2, Colors * Color_palette, U16 Delay);
 *
 * Parameters :
 *
 * U8 Color_1_brightness - Sets specified color #1 brightness from 0 to 255
 *
 * U8 Color_2_brightness - Sets specified color #2 brightness from 0 to 255
 *
 * Colors * Color_palette - Pointer to Colors array
 *
 * U8 Delay - Sets the moving speed(recommended minimum is 10)
 *
 */

void ws2812b_moving_and_vanishing_effect(U8 Color_1_brightness, U8 Color_2_brightness, U8 Color_1, U8 Color_2, U16 Delay);

void delay_micros(uint32_t us);


#endif /* INC_WS2812B_H_ */
