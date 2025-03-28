#ifndef INC_STM32F0XX_GPIO_DRIVER_H_
#define INC_STM32F0XX_GPIO_DRIVER_H_

/* **************************************************
 *			    	INCLUDES					    *
 *************************************************  */
#include <math.h>
#include <types.h>
#include <stm32f446xx.h>

/* **************************************************
 *					DEFINES 					    *
 *************************************************  */

/** Macro that is used for separation of low and high pins */
#define PIN_SEPARATION				7u

/* GPIOx_MODER - Modes */
#define INPUT_MODE 					0u
#define GENERAL_PURPOSE_OUTPUT_MODE 1u
#define ALTERNATE_FUNCTION_MODE 	2u
#define ANALOG_MODE 				3u
#define FALING_TRIGGER				4u
#define RISING_TRIGGER				5u
#define RISING_FALLING_TRIGGER		6u

/* GPIOx_OSPEEDR - Speed modes */
#define LOW_SPEED 					0u
#define MEDIUM_SPEED				1u
#define HIGH_SPEED					3u

/* GPIOx_PUPDR - Pull-up/pull-down configuration */
#define NO_UP_DOWN					0u
#define PULL_UP						1u
#define PULL_DOWN					2u
#define RESERVED_PUPDR				3u

/* GPIOx_OTYPER - Output configuration */
#define OUTPUT_PUSH_PULL			0u
#define OUTPUT_OPEN_DRAIN			1u

/* GPIOx_AFRx - Alternate functionality configuration */
#define AF0							0u
#define AF1 						1u
#define AF2 						2u
#define AF3 						3u
#define AF4 						4u
#define AF5 						5u
#define AF6 						6u
#define AF7 						7u

/* Macros used for clearing bit fields */

#define CLEAR_1_BIT					1u
#define CLEAR_2_BITS 				3u
#define CLEAR_4_BITS				4u

/* Macro that represents value of last GPIOx PIN */

#define GPIOX_LAST_PIN				15u

/* **************************************************
 *			    GLOBAL VARIABLES 					*
 *************************************************  */

/** Structure that holds GPIO pin configuration parameters */
typedef struct
{
	uint8 GPIO_PinNumber;		/** Number if GPIOx pin */
	uint8 GPIO_PinMode;			/** Selected mode for given pin */
	uint8 GPIO_PinSpeed;		/** Speed of pin */
	uint8 GPIO_PinPuPdControl;	/** Pull-up, pull-down configuration of pin*/
	uint8 GPIO_PinOPType;		/** GPIO pin output settings*/
	uint8 GPIO_PinAltFunMode;	/** Alternate function mode */
}GPIO_PinConfig_t;

/* **************************************************
 *			    FUNCTION PROTOTYPE 					*
 *************************************************  */

/**
 *
 * \brief      - TODO
 * \param[in]  - TODO
 * \return     - TODO
 * \globals    - TODO
 *
 */
void GPIO_v_PeripheralClockControl(GPIO_TypeDef *pGPIOx, uint8 enabledOrDisabled);

/**
 *
 * \brief      - TODO
 * \param[in]  - TODO
 * \return     - TODO
 * \globals    - TODO
 *
 */
void GPIO_v_Init(GPIO_TypeDef *pGPIOx, GPIO_PinConfig_t GPIO_PinConfiguration);

/**
 *
 * \brief      - TODO
 * \param[in]  - TODO
 * \return     - TODO
 * \globals    - TODO
 *
 */
void GPIO_v_DeInit(GPIO_TypeDef *pGPIOx);

/**
 *
 * \brief      - TODO
 * \param[in]  - TODO
 * \return     - TODO
 * \globals    - TODO
 *
 */
uint8 GPIO_u_ReadFromInputPin(GPIO_TypeDef *pGPIOx, uint8 u_pinNumber);

/**
 *
 * \brief      - TODO
 * \param[in]  - TODO
 * \return     - TODO
 * \globals    - TODO
 *
 */
uint32 GPIO_u_ReadFromInputPort(GPIO_TypeDef *pGPIOx);

/**
 *
 * \brief      - TODO
 * \param[in]  - TODO
 * \return     - TODO
 * \globals    - TODO
 *
 */
void GPIO_v_WriteToOutputPin(GPIO_TypeDef *pGPIOx, uint8 u_pinNumber, uint8 u_value);

/**
 *
 * \brief      - TODO
 * \param[in]  - TODO
 * \return     - TODO
 * \globals    - TODO
 *
 */
void GPIO_v_WriteToOutputPort(GPIO_TypeDef *pGPIOx, uint32 u_value);

/**
 *
 * \brief      - TODO
 * \param[in]  - TODO
 * \return     - TODO
 * \globals    - TODO
 *
 */
void GPIO_v_TogglePin(GPIO_TypeDef *pGPIOx, uint8 u_pinNumber);

/**
 *
 * \brief      - TODO
 * \param[in]  - TODO
 * \return     - TODO
 * \globals    - TODO
 *
 */
void GPIO_v_IRQInteruptConfig(uint8 IRQNumber, uint8 u_enableOrDisable);

/**
 *
 * \brief      - TODO
 * \param[in]  - TODO
 * \return     - TODO
 * \globals    - TODO
 *
 */
void GPIO_v_IRQPrioConfig(uint8 IRQPrio, uint8 u_IRQPriority);

/**
 *
 * \brief      - TODO
 * \param[in]  - TODO
 * \return     - TODO
 * \globals    - TODO
 *
 */
void GPIO_v_IRQHandling(uint8 u_pinNumber);

/**
 *
 * \brief      - TODO
 * \param[in]  - TODO
 * \return     - TODO
 * \globals    - TODO
 *
 */
void GPIOx_v_ModeConfig(GPIO_TypeDef *pGPIOx, GPIO_PinConfig_t GPIO_PinConfiguration);

/**
 *
 * \brief      - TODO
 * \param[in]  - TODO
 * \return     - TODO
 * \globals    - TODO
 *
 */
void GPIOx_v_SpeedConfig(GPIO_TypeDef *pGPIOx, GPIO_PinConfig_t GPIO_PinConfiguration);

/**
 *
 * \brief      - TODO
 * \param[in]  - TODO
 * \return     - TODO
 * \globals    - TODO
 *
 */
void GPIOx_v_PupdrConfig(GPIO_TypeDef *pGPIOx, GPIO_PinConfig_t GPIO_PinConfiguration);

/**
 *
 * \brief      - TODO
 * \param[in]  - TODO
 * \return     - TODO
 * \globals    - TODO
 *
 */
void GPIOx_v_OutputConfig(GPIO_TypeDef *pGPIOx, GPIO_PinConfig_t GPIO_PinConfiguration);

/**
 *
 * \brief      - TODO
 * \param[in]  - TODO
 * \return     - TODO
 * \globals    - TODO
 *
 */
void GPIOx_v_AlternateFunctionConfig(GPIO_TypeDef *pGPIOx, GPIO_PinConfig_t GPIO_PinConfiguration);

/**
 *
 * \brief      - TODO
 * \param[in]  - TODO
 * \return     - TODO
 * \globals    - TODO
 *
 */
void GPIOx_v_BitSetResetConfig(GPIO_TypeDef *pGPIOx, GPIO_PinConfig_t GPIO_PinConfiguration, uint8 u_pinSetOrReset);

/**
 *
 * \brief      - TODO
 * \param[in]  - TODO
 * \return     - TODO
 * \globals    - TODO
 *
 */
uint8 GPOIx_u_PortCheck(GPIO_TypeDef *pGPIOx);

/**
 *
 * \brief      - TODO
 * \param[in]  - TODO
 * \return     - TODO
 * \globals    - TODO
 *
 */
void GPIOx_v_GPIOCfgStructClear(GPIO_PinConfig_t * pGpioCfg);

#endif /* INC_STM32F0XX_GPIO_DRIVER_H_ */
