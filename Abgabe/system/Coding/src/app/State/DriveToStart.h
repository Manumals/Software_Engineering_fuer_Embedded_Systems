/***************************************************************************************************
  (c) Team 🏁~~ ō͡≡o\ (Maurice Ott, Simon Walderich, Thorben Päpke) 2024
***************************************************************************************************/
/**
 * @file       DriveToStart.h
 *
 *    Module handles driving to the startline 
 */
/**************************************************************************************************/
#ifndef DRIVETOSTART_H
#define DRIVETOSTART_H

/* INCLUDES ***************************************************************************************/
#include "app/EventEnum.h"
#include "os/SoftTimer.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/

/** Start the motors and the internal timer for the maximum time (10s)
* entry function of the state
*/
extern void DriveToStart_startMotorsAndTimer(void);

/** Follow the guide line to the start finish line
* do function of the state
* @return DRIVE_TO_START_IS_ACTIVE_FOR_TOO_LONG : DriveToStart was active for more than 10 seconds
* @return START_FINISH_LINE_WAS_RECOGINZED : The start finish line has been recognized
* @return NO_EVENT_HAS_HAPPEND : No event was triggered in the state
*/
extern EventEnum DriveToStart_followGuideLine(void);

/** Stop the timer
* exit function of the state
*/
extern void DriveToStart_stopTimer(void);

/** Start the lapTimer for the lap time and emit a short beep
* @return Returns the lapTimer
*/
extern SoftTimer* DriveToStart_startTimerAndBeep(void);

#endif /* DRIVETOSTART_H */
