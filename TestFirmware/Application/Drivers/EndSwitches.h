/*
 * EndSwitches.h
 *
 *  Created on: May 9, 2024
 *      Author: loek
 */

#ifndef DRIVERS_ENDSWITCHES_H_
#define DRIVERS_ENDSWITCHES_H_

#include <cstdint>
#include <cmsis_os.h>
#include <etl/array.h>
#include <etl/vector.h>
#include <stm32f4xx_hal.h>

namespace Drivers {

class EndSwitches {
public:
	EndSwitches() = delete;
	~EndSwitches() = delete;
	static constexpr uint32_t NUMBER_OF_SEMAPHORES = 10;

	static etl::vector<osSemaphoreId_t*, NUMBER_OF_SEMAPHORES> semaphoreInstances;
	static constexpr auto GPIO_PINS = etl::make_array<uint16_t>(GPIO_PIN_0);
	static void RegisterSemaphore(osSemaphoreId_t* semaphore);
	static void InterruptHandler(uint16_t GPIO_Pin);
};

} /* namespace Drivers */

#endif /* DRIVERS_ENDSWITCHES_H_ */
