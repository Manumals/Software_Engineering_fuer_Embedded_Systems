/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       DisplayCountdown.h
 *
 *    Module handles displaying the countdown on the display 
 */
/**************************************************************************************************/
#ifndef DISPLAYCOUNTDOWN_H
#define DISPLAYCOUNTDOWN_H

/* INCLUDES ***************************************************************************************/
#include "app/EventEnum.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/

/** Start the timer required for the 3-second countdown
* entry function of the state
*/ 
extern void DisplayCountdown_startCountdown(void);

/** Check the timer and show the remaining time on the display
* do function of the state
* @return COUNTDOWN_IS_FINISHED : The 3 second countdown has reached zero 
* @return NO_EVENT_HAS_HAPPEND : No event was triggered in the state
*/ 
extern EventEnum DisplayCountdown_displayCountdown(void);

/** Stop the countdown timer
* exit function of the state
*/
extern void DisplayCountdown_stopCountdown(void);

#endif /* DISPLAYCOUNTDOWN_H */
