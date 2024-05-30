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
#include "InitializeMcu.h"

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
  /* Initialize HAL modules */
  if (GPIO_RET_OK != Gpio_init())
  {
    ErrorHandler_halt(ERRORHANDLER_STARTUP_INIT_FAIL);
  }
  Pwm_init();
  Tick_Timer_init();
  Irq_init();
  /* Initialize Service modules */
  Button_init();
  LineSensor_init();
  Display_init();
  Buzzer_init();
  DriveControl_init(vid);
  LED_init();

  return INIZALIZATION_DONE;
} 

void InitializeMcu_DisplayTeamName(void)
{
  Display_clear();
  Display_gotoxy(0,0);
  const char teamName[] = "~~ o=o\\";
  Display_write(teamName);
}
/* INTERNAL FUNCTIONS *****************************************************************************/