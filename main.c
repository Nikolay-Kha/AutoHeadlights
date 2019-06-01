#include "stm8s.h"
#include "stm8s_gpio.h"
#include "stm8s_tim1.h"
#include "stm8s_clk.h"
#include "stm8s_adc1.h"
#include "debug.h"
#include "config.h"

#define NUMBER_OF_CYCLE_TO_DISPLAY 1000

uint16_t get_adc_value(ADC1_Channel_TypeDef channel) {
	ADC1_ConversionConfig(ADC1_CONVERSIONMODE_SINGLE, channel, ADC1_ALIGN_RIGHT);
	ADC1_StartConversion();
	while(ADC1_GetFlagStatus(ADC1_FLAG_EOC) == RESET);
	ADC1_ClearFlag(ADC1_FLAG_EOC);
	return ADC1_GetConversionValue();
}

void init_interfaces() {
	// init uart for debug output
	debug_init();

	// init ADC for ACC and light sensor
	GPIO_Init(ACC_PORT, ACC_PIN, GPIO_MODE_IN_FL_NO_IT);
	GPIO_Init(LIGHT_SENSOR_PORT, LIGHT_SENSOR_PIN, GPIO_MODE_IN_FL_NO_IT);
	ADC1_DeInit();
	ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, ACC_ADC, ADC1_PRESSEL_FCPU_D18,
			ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ACC_SCHMITTTRIG, DISABLE);
	ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, LIGHT_SENSOR_ADC, ADC1_PRESSEL_FCPU_D18,
			ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, LIGHT_SENSOR_SCHMITTTRIG, DISABLE);
	ADC1_Cmd(ENABLE);
	// init side lights input
	GPIO_Init(SIDE_LIGHTS_PORT, SIDE_LIGHTS_PIN, GPIO_MODE_IN_FL_NO_IT);

	// init relays, init all of them to prevent LEDs half glow
	GPIO_Init(DRL_PORT, DRL_PIN, GPIO_MODE_OUT_OD_HIZ_FAST);
	GPIO_Init(HEADLIGHTS_PORT, HEADLIGHTS_PIN, GPIO_MODE_OUT_OD_HIZ_FAST);
	GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_OUT_OD_HIZ_FAST);
	GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_OUT_OD_HIZ_FAST);

	// init led
	GPIO_Init(LED_PORT, LED_PIN, GPIO_MODE_OUT_PP_LOW_FAST);

	// init timer3 for time delays
	CLK_PeripheralClockConfig (CLK_PERIPHERAL_TIMER3 , ENABLE);
	TIM1_DeInit();
	// clock source is 2 MHz, prescale is 1000, so one tick is 1 ms
	TIM1_TimeBaseInit(2000, TIM1_COUNTERMODE_UP, 0xFFFF, 0);
	TIM1_GenerateEvent(TIM1_EVENTSOURCE_UPDATE);
	TIM1_Cmd(ENABLE);
}

void blink_led() {
	GPIO_WriteReverse(GPIOB, GPIO_PIN_5); // TODO remove me
	GPIO_WriteReverse(LED_PORT, LED_PIN);
}

void low_pass_filter(uint16_t *acc, uint16_t new_value, uint32_t k_percent) {
	*acc = ((uint32_t)*acc * (uint32_t)(100 - k_percent) + (uint32_t)new_value * k_percent) / (uint32_t)100;
}

void main() {
	uint16_t adc_average;
	uint16_t tick_counter;
	uint16_t i = 0;
	uint8_t headlights_on;
	uint8_t prev_state;
	uint8_t side_lights;
	uint8_t drl;
	uint16_t light_sensor_threshold = LIGHT_SENSOR_THRESHOLD_DAY_TO_NIGHT;
	init_interfaces();
	GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST); // TODO remove me

	debug("Ports are initialized");

	// wait engine until start
	adc_average = get_adc_value(ACC_ADC);
	tick_counter = TIM1_GetCounter();
	while (1) {
		// low pass filter
		low_pass_filter(&adc_average, get_adc_value(ACC_ADC), 10);
		if (adc_average > ACC_VOLTAGE_THRESHOLD) {
			if (TIM1_GetCounter() - tick_counter > ENGINE_START_DETECT_DELAY_MS) {
				// engine has been started for more then ENGINE_START_DETECT_DELAY_MS ago
				debug("Engine started %u", TIM1_GetCounter() - tick_counter);
				break;
			}
		} else {
			tick_counter = TIM1_GetCounter();
		}
		if (++i > NUMBER_OF_CYCLE_TO_DISPLAY) {
			blink_led();
			debug("Waiting for engine start, ADC %u, time %u - %u", adc_average, TIM1_GetCounter(), tick_counter);
			blink_led();
			i = 0;
		}
	}


	// main loop
	adc_average = get_adc_value(LIGHT_SENSOR_ADC);
	// adc_average is 0 in sunny day and 1023 in darkness night
	headlights_on = adc_average > light_sensor_threshold;
	prev_state = headlights_on;
	while (1) {
		// low pass filter
		low_pass_filter(&adc_average, get_adc_value(LIGHT_SENSOR_ADC), 1);
		if (prev_state !=  (adc_average > light_sensor_threshold)) {
			tick_counter = TIM1_GetCounter();
		}
		prev_state = adc_average > light_sensor_threshold;
		if (TIM1_GetCounter() - tick_counter > HEADLIGHTS_SWITCH_DELAY_MS) {
			headlights_on = adc_average > light_sensor_threshold;
		}

		// check side lights to turn on/off DRL
		side_lights = GPIO_ReadInputData(SIDE_LIGHTS_PORT) & SIDE_LIGHTS_PIN;
		drl = !(side_lights || headlights_on);
		if (drl) {
			// turn on DRL
			GPIO_WriteLow(DRL_PORT, DRL_PIN);
		} else {
			// turn off DRL
			GPIO_WriteHigh(DRL_PORT, DRL_PIN);
		}
		if (headlights_on) {
			// turn on headlights
			GPIO_WriteLow(HEADLIGHTS_PORT, HEADLIGHTS_PIN);
			light_sensor_threshold = LIGHT_SENSOR_THRESHOLD_NIGHT_TO_DAY;
		} else {
			// turn off headlights
			GPIO_WriteHigh(HEADLIGHTS_PORT, HEADLIGHTS_PIN);
			// move threshold to implement hysteresis
			light_sensor_threshold = LIGHT_SENSOR_THRESHOLD_DAY_TO_NIGHT;
		}
		if (++i > NUMBER_OF_CYCLE_TO_DISPLAY) {
			blink_led();
			debug("Light level %u, DRL %s, headlights %s, side lights %s", adc_average,
					drl ? "on" : "off", headlights_on ? "on":"off", side_lights ? "on":"off");
			blink_led();
			i = 0;
		}
	}
}
