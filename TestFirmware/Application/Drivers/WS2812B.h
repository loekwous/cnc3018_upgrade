/*
 * WS2812B.h
 *
 *  Created on: May 8, 2024
 *      Author: loek
 */

#ifndef DRIVERS_WS2812B_H_
#define DRIVERS_WS2812B_H_

#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_dma.h>
#include <stm32f4xx_hal_tim.h>
#include <cstring>

namespace Drivers {

class WS2812B {
public:
	WS2812B(TIM_HandleTypeDef *timer, DMA_HandleTypeDef *dma) :
			m_timer(timer), m_dma(dma), m_bitOnValue(
					timer->Init.Period * 2 / 3), m_bitOffValue(
					timer->Init.Period / 3) {
		memset(m_visualBuffer, 0, sizeof(m_visualBuffer));
	}

	virtual ~WS2812B() {
	}

	void Init();
	void SetPixel(uint8_t index, uint8_t R, uint8_t G, uint8_t B);
	void Clear();
	static constexpr size_t NUMBER_OF_PIXELS = 8;
	static constexpr size_t BITS_PER_PIXEL = 24;
private:
	static constexpr size_t GREEN_OFFSET = 0;
	static constexpr size_t RED_OFFSET = 8;
	static constexpr size_t BLUE_OFFSET = 16;
	static constexpr size_t RESET_VALUE_ITEMS = 50;

	uint8_t GetPWMByteFromColor(uint8_t color, uint8_t bit);

	uint32_t m_visualBuffer[NUMBER_OF_PIXELS * BITS_PER_PIXEL
			+ RESET_VALUE_ITEMS];
	TIM_HandleTypeDef *m_timer;
	DMA_HandleTypeDef *m_dma;

	size_t m_bitOnValue = 125 * 2 / 3;
	size_t m_bitOffValue = 125 / 3;

};

}
#endif /* DRIVERS_WS2812B_H_ */
