#ifndef CONFIG_H_
#define CONFIG_H_

#include "stm8s.h"

/* ------------------------------- INPUTS ----------------------------------*/

// Board should be powered with car IGINITION, so no input for that

// ACC is used to detect that IGINITION and ACC are on and also to detect
// that engine is started by voltage on this input.
// The trick is that remote autostart power up only IGNITION, only when
// key in the lock ACC is present. No lights while autostart.
#define ACC_PORT GPIOD
#define ACC_PIN GPIO_PIN_2
#define ACC_ADC ADC1_CHANNEL_3
#define ACC_SCHMITTTRIG ADC1_SCHMITTTRIG_CHANNEL3

// Side lights input, this input is used to turn off DRL
#define SIDE_LIGHTS_PORT GPIOC
#define SIDE_LIGHTS_PIN GPIO_PIN_7

// Light sensor input
#define LIGHT_SENSOR_PORT GPIOD
#define LIGHT_SENSOR_PIN GPIO_PIN_3
#define LIGHT_SENSOR_ADC ADC1_CHANNEL_4
#define LIGHT_SENSOR_SCHMITTTRIG ADC1_SCHMITTTRIG_CHANNEL4

/* ------------------------------- OUTPUTS ---------------------------------*/

// Daytime running lights relay. Active on low.
#define DRL_PORT GPIOB
#define DRL_PIN GPIO_PIN_4

// Headlight relay. Side lights turns on automatically.
#define HEADLIGHTS_PORT GPIOC
#define HEADLIGHTS_PIN GPIO_PIN_3

// on board LED to display status
#define LED_PORT GPIOD
#define LED_PIN GPIO_PIN_4

/* -------------------------------- CONFIG ---------------------------------*/
// These thresholds should be find out experimentally. Use debug output.
#define ACC_VOLTAGE_THRESHOLD 718 // 13.5V
#define LIGHT_SENSOR_THRESHOLD_DAY_TO_NIGHT 400 // threshold for evening
#define LIGHT_SENSOR_THRESHOLD_NIGHT_TO_DAY 75 // threshold for morning (hysteresis)
#define ENGINE_START_DETECT_DELAY_MS 3000
#define HEADLIGHTS_SWITCH_DELAY_MS 30000
#if ACC_VOLTAGE_THRESHOLD <= 0
#error ACC_VOLTAGE_THRESHOLD is too small
#endif
#if ACC_VOLTAGE_THRESHOLD >= 1023
#error ACC_VOLTAGE_THRESHOLD is too big
#endif
#if LIGHT_SENSOR_THRESHOLD_DAY_TO_NIGHT <= 0
#error LIGHT_SENSOR_THRESHOLD_DAY_TO_NIGHT is too small
#endif
#if LIGHT_SENSOR_THRESHOLD_DAY_TO_NIGHT >= 1023
#error LIGHT_SENSOR_THRESHOLD_DAY_TO_NIGHT is too big
#endif
#if LIGHT_SENSOR_THRESHOLD_NIGHT_TO_DAY <= 0
#error LIGHT_SENSOR_THRESHOLD_NIGHT_TO_DAY is too small
#endif
#if LIGHT_SENSOR_THRESHOLD_NIGHT_TO_DAY >= 1023
#error LIGHT_SENSOR_THRESHOLD_NIGHT_TO_DAY is too big
#endif
#if ENGINE_START_DETECT_DELAY_MS >= 32768
#error ENGINE_START_DETECT_DELAY_MS is too big
#endif
#if HEADLIGHTS_SWITCH_DELAY_MS >= 32768
#error HEADLIGHTS_SWITCH_DELAY_MS is too big
#endif
#define DEBUG // enable messages via UART


#endif /* CONFIG_H_ */
