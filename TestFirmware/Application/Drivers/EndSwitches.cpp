/*
 * EndSwitches.cpp
 *
 *  Created on: May 9, 2024
 *      Author: loek
 */

#include "Drivers/EndSwitches.h"

#include <assert.h>

extern "C" void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	Drivers::EndSwitches::InterruptHandler(GPIO_Pin);
}

namespace Drivers {

void EndSwitches::RegisterSemaphore(osSemaphoreId_t *semaphore) {
	assert(semaphore);
	assert(semaphoreInstances.available());
	osSemaphoreAcquire(*semaphore, 0);
	semaphoreInstances.push_back(semaphore);
}

void EndSwitches::InterruptHandler(uint16_t GPIO_Pin) {

	if (etl::find(GPIO_PINS.begin(), GPIO_PINS.end(), GPIO_Pin)
			!= GPIO_PINS.end()) {

		for (auto *semaphore : semaphoreInstances) {
			if (semaphore) {
				osStatus_t status = osSemaphoreRelease(*semaphore);
				assert(status == osOK);
			}
		}
	}
}

etl::vector<osSemaphoreId_t*, EndSwitches::NUMBER_OF_SEMAPHORES> EndSwitches::semaphoreInstances;

}
/* namespace Drivers */
