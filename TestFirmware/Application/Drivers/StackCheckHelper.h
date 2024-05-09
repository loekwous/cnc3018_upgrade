/*
 * StackCheckHelper.h
 *
 *  Created on: May 9, 2024
 *      Author: loek
 */

#ifndef DRIVERS_STACKCHECKHELPER_H_
#define DRIVERS_STACKCHECKHELPER_H_

#include <cmsis_os2.h>
#include <assert.h>
#include <cstdio>

namespace Drivers {

void CheckStackUsage(osThreadId_t threadId, uint32_t stackSize, uint8_t minimumPercentage = 40);

}

#endif /* DRIVERS_STACKCHECKHELPER_H_ */
