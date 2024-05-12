/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       DisplayCountdown.h
 *
 *    Module handles displaying the countdown on the OLED-display 
 */
/**************************************************************************************************/
#ifndef DISPLAYCOUNTDOWN_H
#define DISPLAYCOUNTDOWN_H

/* INCLUDES ***************************************************************************************/
#include "SoftTimer.h"
#include "Display.h"
#include "EventEnum.h"
/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/

/** Start the timer required for the 3-second countdown
* 
* @return nothing
*/ 
extern void DisplayCountdown_entry(void);

/** Check the timer and show the remaining time on the display
* 
* @return COUNTDOWN_IS_FINISHED : The 3 second countdown has reached zero 
* @return NO_EVENT_HAS_HAPPEND : No event was triggered in the state
*/ 
extern EventEnum DisplayCountdown_do(void);

/** Stop the countdown timer
*
* @return nothing
*/
extern void DisplayCountdown_exit(void);

#endif /* DISPLAYCOUNTDOWN_H */
