/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       InitializeMcu.c
 *
 *    Module initializes the microcomputingunit and its components
 */
/**************************************************************************************************/

/* INCLUDES ***************************************************************************************/
#include "app/State/InitializeMcu.h"

#include "os/ErrorHandler.h"

#include "hal/PWM.h"
#include "hal/IRQ.h"
#include "hal/TickTimer.h"
#include "hal/GPIO.h"

#include "service/Button.h"
#include "service/Buzzer.h"
#include "service/Display.h"
#include "service/LED.h"
#include "service/DriveControl.h"
#include "service/LineSensor.h"


/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/

EventEnum InitializeMcu_InitializeAll(void)
{
  //HAL
  if (Gpio_init(void) != 0)
  {
    ErrorHandler_halt(ERRORHANDLER_STARTUP_INIT_FAIL);
  }
  Pwm_init(void);
  Tick_Timer_init(void);
  Irq_init(void);
  //Service
  Button_init(void);
  LineSensor_init(void);
  Display_init(void);
  Buzzer_init(void);
  DriveControl_init(void);
  LED_init(void);
} 

void InitializeMcu_DisplayTeamName(void)
{
  Display_clear(void);
  Display_gotoxy(0,0);
  char teamName[] = "~~ o=o\\";
  Display_write(teamName);
}
/* INTERNAL FUNCTIONS *****************************************************************************/