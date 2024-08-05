/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ws2812b.h"
#include "LoRa.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern Strip WS2812b;
LoRa sx1278;
uint16_t test;
uint8_t buff[1] = {0,};
uint8_t counter = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

uint8_t Lora_rx(LoRa lora, U8 *_buff);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	ws2812b_init();

	sx1278 = newLoRa();
   // buff[0] = 0x01;

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM1_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

  WS2812b.htim1 = htim1;

  sx1278.CS_port         = NSS_GPIO_Port;
   sx1278.CS_pin          = NSS_Pin;
   sx1278.reset_port      = RST_GPIO_Port;
   sx1278.reset_pin       = RST_Pin;
   sx1278.DIO0_port       = DIO0_GPIO_Port;
   sx1278.DIO0_pin        = DIO0_Pin;
   sx1278.hSPIx           = &hspi1;

   test = LoRa_init(&sx1278);





  ws2812b_light_on();
  //HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (U32 *)WS2812b.buffer, LEDS_COUNT);
  HAL_Delay(50);
  ws2812b_light_off();//HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);

  HAL_Delay(500);


	ws2812b_setstrip(127, 0, 0);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while(1)
	{
		 for(U8 i = 0; i < 56; i++)
				{

			 	 		 ws2812b_setpixel(166,0,0, i);
			 	 		 ws2812b_setpixel(0,125,0, i + 1);
			 	 		 ws2812b_setpixel(0,0,125, i + 2);
			 	 		ws2812b_setpixel(100,0,125, i + 3);
			 	 		ws2812b_setpixel(0,100,125, i + 4);


					 ws2812b_light_on();
					 HAL_Delay(50);
					ws2812b_light_off();
					ws2812b_setstrip(127, 0, 0);
					 //HAL_Delay(1);
					 //ws2812b_init();

		//			 ws2812b_light_on();
		//			   //HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (U32 *)WS2812b.buffer, LEDS_COUNT);
		//			   HAL_Delay(50);
		//			   ws2812b_light_off();//HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);

						// HAL_Delay(50);

				}






//		Lora_rx(sx1278, buff);
//		if(buff[0] == 0x01)
//		{
//			buff[0] = 0;
//			ws2812b_setstrip(127,0,0);
//			HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (U32 *)WS2812b.buffer, LEDS_COUNT);
//			HAL_Delay(100);
//			HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
//
//		}else if(buff[0] == 0x02)
//		{
//			buff[0] = 0;
//			ws2812b_setstrip(0,127,0);
//			HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (U32 *)WS2812b.buffer, LEDS_COUNT);
//			HAL_Delay(100);
//			HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
//
//		}else if(buff[0] == 0x03)
//		{
//			buff[0] = 0;
//			ws2812b_setstrip(0,0,127);
//			HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (U32 *)WS2812b.buffer, LEDS_COUNT);
//			HAL_Delay(100);
//		    HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
//		}

////ws2812b_fading(BLUE);
//		ws2812b_setstrip(127,0,0);
//		HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (U32 *)WS2812b.buffer, LEDS_COUNT);
//		HAL_Delay(2000);
//		HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
//		ws2812b_setstrip(0,127,0);
//		HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (U32 *)WS2812b.buffer, LEDS_COUNT);
//	    HAL_Delay(2000);
//		HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
//		ws2812b_setstrip(0,0,127);
//		HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (U32 *)WS2812b.buffer, LEDS_COUNT);
//		HAL_Delay(2000);
//		HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);



    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
uint8_t Lora_rx(LoRa lora, U8 *_buff)
{
	uint8_t val;
	LoRa_startReceiving(&lora);
	val = LoRa_receive(&lora, _buff, 1);
	HAL_Delay(50);
	LoRa_gotoMode(&lora, STNBY_MODE);
	return val;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
			/* User can add his own implementation to report the HAL error return state */
			__disable_irq();
			while (1) {
			}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
