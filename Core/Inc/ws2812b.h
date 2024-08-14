/*
 * ws2812b.h
 *
 *  Created on: Jul 26, 2024
 *      Author: irubt
 */

#ifndef INC_WS2812B_H_
#define INC_WS2812B_H_

/* Includes ------------------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "main.h"
#include "stdbool.h"
#include "math.h"

#define STM32F1

#ifdef STM32F1
    #include "stm32f1xx_hal_conf.h"

#elif defined(STM32F2)
    #include "stm32f2xx_hal_conf.h"

#elif defined(STM32F3)
    #include "stm32f3xx_hal_conf.h"

#elif defined(STM32F4)
    #include "stm32f4xx_hal_conf.h"

#elif defined(STM32F7)
    #include "stm32f7xx_hal_conf.h"

#elif defined(STM32H7)
    #include "stm32h7xx_hal_conf.h"

#elif defined(STM32L0)
    #include "stm32l0xx_hal_conf.h"

#elif defined(STM32L1)
    #include "stm32l1xx_hal_conf.h"

#elif defined(STM32L4)
    #include "stm32l4xx_hal_conf.h"

#elif defined(STM32L5)
    #include "stm32l5xx_hal_conf.h"

#elif defined(STM32G0)
    #include "stm32g0xx_hal_conf.h"

#elif defined(STM32G4)
    #include "stm32g4xx_hal_conf.h"

#elif defined(STM32WB)
    #include "stm32wbxx_hal_conf.h"

#elif defined(STM32WL)
    #include "stm32wlxx_hal_conf.h"

#elif defined(STM32MP1)
    #include "stm32mp1xx_hal_conf.h"

#endif

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
/* USER CODE END PTD */



/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */


#define DWT_CONTROL *(volatile unsigned long *)0xE0001000
#define SCB_DEMCR   *(volatile unsigned long *)0xE000EDFC


#define STRIP_SIZE 10
#define PIXELS_COUNT 120 // Changeable parameter(number of pixels in your led strip)

#define BITS_IN_PIXEL 24
#define BYTE 8

#define TIME_TO_RST 50 // Should be greater than 50 us

#define LEDS_COUNT (PIXELS_COUNT * BITS_IN_PIXEL) + TIME_TO_RST

/* USER CODE END PD */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */



/*
 * ENUMS
 */
typedef enum
{
	PERIOD = 90 - 1,
	LOGIC_ZERO = 26 - 1, // in ns
	LOGIC_ONE = 65 - 1 // in ns

} Led_states;

typedef enum
{
	GREEN = 0,
	RED = 1,
	BLUE = 2

}Colors;

/*
 * ENUMS
 */

/*
 * STRUCTS
 */
typedef struct
{
	U8 Red;
	U8 Green;
	U8 Blue;

}Color;

typedef struct
{
	U16 buffer[LEDS_COUNT];
	TIM_HandleTypeDef * htim1;
	TIM_TypeDef * TIM;

}WS2812_hw;

typedef struct
{
	U16 Hue; // Hue >= 0 deg є Hue <= 360 deg
	U8 Saturation; // Saturation >= 0% є Saturation <= 100%
	U8 Value; // Value >= 0% є Value <= 100% (Brightness)

}HSV;

typedef struct
{
	void (*init)(TIM_HandleTypeDef *  htim, TIM_TypeDef * TIM);
	void (*set_pixel)(Color * Col, U16 Pixelnum);
	void (*set_pixel_hsv)(HSV * Col, U16 Pixelnum);
	void (*setstrip)(Color *_Col);
	void (*show)(U16 Delay);
	void (*moving_effect_two_colors)(Color *_Col_1, Color * _Col_2,  U16 Delay);
	void (*moving_effect_three_colors)(Color *_Col_1, Color * _Col_2, Color * _Col_3, U16 Delay);
	void (*sliding_effect)(Color *_Col_1, Color * _Col_2, Color * _Col_3, U16 Delay);
	void (*moving_and_vanishing_effect)(Color *_Col_1, Color * _Col_2, U16 Delay);
	void(*delay_in_us)(U32 us);

}WS2812;



/*
 * STRUCTS
 */

/* USER CODE END PV */


/* Private function prototypes -----------------------------------------------*/

/* USER CODE BEGIN PFP */


/*
 * Description : Initializes the Strip struct fields
 *
 * Prototype void ws2812b_init(TIM_HandleTypeDef *  htim, TIM_TypeDef * TIM;
 *
 * Parameters :
 *
 * TIM_HandleTypeDef *  htim - pointer on object of TIM_HandleTypeDef struct
 * TIM_TypeDef * TIM - pointer on object of TIM_TypeDef struct
 */

void ws2812b_init(TIM_HandleTypeDef *  htim, TIM_TypeDef * TIM);

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

void ws2812b_setpixel(Color * Col, U16 Pixelnum);



 /*
  * Description : Lights up the whole strip with specified color
  *
  * Prototype : void ws2812b_setstrip(U8 Green, U8 Red, U8 Blue);
  *
  * Parameters :
  *
  *	Color *_Col - pointer on object of Color struct which contains three fields :
  *
  *	U8 Red - Sets the brightness of Red color in the whole stripe from 0 to 255
  *
  * U8 Green - Sets the brightness of Green color in the whole stripe from 0 to 255
  *
  * U8 Blue - Sets the brightness of Blue color in the whole stripe from 0 to 255
  *
  */
void ws2812b_setstrip(Color *_Col);

/*
 * Description : Sends the specified effect to strip
 *
 * Prototype : void ws2812b_show(U16 Delay);
 *
 * Parameters :
 *
 * U16 Delay - Led animations delay
 *
 */

void ws2812b_show(U16 Delay);


/*
 *
 * EFFECTS FUNCTIONS
 *
 */

/* Description : Dynamic effect #1
 *
 * Prototype : void ws2812b_moving_effect_two_colors(Color *_Col_1, Color * _Col_2,  U16 Delay);
 *
 *
 * Parameters :
 *
 * Color *_Col_1 - pointer on object of Color struct
 *
 * Color *_Col_2 - pointer on object of Color struct
 *
 * U16 Delay - Sets the moving speed(recommended minimum is 10)
 *
 */

void ws2812b_moving_effect_two_colors(Color *_Col_1, Color * _Col_2,  U16 Delay);

/*
 * Description : Dynamic effect #2
 *
 * Prototype : void ws2812b_moving_effect_three_colors(Color *_Col_1, Color * _Col_2, Color * _Col_3, U16 Delay);
 *
 * Parameters :
 *
 * Color *_Col_1 - pointer on object of Color struct
 *
 * Color *_Col_2 - pointer on object of Color struct
 *
 * U16 Delay - Sets the moving speed(recommended minimum is 10)
 *
 */

void ws2812b_moving_effect_three_colors(Color *_Col_1, Color * _Col_2, Color * _Col_3, U16 Delay);

/* Description : Dynamic effect #3
 *
 * Prototype : void ws2812b_sliding_effect(Color *_Col_1, Color * _Col_2, Color * _Col_3, U16 Delay);
 *
 * Parameters :
 *
 * Color *_Col_1 - pointer on object of Color struct
 *
 * Color *_Col_2 - pointer on object of Color struct
 *
 * Color *_Col_3 - pointer on object of Color struct
 *
 * U16 Delay - Sets the moving speed(recommended minimum is 10)
 *
 */

void ws2812b_sliding_effect(Color *_Col_1, Color * _Col_2, Color * _Col_3, U16 Delay);



/* Description : Dynamic effect #4
 *
 * Prototype : void ws2812b_moving_and_vanishing_effect(Color *_Col_1, Color * _Col_2, U16 Delay);
 *
 * Parameters :
 *
 * Color *_Col_1 - pointer on object of Color struct
 *
 * Color *_Col_2 - pointer on object of Color struct
 *
 * U16 Delay - Sets the moving speed(recommended minimum is 10)
 *
 */

void ws2812b_moving_and_vanishing_effect(Color *_Col_1, Color * _Col_2, U16 Delay);


/*
 * Description : Constructor
 *
 * Prototype : WS2812 new_Strip(WS2812 * _Strip);
 *
 * Parameters :
 *
 * WS2812 * _Strip - Pointer to an object of type WS2812
 */
WS2812 new_Strip(WS2812 * _Strip);


/*
 * Description : Delay in microseconds
 *
 * Prototype : void ws2812b_delay_in_microseconds(U32 us);
 *
 * Parameters :
 *
 * U32 us - Delay time(in microseconds)
 */
void ws2812b_delay_in_microseconds(U32 us);


void ws2812b_set_pixel_hsv(HSV * Col, U16 Pixelnum);
/* USER CODE END PFP */


#endif /* INC_WS2812B_H_ */
