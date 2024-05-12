/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       ErrorHandlerState.h
 *
 *    Shows an error message on the display depending on the event
 */
/**************************************************************************************************/
#ifndef ERRORHANDLERSTATE_H
#define ERRORHANDLERSTATE_H

/* INCLUDES ***************************************************************************************/
#include "DriveHandler.h"
#include "Buzzer.h"
#include "ErrorHandler.h"
#include "EventEnum.h"
/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/

/** Convert the event into an error code and output it via the ErrorHandler on the display
*
* @param[in] errorReason The event that triggered the error
*
* @return nothing
*/
extern void ErrorHandlerState_entry(EventEnum errorReason);

/** Wait for the user to reset the microcontroller
*
* @return nothing
*/
extern void ErrorHandlerState_do(void);

#endif /* ERRORHANDLERSTATE_H */