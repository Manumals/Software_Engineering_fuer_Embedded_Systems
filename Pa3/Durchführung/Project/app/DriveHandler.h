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
* @param[in] sensorValues Indicates the line sensors from left (bit 0) to right (bit 4) in the first 5 bits. 1 means line detected 0 no line detected 
*/
extern void DriveHandler_FollowGuideLine(UInt8 sensorValues);

#endif /* DRIVEHANDLER_H */