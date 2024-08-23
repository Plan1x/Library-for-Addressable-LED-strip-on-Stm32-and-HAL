
# 📝Content

* [Introduction](https://github.com/Plan1x/Ws2812b-Stm32-HAL-LED-Driver?tab=readme-ov-file#-introduction)

* [License](https://github.com/Plan1x/Ws2812b-Stm32-HAL-LED-Driver?tab=readme-ov-file#license)

* [Requirements]()

* [Guide Video](https://github.com/Plan1x/Ws2812b-Stm32-HAL-LED-Driver?tab=readme-ov-file#guide-video)

* [Circuit](https://github.com/Plan1x/Ws2812b-Stm32-HAL-LED-Driver?tab=readme-ov-file#circuit)

* [Usage](https://github.com/Plan1x/Ws2812b-Stm32-HAL-LED-Driver?tab=readme-ov-file#usage)

* [Functions](https://github.com/Plan1x/Ws2812b-Stm32-HAL-LED-Driver?tab=readme-ov-file#%EF%B8%8Ffunctions)

* [Bugs and Shortcomings](https://github.com/Plan1x/Ws2812b-Stm32-HAL-LED-Driver?tab=readme-ov-file#bugs-and-shortcomings)

* [How to reach me](https://github.com/Plan1x/Ws2812b-Stm32-HAL-LED-Driver?tab=readme-ov-file#-how-to-reach-me)

# 📜 Introduction

A library for simple and fast control of addressable LED strips based on Stm32 controllers and the HAL library.

# 🪪License

<p align="left">


<a href="https://github.com/Plan1x/Ws2812b-Stm32-HAL-LED-Driver/blob/main/LICENSE" target="blank"><img align="center" src="https://upload.wikimedia.org/wikipedia/commons/thumb/1/12/Cc-by-nc-sa_icon.svg/1200px-Cc-by-nc-sa_icon.svg.png" alt="plan1xaa" height="50" width="150" /></a>
  
</p>

# 📋Requirements

* Addressable LED strip.
  **Supported Chips: WS2812, WS2812b, WS2811, WS2813, SK6812, WS2815, and GS8208 (I use WS2812)**

* Stm32 microcontroller.
**Bluepill is recommended(Stm32f103c8t6)**

* Power supply.  **I use a 50 Watt power supply (5 V, 10 A)**

* Resistor 300 - 500 Ohm

* Logic Level Converter (3.3 to 5 V)

# 📷Guide Video

**Soon**

# ⚡Circuit
![Circuit](https://github.com/user-attachments/assets/5e5a63ae-fa09-4585-8518-46d9f253ffa7)


# 👨‍💻Usage

## 🛠️Hardware

1. Create a new Stm32 project

2. Set the maximum permissible core clock frequency on the mcu you are using (on mine it is 72 MHz)

3. Configure any channel of any timer in PWM Generation mode (I use the first channel of the first timer)
4. Configuring Timer and DMA Parameters: 
### Timer Parameters
* Prescaler - 0
* Counter Period:
  
In order to obtain the desired period of the PWM signal, you should use the formula:

```Counter Period = (1.25 us * core clock frequency) - 1``` 
* Auto-reload preload - Enable
* PWM MODE - PWM mode 1
* Pulse - 0 (Or substitute here any value less than Counter Period if you want to make sure that you get a PWM signal with the correct period)

### DMA Parameters

* Mode - Circular
* DMA Request - TIMx_CHx
* Direction - Memory to Peripheral
* Data Width : Peripheral - Half Word (16 Bits), Memory - Byte (8 Bits)

### Checking the correct hardware settings (optionaly)

In the body of the main function

```C 
  /* USER CODE BEGIN 2 */

	HAL_TIM_PWM_Start_IT(&htimx, TIM_CHANNEL_x);

  /* USER CODE END 2 */

```
If everything is configured correctly, then by connecting an oscilloscope to the PWM configured GPIO pin we will receive a sequence of rectangular pulses with a period of 1.25 μs.

![oscillogram](https://github.com/user-attachments/assets/e881e5d2-36a5-4134-a390-23d405e24c92)

## 💻Software 
1. Install the [Library](https://github.com/Plan1x/Ws2812b-Stm32-HAL-LED-Driver/tree/main/Ws2812_HAL_Driver) 
2. Include [ws2812b.h](https://github.com/Plan1x/Ws2812b-Stm32-HAL-LED-Driver/blob/main/Ws2812_HAL_Driver/Inc/ws2812b.h) and [Colors.h](https://github.com/Plan1x/Ws2812b-Stm32-HAL-LED-Driver/blob/main/Ws2812_HAL_Driver/Inc/Colors.h) in main.c file

```C
 /* Private includes ----------------------------------------------------------*/

/* USER CODE BEGIN Includes */

#include "ws2812b.h"
#include "Colors.h"

/* USER CODE END Includes */
```
3. Create a global WS2812 structure object in main.c

```C
/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

WS2812 Strip;

/* USER CODE END PV */
```
4. Call the constructor in the body of the main function

```C
  /* USER CODE BEGIN 1 */


	Strip = New_Strip(&Strip);


  /* USER CODE END 1 */
```
5. Call the Init function within the body of the main function

```C
  /* USER CODE BEGIN 2 */

	Strip.Init(&htimx, TIMx); // Substitute the x with the number of the timer that you configured

  /* USER CODE END 2 */
```
6. The last step is to go to the [ws2812b.h](https://github.com/Plan1x/Ws2812b-Stm32-HAL-LED-Driver/blob/main/Ws2812_HAL_Driver/Inc/ws2812b.h) file and change the parameter to the number of pixels on your LED strip
```C
/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define PIXELS_COUNT Number of pixels on your LED strip
```
Congratulations! 
This is all you had to do before you started using the library. Pretty simple, right?



# ♾️Functions

**void Set_pixel_RGB(RGB * _Col, U16 Pixelnum);**

Create an RGB Structure Object
```C
/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
WS2812 Strip;

RGB Color;
/* USER CODE END PV */

```
Inside the main function

```C
/* USER CODE BEGIN 1 */


	Strip = New_Strip(&Strip);

	Color.Red = 255; // Brightness of Red color led from 0 to 255
	Color.Green = 0; // Brightness of Green color led from 0 to 255
	Color.Blue = 0; // Brightness of Blue color led from 0 to 255
	
  /* USER CODE END 1 */
```
```C
 /* USER CODE BEGIN 2 */

	Strip.Init(&htim1, TIM1);
	
	Strip.Set_pixel_RGB(&Color, 0); // Light the first pixel red
	Strip.Show_without_Delay(); // Load data into LED strip

  /* USER CODE END 2 */

```
![1](https://github.com/user-attachments/assets/05a0f42c-4750-4c92-96f4-d23194952611)

**void Set_pixel_HSV(HSV * _Col, U16 Pixelnum);**

Create an HSV Structure Object

```C
/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
WS2812 Strip;

HSV Color;
/* USER CODE END PV */
```
Inside the main function

```C
/* USER CODE BEGIN 1 */


	Strip = New_Strip(&Strip);

	Color.Hue = 0; // Hue from 0 to 360 degrees ( https://en.wikipedia.org/wiki/HSL_and_HSV )
	Color.Saturation = 100; // Hue saturation from 0% to 100%
	Color.Value = 100; // Brightness from 0% to 100%
	

  /* USER CODE END 1 */
```
```C
  /* USER CODE BEGIN 2 */

	Strip.Init(&htim1, TIM1);

	Strip.Set_pixel_HSV(&Color, 1); // Light the second pixel red
	Strip.Show_without_Delay(); // Load data into LED strip

  /* USER CODE END 2 */
```
![2](https://github.com/user-attachments/assets/86b97caa-1e99-44bd-bfbc-1a7193ffafce)

**void Setstrip_RGB (RGB * _Col);**

Create an RGB Structure Object
```C
/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
WS2812 Strip;

RGB Color;
/* USER CODE END PV */

```
Inside the main function

```C
/* USER CODE BEGIN 1 */


	Strip = New_Strip(&Strip);

	Color.Red = 0; // Brightness of Red color led from 0 to 255
	Color.Green = 255; // Brightness of Green color led from 0 to 255
	Color.Blue = 0; // Brightness of Blue color led from 0 to 255
	
  /* USER CODE END 1 */
```
```C
  /* USER CODE BEGIN 2 */

	Strip.Init(&htim1, TIM1);

	Strip.Setstrip_RGB(&Color); // Light up the entire LED strip green
	Strip.Show_without_Delay(); // Load data into LED strip

  /* USER CODE END 2 */
```
![3](https://github.com/user-attachments/assets/fa560588-5523-4690-a21a-e05533756e81)

**void SetStrip_HSV(HSV * _Col);**

Create an HSV Structure Object

```C
/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
WS2812 Strip;

HSV Color;
/* USER CODE END PV */
```
Inside the main function

```C
/* USER CODE BEGIN 1 */


	Strip = New_Strip(&Strip);

	Color.Hue = 277; // Hue from 0 to 360 degrees ( https://en.wikipedia.org/wiki/HSL_and_HSV )
	Color.Saturation = 100; // Hue saturation from 0% to 100%
	Color.Value = 100; // Brightness from 0% to 100%
	

  /* USER CODE END 1 */
```
```C
  /* USER CODE BEGIN 2 */

	Strip.Init(&htim1, TIM1);

	Strip.SetStrip_HSV(&Color); // Light up the entire LED strip purple
	Strip.Show_without_Delay(); // Load data into LED strip

  /* USER CODE END 2 */
```
![4](https://github.com/user-attachments/assets/b1f29f56-0b92-4c08-b025-6a37872b16dc)

**void Moving_effect_two_colors(RGB * _Col_1, RGB * _Col_2,  U16 Delay);**

Create two RGB structure objects

```C
/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
WS2812 Strip;

RGB Color_1, Color_2;
/* USER CODE END PV */
```
Inside the main function
```C
/* USER CODE BEGIN 1 */


	Strip = New_Strip(&Strip);

	Color_1.Red = 255;
	Color_2.Blue = 255;



  /* USER CODE END 1 */
```

Inside the while(1)

```C
/* USER CODE BEGIN WHILE */
	while (1)
	{

		Strip.Moving_effect_two_colors(&Color_1, &Color_2, 100); // Moving effect with 100 ms Delay 


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
```
![5](https://github.com/user-attachments/assets/1cf504f7-6dfc-41ee-878d-e32703dcf9c4)

**void Sliding_effect(RGB * _Col_1, RGB * _Col_2, RGB * _Col_3, U16 Delay);**

Create three RGB structure objects

```C
/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
WS2812 Strip;

RGB Color_1, Color_2, Color_3;
/* USER CODE END PV */
```
Inside the main function
```C
/* USER CODE BEGIN 1 */


	Strip = New_Strip(&Strip);

	Color_1.Red = 255;
	Color_2.Blue = 255;
    Color_3.Green = 255;


  /* USER CODE END 1 */
```
Inside the while(1)

```C
/* USER CODE BEGIN WHILE */
	while (1)
	{

	Strip.Sliding_effect(&Color_1, &Color_2, &Color_3, 20); // Slidig effect with 20 ms Delay 


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
```
![5 (1)](https://github.com/user-attachments/assets/0c61d561-d644-4ba7-bad8-4eed124c1601)

**void Moving_and_vanishing_effect(RGB * _Col_1, RGB * _Col_2, U16 Delay);**

Create two RGB structure objects

```C
/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
WS2812 Strip;

RGB Color_1, Color_2;
/* USER CODE END PV */
```
Inside the main function
```C
/* USER CODE BEGIN 1 */


	Strip = New_Strip(&Strip);

	Color_1.Red = 255;
	Color_2.Green = 255;



  /* USER CODE END 1 */
```

Inside the while(1)

```C
/* USER CODE BEGIN WHILE */
	while (1)
	{

		Strip.Moving_and_vanishing_effect(&Color_1, &Color_2, 30); // Effect with 30 ms Delay 


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
```
![7 (1)](https://github.com/user-attachments/assets/33d67677-22ab-4282-b163-a5edf2e4b45b)

**void Custom_Palette_RGB(Palette_RGB * _palette, U8 palettes_num, U16 Delay);**

This function allows you to display your own color palette on the LED strip in RGB format. 
In the header file [Colors.h](https://github.com/Plan1x/Ws2812b-Stm32-HAL-LED-Driver/blob/main/Ws2812_HAL_Driver/Inc/Colors.h) you can find several test palettes. I'll Palette_RGB test

I'll be using the Palette_RGB test palette, but you can create any palette you want! In order for the palette to be evenly displayed on the LED strip, you need to make sure that the number of colors in your palette is a multiple of the number of pixels on it

Inside the while(1)

```C
/* USER CODE BEGIN WHILE */
	while (1)
	{

		Strip.Custom_Palette_RGB(&test, 5, 40); // Running five test palettes simultaneously with a 40 ms delay


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
```
![8-ezgif com-cut](https://github.com/user-attachments/assets/874ceb7e-cb34-400c-b99c-499f16766e12)

**void Custom_Palette_HSV(Palette_HSV * _palette, U8 palettes_num, U16 Delay);**

Same function as Custom_Palette_RGB, but now we have the ability to create a palette by specifying colors in HSV format

```C
/* USER CODE BEGIN WHILE */
	while (1)
	{

	Strip.Custom_Palette_HSV(&Rainbow, 1, 30); // Running one Rainbow palette with a 30 ms delay


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
```
![9-ezgif com-cut](https://github.com/user-attachments/assets/3b3daeee-7830-4328-92fb-f62e839811ed)


**If you want to get acquainted with the syntax of each function in more detail, go to the header file [ws2812b.h](https://github.com/Plan1x/Ws2812b-Stm32-HAL-LED-Driver/blob/main/Ws2812_HAL_Driver/Inc/ws2812b.h). I left a description for each function there next to it's prototype**

# ✍Bugs and Shortcomings
**If you find any bugs or shortcomings in my library, or if you know how to improve the functionality, I will be very grateful if you let me know about it**


# 🔗 How to reach me
<p align="left">


<a href="https://t.me/plan1xaa" target="blank"><img align="center" src="https://cdn.pixabay.com/photo/2021/12/27/10/50/telegram-icon-6896828_960_720.png" alt="plan1xaa" height="50" width="50" /></a>
  
</p>

