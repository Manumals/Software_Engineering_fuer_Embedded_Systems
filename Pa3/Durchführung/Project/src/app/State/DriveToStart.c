/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       DriveToStart.c
 *
 *    Module handles driving to the startline 
 */
/**************************************************************************************************/

/* INCLUDES ***************************************************************************************/
#include "DriveToStart.h"

#include "app/DriveHandler.h"
#include "service/LineSensor.h"
#include "service/Buzzer.h"

/* CONSTANTS **************************************************************************************/
#define MAX_START_TIME_s (10U)
#define MAX_LAP_TIME_s (20U)

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/
static SoftTimer* gMaxTimeTimer;
static SoftTimer* gLapTimer;

LineSensorValues* sensorValues;

/* EXTERNAL FUNCTIONS *****************************************************************************/

void DriveToStart_StartMotorsAndTimer(void)
{
    SoftTimer_Ret status;

    DriveHandler_FollowGuideLine(sensorValues);

    if (SOFT_TIMER_UNREGISTERED == gMaxTimeTimer->state)
    {
        status = SoftTimerHandler_register(gMaxTimeTimer);
    }
    if (SOFTTIMER_RET_SUCCESS == status)
    {
        SoftTimer_start(gMaxTimeTimer, MAX_START_TIME_s);
    }
}

EventEnum DriveToStart_FollowGuideLine(void)
{
    EventEnum retEvent = NO_EVENT_HAS_HAPPEND;

    LineSensor_read(sensorValues);

    if (SOFTTIMER_IS_EXPIRED(gMaxTimeTimer))
    {
        retEvent = DRIVE_TO_START_IS_ACTIVE_FOR_TOO_LONG;
    }
    else if ((500 < sensorValues->value[LINESENSOR_LEFT]) && (500 < sensorValues->value[LINESENSOR_RIGHT]))
    {
        retEvent = START_FINISH_LINE_WAS_RECOGINZED;
    }

    return retEvent;
}

void DriveToStart_StopTimer(void)
{
    SoftTimer_Stop(gMaxTimeTimer);
}

SoftTimer* DriveToStart_StartTimerAndBeep(void)
{
    SoftTimer_Ret status;
    SoftTimer* retTimer;

    if (SOFT_TIMER_UNREGISTERED == gLapTimer->state)
    {
        status = SoftTimerHandler_register(gLapTimer);
    }
    if (SOFTTIMER_RET_SUCCESS == status)
    {
        SoftTimer_start(gLapTimer, MAX_LAP_TIME_s);
        retTimer = gLapTimer;
    }

    Buzzer_beep(BUZZER_NOTIFY);

    return retTimer;
}

/* INTERNAL FUNCTIONS *****************************************************************************/