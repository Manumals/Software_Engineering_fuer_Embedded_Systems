/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       DriveOverGap.h
 *
 *    Module performs the necessary maneuvers to drive over a gap
 */
/**************************************************************************************************/
#ifndef DRIVEOVERGAP_H
#define DRIVEOVERGAP_H

/* INCLUDES ***************************************************************************************/
#include "SoftTimer.h"
#include "EventEnum.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/

/** Saves the time of the lapTimer where the state is called
* entry function of the state
* @param[in] lapTimer The lap timer with the current lap time
*/
extern void DriveOverGap_SaveCurrentLapTime(SoftTimer* lapTimer);

/** Drive algorithm to drive over a gap, drive straight ahead at full throttle until the guide line is recognized again
* do function of the state
* @param[in] lapTimer The lap timer with the current lap time
*
* @return GUIDELINE_WAS_RECOGNIZED : The guide line has been found again
* @return LAPTIME_IS_TOO_LONG : LapTimer is greater than 20 seconds (error)
* @return DRIVE_OVER_GAP_IS_ACTIVE_FOR_TOO_LONG : State was active for more than 5 seconds (error)
* @return NO_EVENT_HAS_HAPPEND : No event was triggered in the state
*/
extern EventEnum DriveOverGap_DriveOverGap(SoftTimer* lapTimer);

#endif /* DRIVEOVERGAP_H */