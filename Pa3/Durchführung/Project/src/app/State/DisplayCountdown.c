/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       DisplayCountdown.c
 *
 *    Module handles displaying the countdown on the OLED-display 
 */
/**************************************************************************************************/

/* INCLUDES ***************************************************************************************/
#include <stdio.h>

#include "DisplayCountdown.h"
#include "os/SoftTimer.h"
#include "service/Display.h"

/* CONSTANTS **************************************************************************************/
#define TIME_MAX_MS 3000

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/
static SoftTimer gCountdownTimer;
static UInt8 gLastTime;

/* EXTERNAL FUNCTIONS *****************************************************************************/

void DisplayCountdown_StartCountdown(void)
{
    gLastTime = 0U;
    SoftTimer_init(&gCountdownTimer);
    SoftTimerHandler_register(&gCountdownTimer);
    SoftTimer_start(&gCountdownTimer, TIME_MAX_MS);
}

EventEnum DisplayCountdown_DisplayCountdown(void)
{
    UInt16 time_ms = SoftTimer_get(&gCountdownTimer);
    UInt8  time_s = (time_ms + 999U) / 1000U; /* round up to full seconds */

    /* Display countdown in seconds if the second did change */
    if (gLastTime != time_s)
    {
        #define TIME_LEN_MAX (10)
        gLastTime = time_s;
        Display_gotoxy(0, 1);
        char time[TIME_LEN_MAX] = {0};
        sprintf(time, "%u", time_s);
        Display_write(time, TIME_LEN_MAX);
    }

    /* return value for state handle */
    if (SOFTTIMER_IS_EXPIRED(&gCountdownTimer))
    {
        return COUNTDOWN_IS_FINISHED;
    }
    else
    {
        return NO_EVENT_HAS_HAPPEND;
    }
}

void DisplayCountdown_StopCountdown(void)
{
    SoftTimer_Stop(&gCountdownTimer);
    SoftTimerHandler_unRegister(&gCountdownTimer);
}

/* INTERNAL FUNCTIONS *****************************************************************************/