/***************************************************************************************************
  (c) Team 🏁~~ ō͡≡o\ (Maurice Ott, Simon Walderich, Thorben Päpke) 2024
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
#include "Common/Debug.h"
#include "service/LineSensor.h"

/* CONSTANTS **************************************************************************************/
#ifdef FAST_GAP_ERROR
    #define STATE_TIME_MAX_MS (300) /**< Timeout time to find line before an error is raised */
#else
    #define STATE_TIME_MAX_MS (5000) /**< Timeout time to find line before an error is raised */
#endif

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/
static UInt16 gEntryTime = 0U; /**< Stores the time of the lapTimer when the line was lost */

/* EXTERNAL FUNCTIONS *****************************************************************************/
void DriveOverGap_saveCurrentLapTime(SoftTimer* lapTimer)
{
    #ifdef SHOW_GAP_MSGS
        Debug_showMsg("Gap!");
    #endif
    gEntryTime = SoftTimer_get(lapTimer);
}

EventEnum DriveOverGap_driveOverGap(SoftTimer* lapTimer)
{
    EventEnum returnEnum = NO_EVENT_HAS_HAPPEND;
    LineSensorValues sensorValues;
    LineSensor_read(&sensorValues, NULL);
    DriveHandler_findGuideLine();

    for (UInt8 counterSensors = 0U; counterSensors < LINESENSOR_COUNT; counterSensors++)
    {   
        if (CALIB_OVER_LINE(sensorValues.value[counterSensors]))
        {
            returnEnum = GUIDELINE_WAS_RECOGNIZED;
            break;
        }
    }

    if (NO_EVENT_HAS_HAPPEND == returnEnum)
    {
        if (SOFTTIMER_IS_EXPIRED(lapTimer))
        {
            #ifdef SHOW_GAP_MSGS
                Debug_showMsg("LapTimeOut");
            #endif
            returnEnum = LAPTIME_IS_TOO_LONG;
        }
        else if (STATE_TIME_MAX_MS <= (gEntryTime - SoftTimer_get(lapTimer)))
        {
            #ifdef SHOW_GAP_MSGS
                Debug_showMsg("GapTimeOut");
                Debug_showMsgNum(gEntryTime, "ms entry");
                Debug_showMsgNum(SoftTimer_get(lapTimer), "ms LapTimer");
            #endif
            returnEnum = DRIVE_OVER_GAP_IS_ACTIVE_FOR_TOO_LONG;
        }
        else
        {
            /* do nothing because nothing is supposed to happen */
        }
    }

    return returnEnum;
}

/* INTERNAL FUNCTIONS *****************************************************************************/
