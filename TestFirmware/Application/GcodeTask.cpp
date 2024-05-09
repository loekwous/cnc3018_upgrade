/*
 * GcodeTask.cpp
 *
 *  Created on: May 8, 2024
 *      Author: loek
 */

#include "GcodeTask.h"

#include <cstdio>
#include <cmsis_os2.h>

#include "Drivers/GCodeStorage.h"
#include "Drivers/StackCheckHelper.h"
#include "Drivers/EndSwitches.h"

namespace {

constexpr const char *GCODE_TASK_NAME = "GcodeTask";
constexpr uint32_t GCODE_TASK_STACK_SIZE_BYTES = 1024 * 32;
constexpr osPriority_t GCODE_TASK_PRIORITY = osPriorityNormal;

constexpr uint32_t QUEUE_SIZE = 10;
constexpr uint32_t GCODE_MESSAGE_SIZE = 32;
constexpr uint32_t QUEUE_TIMEOUT_MS = 100;
constexpr uint32_t SEMAPHORE_TIMEOUT_MS = 1;

osThreadId_t threadHandle;
osThreadAttr_t threadAttributes;
osMessageQueueId_t queueHandle;

osSemaphoreId_t semaphoreSwitches;
osSemaphoreAttr_t semaphoreAttributes;

/**
 * @brief This structure holds the raw GCODE string received from e.g. UART
 */
struct GCodeMessage {
	char message[GCODE_MESSAGE_SIZE];
};

}

static void Run(void *arguments) {
	(void) arguments;

	printf("Started GCODE task\n");

	semaphoreSwitches = osSemaphoreNew(1, 0, &semaphoreAttributes);
	assert(semaphoreSwitches != nullptr);
	if (!semaphoreSwitches) {
		perror("Semaphore failed to create");
		Error_Handler();
	}

	Drivers::EndSwitches::RegisterSemaphore(&semaphoreSwitches);

	Drivers::GCodeStorage gcodeStorage;

	if (!gcodeStorage.Init()) {
		perror("GCode storage failed");
	} else {
		printf("GCode storage initialized\n");
	}

	for (;;) {
		GCodeMessage message;
		if (osMessageQueueGet(queueHandle, &message, nullptr,
				pdMS_TO_TICKS(QUEUE_TIMEOUT_MS)) == osOK) {
			asm volatile("nop");
		}

		if (osSemaphoreAcquire(semaphoreSwitches,
				pdMS_TO_TICKS(SEMAPHORE_TIMEOUT_MS)) == osOK) {
			fprintf(stderr, "END switch has been clicked!!!!!!\n");
		}

		Drivers::CheckStackUsage(threadHandle, GCODE_TASK_STACK_SIZE_BYTES, 40);
	}
}

bool Tasks::GCode::Start() {
	queueHandle = osMessageQueueNew(QUEUE_SIZE, sizeof(GCodeMessage), nullptr);
	if (queueHandle != nullptr) {
		threadAttributes.name = GCODE_TASK_NAME;
		threadAttributes.priority = GCODE_TASK_PRIORITY;
		threadAttributes.stack_size = GCODE_TASK_STACK_SIZE_BYTES;
		threadHandle = osThreadNew(Run, nullptr, &threadAttributes);
	}
	return (queueHandle != nullptr) && (threadHandle != nullptr);
}

