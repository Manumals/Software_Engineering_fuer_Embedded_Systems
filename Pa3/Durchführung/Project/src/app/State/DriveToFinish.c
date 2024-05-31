/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       DriveToFinish.c
 *
 *    Module takes care of following the guide line 
 */
/**************************************************************************************************/

/* INCLUDES ***************************************************************************************/
#include "DriveToFinish.h"

#include "app/DriveHandler.h"
#include "service/LineSensor.h"

/* CONSTANTS **************************************************************************************/
#define TRESHOLD_LINE_RECOGNIZED 500

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/

EventEnum DriveToFinish_FollowGuideLine(SoftTimer* lapTimer)
{
    EventEnum retEvent = NO_EVENT_HAS_HAPPEND;

    LineSensorValues sensorValues;
    LineSensor_read(&sensorValues);

    DriveHandler_FollowGuideLine(&sensorValues);
    
    if (SOFTTIMER_IS_EXPIRED(lapTimer))
    {
        retEvent = LAPTIME_IS_TOO_LONG;
    }
    else if ((TRESHOLD_LINE_RECOGNIZED < sensorValues.value[LINESENSOR_LEFT]) && (TRESHOLD_LINE_RECOGNIZED < sensorValues.value[LINESENSOR_RIGHT]))
    {
        retEvent = START_FINISH_LINE_WAS_RECOGINZED;
    }
    else if ((TRESHOLD_LINE_RECOGNIZED >= sensorValues.value[LINESENSOR_LEFT]) 
    && (TRESHOLD_LINE_RECOGNIZED >= sensorValues.value[LINESENSOR_MIDDLE_LEFT]) 
    && (TRESHOLD_LINE_RECOGNIZED >= sensorValues.value[LINESENSOR_MIDDLE])
    && (TRESHOLD_LINE_RECOGNIZED >= sensorValues.value[LINESENSOR_MIDDLE_RIGHT])
    && (TRESHOLD_LINE_RECOGNIZED >= sensorValues.value[LINESENSOR_RIGHT]))
    {
        retEvent = GUIDELINE_WAS_LOST;
    }

    return retEvent;
}

/* INTERNAL FUNCTIONS *****************************************************************************/