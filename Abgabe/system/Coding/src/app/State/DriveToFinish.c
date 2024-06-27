/***************************************************************************************************
  (c) Team 🏁~~ ō͡≡o\ (Maurice Ott, Simon Walderich, Thorben Päpke) 2024
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
#include "Common/Debug.h"
#include "service/LineSensor.h"
#ifdef FAST_LAP
    #include "app/State/SetParameters.h"
#endif

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/
/** Only detect a gap after this amount of unsuccessfull line detections
 *  This is especially necessary because the distance between inner and outer line sensors
 *  is very big and leads to false gap detections otherwise
 *  false negative gap detection with speed 100: >207<=210,
 *  false positive gap detection: >=150 (strong derailing: >=300 no maximum limit possible)
 */
#define LINE_LOST_MAX (300U)
#define LAP_TIME_MIN ((MAX_LAP_TIME_MS) - 1000U) /**< Minimum lap time to avoid assessing start line as finish line */

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/
/** Count consecutive unsuccessfull line detections of all sensors
 *  Also see description of LINE_LOST_MAX
 */
static UInt16 gLineLostCount = 0U;

/* EXTERNAL FUNCTIONS *****************************************************************************/
EventEnum DriveToFinish_followGuideLine(SoftTimer* lapTimer)
{
    EventEnum retEvent = NO_EVENT_HAS_HAPPEND;

    LineSensorValues sensorValues;
    LineSensor_read(&sensorValues, NULL);

    #ifdef FAST_LAP
        if (PARAM_SETS_COUNT - 2U <= SetParameters_getCurrentParamSetIdx())
        {
            return DriveHandler_driveFastLap(lapTimer->counter, &sensorValues, PARAM_SETS_COUNT - 1U == SetParameters_getCurrentParamSetIdx());
        }
    #endif

    DriveHandler_followGuideLine(&sensorValues);
    
    if (SOFTTIMER_IS_EXPIRED(lapTimer))
    {
        retEvent = LAPTIME_IS_TOO_LONG;
    }
    else if ((CALIB_OVER_LINE(sensorValues.value[LINESENSOR_LEFT]) && CALIB_OVER_LINE(sensorValues.value[LINESENSOR_RIGHT]))
    && (LAP_TIME_MIN >= SoftTimer_get(lapTimer)))
    {
        /* Recognized finish line */
        retEvent = START_FINISH_LINE_WAS_RECOGINZED;
    }
    else if (CALIB_NO_LINE(sensorValues.value[LINESENSOR_LEFT])
    && CALIB_NO_LINE(sensorValues.value[LINESENSOR_MIDDLE_LEFT])
    && CALIB_NO_LINE(sensorValues.value[LINESENSOR_MIDDLE])
    && CALIB_NO_LINE(sensorValues.value[LINESENSOR_MIDDLE_RIGHT])
    && CALIB_NO_LINE(sensorValues.value[LINESENSOR_RIGHT]))
    {
        /* No line sensor is recognizing a line */
        if (LINE_LOST_MAX <= ++gLineLostCount)
        {
            gLineLostCount = 0U;
            retEvent = GUIDELINE_WAS_LOST;
        }
    }
    else
    {
        /* Everything is fine */
        gLineLostCount = 0U;
    }

    return retEvent;
}

/* INTERNAL FUNCTIONS *****************************************************************************/
