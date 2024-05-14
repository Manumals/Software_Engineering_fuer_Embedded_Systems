/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       StateHandler.c
 *
 *    Decides which states are called next depending on the returned events  
 */
/**************************************************************************************************/

/* INCLUDES ***************************************************************************************/
#include "StateHandler.h"

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

/* VARIABLES **************************************************************************************/
static EventEnum gCurrentState;
static SoftTimer* gLapTimer;

/* EXTERNAL FUNCTIONS *****************************************************************************/

void StateHandler_process(void)
{

}
/* INTERNAL FUNCTIONS *****************************************************************************/