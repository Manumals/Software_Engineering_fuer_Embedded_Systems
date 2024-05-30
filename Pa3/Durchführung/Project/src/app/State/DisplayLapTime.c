/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       DisplayLapTime.c
 *
 *    Module handles displaying the time needed to complete one lap on the OLED display
 *    and stops the timer as well as the power to the motors 
 */
/**************************************************************************************************/

/* INCLUDES ***************************************************************************************/
#include "DisplayLapTime.h"

#include "app/DriveHandler.h"
#include "service/Display.h"
#include "service/Buzzer.h"
/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/

EventEnum DisplayLapTime_StopAfterLap(SoftTimer* lapTimer)
{
    /* Stop the lapTime timer */
    SoftTimer_Stop(lapTimer);

    /* Stops the power supply to the DriveMotors */
    DriveHandler_StopDriving();

    /* Emit a short beep */
    Buzzer_beep(BUZZER_NOTIFY);
}

void DisplayLapTime_DisplayLapTime(SoftTimer* lapTimer)
{
    /* Shows the completed lap time on the OledDisplay */
    #define TIME_LEN_MAX (10)
    UInt16 time = SoftTimer_get(lapTimer);
    Display_gotoxy(0, 2);
    char strTime[TIME_LEN_MAX] = {0};
    sprintf(strTime, "%.2u:%0.3u", time / 1000U, time % 1000U);
    Display_write(strTime, TIME_LEN_MAX);
}

/* INTERNAL FUNCTIONS *****************************************************************************/