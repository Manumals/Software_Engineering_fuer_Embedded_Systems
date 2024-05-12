/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       InitializeMcu.h
 *
 *    Module initializes the microcomputingunit and its components
 */
/**************************************************************************************************/
#ifndef INITIALIZEMCU_H
#define INITIALIZEMCU_H

/* INCLUDES ***************************************************************************************/
#include "EventEnum.h"
#include "Button.h"
#include "Buzzer.h"
#include "Display.h"
#include "LED.h"
#include "DriveControl.h"
#include "LineSensor.h"
#include "GPIO.h"
#include "PWM.h"
#include "TickTimer.h"
#include "IRQ.h"
/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/

/** Initialize the microcontroller with all required variables and inits
*
* @return INIZALIZATION_DONE : The initialization of the MCU is complete 
*/
extern EventEnum InitializeMcu_entry(void);

/** Display the team name on the Oled display 
*
* @return nothing
*/
extern void InitializeMcu_exit(void);

#endif /* INITIALIZEMCU_H */