/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define STP_Y_DIR_Pin GPIO_PIN_0
#define STP_Y_DIR_GPIO_Port GPIOC
#define STP_X_DIR_Pin GPIO_PIN_1
#define STP_X_DIR_GPIO_Port GPIOC
#define MOT_SPI_MISO_Pin GPIO_PIN_2
#define MOT_SPI_MISO_GPIO_Port GPIOC
#define MOT_SPI_MOSI_Pin GPIO_PIN_3
#define MOT_SPI_MOSI_GPIO_Port GPIOC
#define JS_VRX_Pin GPIO_PIN_0
#define JS_VRX_GPIO_Port GPIOA
#define JS_VRY_Pin GPIO_PIN_1
#define JS_VRY_GPIO_Port GPIOA
#define JS_SW_Pin GPIO_PIN_2
#define JS_SW_GPIO_Port GPIOA
#define PROBE_0_Pin GPIO_PIN_4
#define PROBE_0_GPIO_Port GPIOA
#define PROBE_1_Pin GPIO_PIN_5
#define PROBE_1_GPIO_Port GPIOA
#define RS485_RX_Pin GPIO_PIN_5
#define RS485_RX_GPIO_Port GPIOC
#define RS485_TX_Pin GPIO_PIN_10
#define RS485_TX_GPIO_Port GPIOB
#define MOT_SPI_NSS_Pin GPIO_PIN_12
#define MOT_SPI_NSS_GPIO_Port GPIOB
#define MOT_SPI_SCK_Pin GPIO_PIN_13
#define MOT_SPI_SCK_GPIO_Port GPIOB
#define RS485_EN_Pin GPIO_PIN_14
#define RS485_EN_GPIO_Port GPIOB
#define STP_MS1_Pin GPIO_PIN_6
#define STP_MS1_GPIO_Port GPIOC
#define WS2812_DI_Pin GPIO_PIN_7
#define WS2812_DI_GPIO_Port GPIOC
#define SD_CD_Pin GPIO_PIN_8
#define SD_CD_GPIO_Port GPIOA
#define ESP_RX_Pin GPIO_PIN_10
#define ESP_RX_GPIO_Port GPIOA
#define ESP_TX_Pin GPIO_PIN_15
#define ESP_TX_GPIO_Port GPIOA
#define STP_MS2_Pin GPIO_PIN_10
#define STP_MS2_GPIO_Port GPIOC
#define STP_MS3_Pin GPIO_PIN_12
#define STP_MS3_GPIO_Port GPIOC
#define STP_Z_STEP_Pin GPIO_PIN_3
#define STP_Z_STEP_GPIO_Port GPIOB
#define STP_Y_STEP_Pin GPIO_PIN_4
#define STP_Y_STEP_GPIO_Port GPIOB
#define STP_X_STEP_Pin GPIO_PIN_6
#define STP_X_STEP_GPIO_Port GPIOB
#define STP_Z_DIR_Pin GPIO_PIN_7
#define STP_Z_DIR_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
