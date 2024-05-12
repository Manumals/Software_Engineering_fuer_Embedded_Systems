/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       StateHandler.h
 *
 *    Decides which states are called next depending on the returned events  
 */
/**************************************************************************************************/
#ifndef STATEHANDLER_H
#define STATEHANDLER_H

/* INCLUDES ***************************************************************************************/
#include "EventEnum.h"
#include "InitializeMcu.h"
#include "CalibrateLineSensors.h"
#include "ReadyToDrive.h"
#include "DriveToStart.h"
#include "DriveToFinish.h"
#include "DriveOverGap.h"
#include "DisplayCountdown.h"
#include "DisplayLapTime.h"
#include "ErrorHandlerState.h"
#include "SetParameters.h"
/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/

/** Function that decides which state comes next
* 
* @return nothing
*/
extern void StateHandler_process(void);

#endif /* STATEHANDLER_H */