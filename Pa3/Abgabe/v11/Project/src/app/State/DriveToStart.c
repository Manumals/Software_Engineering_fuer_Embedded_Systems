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
#define MAX_START_TIME_MS (10000U)
#define MAX_LAP_TIME_MS (20000U)

#define TRESHOLD_LINE_RECOGNIZED (500U)

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/
static SoftTimer gMaxTimeTimer;
static SoftTimer gLapTimer;

LineSensorValues sensorValues;

/* EXTERNAL FUNCTIONS *****************************************************************************/

void DriveToStart_startMotorsAndTimer(void)
{
    SoftTimer_Ret status = SOFTTIMER_RET_UNKNOWNTIMER;

    LineSensor_read(&sensorValues);
    SoftTimer_init(&gMaxTimeTimer);

    DriveHandler_followGuideLine(&sensorValues);

    if (SOFT_TIMER_UNREGISTERED == gMaxTimeTimer.state)
    {
        status = SoftTimerHandler_register(&gMaxTimeTimer);
    }
    if (SOFTTIMER_RET_SUCCESS == status)
    {
        SoftTimer_start(&gMaxTimeTimer, MAX_START_TIME_MS);
    }
}

EventEnum DriveToStart_followGuideLine(void)
{
    EventEnum retEvent = NO_EVENT_HAS_HAPPEND;

    LineSensor_read(&sensorValues);

    if (SOFTTIMER_IS_EXPIRED(&gMaxTimeTimer))
    {
        retEvent = DRIVE_TO_START_IS_ACTIVE_FOR_TOO_LONG;
    }
    else if ((TRESHOLD_LINE_RECOGNIZED < sensorValues.value[LINESENSOR_LEFT]) && (TRESHOLD_LINE_RECOGNIZED < sensorValues.value[LINESENSOR_RIGHT]))
    {
        retEvent = START_FINISH_LINE_WAS_RECOGINZED;
    }

    return retEvent;
}

void DriveToStart_stopTimer(void)
{
    SoftTimer_Stop(&gMaxTimeTimer);
    SoftTimerHandler_unRegister(&gMaxTimeTimer);
}

SoftTimer* DriveToStart_startTimerAndBeep(void)
{
    SoftTimer_Ret status = SOFTTIMER_RET_UNKNOWNTIMER;
    SoftTimer* retTimer;
        
    SoftTimer_init(retTimer);
    SoftTimer_init(&gLapTimer);

    if (SOFT_TIMER_UNREGISTERED == gLapTimer.state)
    {
        status = SoftTimerHandler_register(&gLapTimer);
    }
    if (SOFTTIMER_RET_SUCCESS == status)
    {
        SoftTimer_start(&gLapTimer, MAX_LAP_TIME_MS);
        retTimer = &gLapTimer;
    }

    Buzzer_beep(BUZZER_NOTIFY);

    return retTimer;
}

/* INTERNAL FUNCTIONS *****************************************************************************/