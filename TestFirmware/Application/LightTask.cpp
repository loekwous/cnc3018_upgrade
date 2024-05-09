/*
 * LightTask.cpp
 *
 *  Created on: May 8, 2024
 *      Author: loek
 */

#include "LightTask.h"

#include <cstdio>

#include <cmsis_os.h>

#include "Drivers/WS2812B.h"
#include "Drivers/StackCheckHelper.h"

extern TIM_HandleTypeDef htim2;
extern DMA_HandleTypeDef hdma_tim2_ch3_up;

namespace {

constexpr const char *LIGHT_TASK_NAME = "LightTask";
constexpr uint32_t LIGHT_TASK_STACK_SIZE_BYTES = 2048 * 4;
constexpr osPriority_t LIGHT_TASK_PRIORITY = osPriorityRealtime;

constexpr uint32_t QUEUE_SIZE = 10;
constexpr uint32_t QUEUE_TIMEOUT_MS = 100;

osThreadId_t threadHandle;
osThreadAttr_t threadAttributes;
osMessageQueueId_t queueHandle;

struct LightSettings {
	uint8_t percentage = 0;
	uint8_t brightness = 0;
} lightSettings;

struct LightMessage {
	using SystemStatus = Tasks::Light::SystemStatus;
	uint8_t percentage = UINT8_MAX;
	uint8_t brightness = UINT8_MAX;
	SystemStatus status = SystemStatus::UNDEFINED;
};

}

static void UpdateSystemState(Drivers::WS2812B *driver,
		Tasks::Light::SystemStatus status) {
	using SystemStatus = Tasks::Light::SystemStatus;
	assert(driver != nullptr);
	assert(status != SystemStatus::UNDEFINED);

	switch (status) {
	case SystemStatus::OK:
		driver->SetPixel(0, 0, 255, 0);
		break;
	case SystemStatus::BUSY:
		driver->SetPixel(0, 0, 0, 255);
		break;
	case SystemStatus::ERROR:
		driver->SetPixel(0, 255, 0, 0);
		break;
	default:
		break;
	}
}

static void UpdatePositionalLight(Drivers::WS2812B *driver) {
	const uint32_t numberOfLeds = Drivers::WS2812B::NUMBER_OF_PIXELS;
	const uint32_t percentage = static_cast<uint32_t>(lightSettings.percentage);
	const uint32_t activeLed = percentage * (numberOfLeds - 2) / 100 + 1;
	const uint32_t absoluteBrightness = static_cast<uint32_t>(lightSettings.brightness) * 255 / 100;

	assert(activeLed < numberOfLeds);
	assert(activeLed >= 1);
	assert(percentage <= 100);
	assert(absoluteBrightness < 256);

	for (uint8_t index = 1; index < Drivers::WS2812B::NUMBER_OF_PIXELS;
			index++) {
		uint8_t pixelValue =
				(index == activeLed) ? static_cast<uint8_t>(absoluteBrightness) : 0;

		driver->SetPixel(index, pixelValue, pixelValue, pixelValue);

	}
}

static void Run(void *arguments) {
	(void) arguments;
	using SystemStatus = Tasks::Light::SystemStatus;

	Drivers::WS2812B ledString(&htim2, &hdma_tim2_ch3_up);
	printf("Created WS2812B instance\n");
	ledString.Init();
	printf("Initialized WS2812B instance\n");

	for (;;) {
		LightMessage message;
		if (osMessageQueueGet(queueHandle, &message, nullptr,
		pdMS_TO_TICKS(QUEUE_TIMEOUT_MS)) == osOK) {

			Drivers::CheckStackUsage(threadHandle, LIGHT_TASK_STACK_SIZE_BYTES, 40);

			if (message.status != SystemStatus::UNDEFINED) {
				UpdateSystemState(&ledString, message.status);
			} else {

				if (message.percentage != UINT8_MAX) {
					lightSettings.percentage = message.percentage;
				}

				if (message.brightness != UINT8_MAX) {
					lightSettings.brightness = message.brightness;
				}
				UpdatePositionalLight(&ledString);
			}

		}
	}
}

bool Tasks::Light::SetSystemStatus(Tasks::Light::SystemStatus status) {
	if (queueHandle != nullptr) {
		LightMessage message = { .status = status };
		osStatus_t status = osMessageQueuePut(queueHandle, &message,
				osPriorityHigh,
				osWaitForever);
		return (status == osOK);
	}
	return false;
}

bool Tasks::Light::SetXPercentage(uint8_t percentage) {
	if (queueHandle != nullptr) {
		LightMessage message = { .percentage = percentage };
		osStatus_t status = osMessageQueuePut(queueHandle, &message,
				osPriorityHigh,
				osWaitForever);
		return (status == osOK);
	}
	return false;
}

bool Tasks::Light::SetXBrightness(uint8_t percentage) {
	if (queueHandle != nullptr) {
		LightMessage message = { .brightness = percentage };
		osStatus_t status = osMessageQueuePut(queueHandle, &message,
				osPriorityHigh,
				osWaitForever);
		return (status == osOK);
	}
	return false;
}

bool Tasks::Light::Start() {
	queueHandle = osMessageQueueNew(QUEUE_SIZE, sizeof(LightMessage), nullptr);
	if (queueHandle != nullptr) {
		threadAttributes.name = LIGHT_TASK_NAME;
		threadAttributes.priority = LIGHT_TASK_PRIORITY;
		threadAttributes.stack_size = LIGHT_TASK_STACK_SIZE_BYTES;
		threadHandle = osThreadNew(Run, nullptr, &threadAttributes);
	}
	return (queueHandle != nullptr) && (threadHandle != nullptr);
}

