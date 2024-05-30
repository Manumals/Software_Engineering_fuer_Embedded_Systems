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

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/

EventEnum DriveToFinish_followGuideLine(SoftTimer* lapTimer)
{
    EventEnum retEvent = NO_EVENT_HAS_HAPPEND;

    LineSensorValues* sensorValues;
    LineSensor_read(sensorValues);

    DriveHandler_followGuideLine(sensorValues);
    
    if (SOFTTIMER_IS_EXPIRED(lapTimer))
    {
        retEvent = LAPTIME_IS_TOO_LONG;
    }
    else if ((500 < sensorValues->value[LINESENSOR_LEFT]) && (500 < sensorValues->value[LINESENSOR_RIGHT]))
    {
        retEvent = START_FINISH_LINE_WAS_RECOGINZED;
    }
    else if ((500 >= sensorValues->value[LINESENSOR_LEFT]) 
    && (500 >= sensorValues->value[LINESENSOR_MIDDLE_LEFT]) 
    && (500 >= sensorValues->value[LINESENSOR_MIDDLE])
    && (500 >= sensorValues->value[LINESENSOR_MIDDLE_RIGHT])
    && (500 >= sensorValues->value[LINESENSOR_RIGHT]))
    {
        retEvent = GUIDELINE_WAS_LOST;
    }

    return retEvent;
}

/* INTERNAL FUNCTIONS *****************************************************************************/