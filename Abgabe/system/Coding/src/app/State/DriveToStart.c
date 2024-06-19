/***************************************************************************************************
  (c) Team 🏁~~ ō͡≡o\ (Maurice Ott, Simon Walderich, Thorben Päpke) 2024
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
#include "Common/Debug.h"
#include "os/SoftTimer.h"
#include "service/Buzzer.h"
#include "service/LineSensor.h"

/* CONSTANTS **************************************************************************************/
#define MAX_START_TIME_MS (10000U) /**< Maximum allowed time to find the start line */

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/
/** Timer to assess if the maximum time for finding the start line is reached
 *  The maximum time is defined by MAX_START_TIME_MS
*/
static SoftTimer gTimeoutTimer = {0};
static SoftTimer gLapTimer = {0}; /**< Timer to measure the lap time (time needed to drive from start to finsh line) */

/* EXTERNAL FUNCTIONS *****************************************************************************/
void DriveToStart_startMotorsAndTimer(void)
{
    SoftTimer_init(&gTimeoutTimer);
    SoftTimerHandler_register(&gTimeoutTimer);
    SoftTimer_start(&gTimeoutTimer, MAX_LAP_TIME_MS);
    
    LineSensor_enableEmitter();
}

EventEnum DriveToStart_followGuideLine(void)
{
    EventEnum retEvent = NO_EVENT_HAS_HAPPEND;

    LineSensorValues sensorValues;
    LineSensor_read(&sensorValues, NULL);
    #ifdef SHOW_START_LINE_RECOGNITION
        Debug_showLineRecognition(&sensorValues);
    #endif

    if (SOFTTIMER_IS_EXPIRED(&gTimeoutTimer))
    {
        /* Timeout reached -> raise an error */
        retEvent = DRIVE_TO_START_IS_ACTIVE_FOR_TOO_LONG;
        LineSensor_disableEmitter();
        #ifdef SHOW_START_MSGS
            Debug_showMsg("Dri2Strt Timeout");
        #endif
    }
    else if (CALIB_OVER_LINE(sensorValues.value[LINESENSOR_LEFT]) && CALIB_OVER_LINE(sensorValues.value[LINESENSOR_RIGHT]))
    {
        /* Start line was recognized */
        #ifdef SHOW_START_MSGS
            Debug_showMsg("Seeing StartLine!");
        #endif
        retEvent = START_FINISH_LINE_WAS_RECOGINZED;
    }
    else
    {
        /* Nothing new happened, keep following the guide line */
        DriveHandler_followGuideLine(&sensorValues);
    }

    return retEvent;
}

void DriveToStart_stopTimer(void)
{
    SoftTimerHandler_unRegister(&gTimeoutTimer);
}

SoftTimer* DriveToStart_startTimerAndBeep(void)
{
    /* Start the lap timer */
    if (SOFT_TIMER_UNREGISTERED == gLapTimer.state)
    {
        SoftTimer_init(&gLapTimer);
        SoftTimerHandler_register(&gLapTimer);
    }
    SoftTimer_start(&gLapTimer, MAX_LAP_TIME_MS);
    
    #ifdef SHOW_START_MSGS
        Debug_showMsg("Starting lap");
    #endif
    Buzzer_beep(BUZZER_NOTIFY);
    
    return &gLapTimer;
}

/* INTERNAL FUNCTIONS *****************************************************************************/
