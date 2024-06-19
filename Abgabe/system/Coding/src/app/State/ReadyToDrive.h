/***************************************************************************************************
  (c) Team 🏁~~ ō͡≡o\ (Maurice Ott, Simon Walderich, Thorben Päpke) 2024
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
/** Activate the buttons
* entry function of the state
*/
extern void ReadyToDrive_activateButtons(void);

/** Wait for input from the user
* do function of the state
* @return CALIBRATION_BUTTON_HAS_BEEN_RELEASED : The line sensors are calibrated again
* @return PARAM_BUTTON_HAS_BEEN_RELEASED : A new parameter set is selected
* @return START_BUTTON_HAS_BEEN_RELEASED : Start the countdown
* @return NO_EVENT_HAS_HAPPEND : No event was triggered in the state
*/
extern EventEnum ReadyToDrive_checkStateOfButtons(void);

/** Deactivate the buttons
* exit function of the state
*/
extern void ReadyToDrive_deactivateButtons(void);

#endif /* READYTODRIVE_H */
