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
#include "EventEnum.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/

/** Convert the event into an error code and output it via the ErrorHandler on the display
* entry function of the state
* @param[in] errorReason The event that triggered the error
*/
extern void ErrorHandlerState_CallErrorHandler(EventEnum errorReason);

/** Wait for the user to reset the microcontroller
* do function of the state
*/
extern void ErrorHandlerState_WaitForReset(void);

#endif /* ERRORHANDLERSTATE_H */