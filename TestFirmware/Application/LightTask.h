/*
 * LightTask.h
 *
 *  Created on: May 8, 2024
 *      Author: loek
 */

#ifndef LIGHTTASK_H_
#define LIGHTTASK_H_

#include <cstdint>

namespace Tasks::Light {

enum class SystemStatus {UNDEFINED, OK, BUSY, ERROR};

bool Start();

bool SetSystemStatus(SystemStatus status);

bool SetXPercentage(uint8_t percentage);

bool SetXBrightness(uint8_t percentage);

}

#endif /* LIGHTTASK_H_ */
