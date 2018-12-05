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
#define LIGHT_SENSOR_THRESHOLD 650
#define ENGINE_START_DETECT_DELAY_MS 3000
#define HEADLIGHTS_SWITCH_DELAY_MS 30000
#define DEBUG // enable messages via UART


#endif /* CONFIG_H_ */
