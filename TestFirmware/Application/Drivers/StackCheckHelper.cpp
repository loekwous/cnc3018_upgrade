/*
 * StackCheckHelper.cpp
 *
 *  Created on: May 9, 2024
 *      Author: loek
 */

#include "StackCheckHelper.h"

void Drivers::CheckStackUsage(osThreadId_t threadId, uint32_t stackSize, uint8_t minimumPercentage) {
	uint32_t stackMinimumFree = osThreadGetStackSpace(threadId);
	uint32_t percentage = stackMinimumFree * 100 / stackSize;

	assert(minimumPercentage < 101);
	assert(percentage < 101);

	if (percentage < minimumPercentage) {
		const char *threadName = osThreadGetName(threadId);
		fprintf(stderr,
				"Thread \"%s\" high watermark reached: %lu free of %lu\n",
				threadName, stackMinimumFree, stackSize);
	}
	assert(percentage >= minimumPercentage);
}
