/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       ReadyToDrive.h
 *
 *    Robot is ready to drive and is waiting for user input
 */
/**************************************************************************************************/
#ifndef READYTODRIVE_H
#define READYTODRIVE_H

/* INCLUDES ***************************************************************************************/
#include "app/EventEnum.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/

/** Wait for input from the user
* do function of the state
* @return CALIBRATION_BUTTON_HAS_BEEN_RELEASED : The line sensors are calibrated again
* @return PARAM_BUTTON_HAS_BEEN_RELEASED : A new parameter set is selected
* @return START_BUTTON_HAS_BEEN_RELEASED : Start the countdown
* @return NO_EVENT_HAS_HAPPEND : No event was triggered in the state
*/
extern EventEnum ReadyToDrive_CheckStateOfButtons(void);

#endif /* READYTODRIVE_H */