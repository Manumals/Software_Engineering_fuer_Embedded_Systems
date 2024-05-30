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
#define STATE_IS_ACTIVE_FOR_TOO_LONG 5000
/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/

static UInt16 gEntryTime;

/* EXTERNAL FUNCTIONS *****************************************************************************/

void DriveOverGap_SaveCurrentLapTime(SoftTimer* lapTimer)
{
    gEntryTime = SoftTimer_get(lapTimer);
}

EventEnum DriveOverGap_DriveOverGap(SoftTimer* lapTimer)
{
    DriveHandler_FindGuideLine();

    EventEnum returnEnum = NO_EVENT_HAS_HAPPEND;
    LineSensorValues* sensorValues;
    LineSensor_read(sensorValues);

    for (UInt8 counterSensors = 0; counterSensors < 3; counterSensors++)
    {   
        if (500 < sensorValues->value[counterSensors])
        {
            returnEnum = THE_GUIDELINE_WAS_RECOGNIZED;
            counterSensors = 3; /** Stop loop*/
        }
    }

    if (STATE_IS_ACTIVE_FOR_TOO_LONG >= (gEntryTime - SoftTimer_get(lapTimer)))
    {
        returnEnum = DRIVE_OVER_GAP_IS_ACTIVE_FOR_TOO_LONG;
    }

    return returnEnum;
}

/* INTERNAL FUNCTIONS *****************************************************************************/