/*
 * WS2812B.cpp
 *
 *  Created on: May 8, 2024
 *      Author: loek
 */

#include "WS2812B.h"
#include "assert.h"

extern "C" {
extern TIM_HandleTypeDef htim2;

//void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
//{
//	HAL_TIM_PWM_Stop_DMA(&htim2, TIM_CHANNEL_3);
//}
}

namespace Drivers {

/**
 * @brief Get byte to write to PWM array. Bit is counted from MSB
 * @param color Color value to extract bit from
 * @param bit Bit offset from MSB of color value
 * @return Value corresponding to WS2812 one or zero
 */
uint8_t WS2812B::GetPWMByteFromColor(uint8_t color, uint8_t bit) {
	assert(bit < 8);
	return (color & (0x80 >> bit)) ?
			WS2812B::m_bitOnValue : WS2812B::m_bitOffValue;
}

static_assert(WS2812B::NUMBER_OF_PIXELS <= UINT8_MAX, "This number of pixels cannot be indexed");

void WS2812B::Init() {

	Clear();
	HAL_TIM_PWM_Start_DMA(m_timer, TIM_CHANNEL_3, m_visualBuffer,
			sizeof(m_visualBuffer) / sizeof(uint32_t));

}

void WS2812B::Clear() {
	for (uint8_t index = 0; index < WS2812B::NUMBER_OF_PIXELS; index++) {
		SetPixel(index, 0, 0, 0);
	}
}

void WS2812B::SetPixel(uint8_t index, uint8_t R, uint8_t G, uint8_t B) {
	assert(index < WS2812B::NUMBER_OF_PIXELS);

	size_t absolutePositon = WS2812B::RESET_VALUE_ITEMS
			+ (static_cast<size_t>(index) * WS2812B::BITS_PER_PIXEL);

	for (uint8_t bit = 0; bit < 8; bit++) {
		m_visualBuffer[absolutePositon + GREEN_OFFSET + bit] =
				GetPWMByteFromColor(G, bit);
		m_visualBuffer[absolutePositon + RED_OFFSET + bit] =
				GetPWMByteFromColor(R, bit);
		m_visualBuffer[absolutePositon + BLUE_OFFSET + bit] =
				GetPWMByteFromColor(B, bit);
	}
}

} /* namespace Drivers */
