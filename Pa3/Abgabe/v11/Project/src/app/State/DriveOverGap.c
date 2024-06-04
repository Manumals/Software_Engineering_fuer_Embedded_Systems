/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       DriveOverGap.c
 *
 *    Module performs the necessary maneuvers to drive over a gap 
 */
/**************************************************************************************************/

/* INCLUDES ***************************************************************************************/
#include "DriveOverGap.h"

#include "app/DriveHandler.h"
#include "service/LineSensor.h"

/* CONSTANTS **************************************************************************************/
#define STATE_TIME_MAX_MS (5000)
#define SENSOR_THRESHOLD (500)
/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/

static UInt16 gEntryTime;

/* EXTERNAL FUNCTIONS *****************************************************************************/

void DriveOverGap_saveCurrentLapTime(SoftTimer* lapTimer)
{
    gEntryTime = SoftTimer_get(lapTimer);
}

EventEnum DriveOverGap_driveOverGap(SoftTimer* lapTimer)
{
    DriveHandler_findGuideLine();

    EventEnum returnEnum = NO_EVENT_HAS_HAPPEND;
    LineSensorValues sensorValues;
    LineSensor_read(&sensorValues);

    for (UInt8 counterSensors = 0; counterSensors < 3; counterSensors++)
    {   
        if (SENSOR_THRESHOLD < sensorValues.value[counterSensors])
        {
            returnEnum = THE_GUIDELINE_WAS_RECOGNIZED;
            counterSensors = 3; /* Stop loop*/
        }
    }

    if (SOFTTIMER_IS_EXPIRED(lapTimer))
    {
        returnEnum = LAPTIME_IS_TOO_LONG;
    }
    else if (STATE_TIME_MAX_MS >= (gEntryTime - SoftTimer_get(lapTimer)))
    {
        returnEnum = DRIVE_OVER_GAP_IS_ACTIVE_FOR_TOO_LONG;
    }

    return returnEnum;
}

/* INTERNAL FUNCTIONS *****************************************************************************/