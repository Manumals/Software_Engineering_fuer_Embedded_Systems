/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       DriveHandler.h
 *
 *    Module takes care of all driving functions
 */
/**************************************************************************************************/
#ifndef DRIVEHANDLER_H
#define DRIVEHANDLER_H

/* INCLUDES ***************************************************************************************/
#include "service/LineSensor.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/

/** Stop both driving motors
*/
extern void DriveHandler_StopDriving(void);

/** Travel straight ahead at the maximum specified speed to find the new guide line
*/
extern void DriveHandler_FindGuideLine(void);

/** Follow the guide line according to the transferred line sensor values
*
* @param[in] sensorValues Array with the five line sensor values
*/
extern void DriveHandler_FollowGuideLine(const LineSensorValues *sensorValues);

#endif /* DRIVEHANDLER_H */