/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
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
#include "DriverHandler.h"
#include "SoftTimer.h"
#include "Buzzer.h"
#include "EventEnum.h"
#include "LineRecognition.h"
/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/

/** Start the motors and the internal timer for the maximum time (10s)
*
* @return nothing
*/
extern void DriveToStart_entry(void);

/** Follow the guide line to the start finish line
*
* @return DRIVE_TO_START_IS_ACTIVE_FOR_TOO_LONG : DriveToStart was active for more than 10 seconds
* @return START_FINISH_LINE_WAS_RECOGINZED : The start finish line has been recognized
* @return NO_EVENT_HAS_HAPPEND : No event was triggered in the state
*/
extern EventEnum DriveToStart_do(void);

/** Stop the internal timer 
*
* @return nothing
*/
extern EventEnum DriveToStart_exit(void);

/** Start the lapTimer for the lap time and emit a short beep
*
* @return Returns the lapTimer
*/
extern SoftTimer* DriveToStart_GoToDriveToFinish(void);

#endif /* DRIVETOSTART_H */
