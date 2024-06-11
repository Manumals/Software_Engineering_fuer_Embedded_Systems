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
#include "app/EventEnum.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/

/** Convert the event into an error code and output it via the ErrorHandler on the display and wait for the user to reset the microcontroller
* entry function of the state
* @param[in] errorReason The event that triggered the error
*/
extern void ErrorHandlerState_callErrorHandler(EventEnum errorReason);

#endif /* ERRORHANDLERSTATE_H */