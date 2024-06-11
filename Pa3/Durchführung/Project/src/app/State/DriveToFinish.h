/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       DriveToFinish.h
 *
 *    Module takes care of following the guide line 
 */
/**************************************************************************************************/
#ifndef DRIVETOFINSIH_H
#define DRIVETOFINSIH_H

/* INCLUDES ***************************************************************************************/
#include "os/SoftTimer.h"
#include "app/EventEnum.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/

/** Function handles following the guide line from start to finish
 * do function of the state
 * @param[in] lapTimer The lap timer with the current lap time
 *
 * @return START_FINISH_LINE_WAS_RECOGINZED : All five sensors recognize the guide line
 * @return GUIDELINE_WAS_LOST : The Guide line can no longer be recognized by any of the five senesors 
 * @return LAPTIME_IS_TOO_LONG : LapTimer is greater than 20 seconds (error)
 * @return NO_EVENT_HAS_HAPPEND : No event was triggered in the state
*/ 
extern EventEnum DriveToFinish_followGuideLine(SoftTimer* lapTimer);

#endif /* DRIVETOFINSIH_H */
