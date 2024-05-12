/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       SetParameters.h
 *
 *    Module handles setting of parametersets and returns the currently selected parameter set on request
 */
/**************************************************************************************************/
#ifndef SETPARAMETERS_H
#define SETPARAMETERS_H

/* INCLUDES ***************************************************************************************/
#include "Display.h"
#include "SoftTimer.h"
#include "EventEnum.h"
/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/

/** Cycle to the next parameter set and start the internal timer
*
* @return nothing
*/
extern void SetParameters_entry(void);

/** Display the current parameter set
*
* @return nothing
*/
extern void SetParameters_do(void);

/** Stop the timer and clear the display
*
* @return nothing
*/
extern void SetParameters_exit(void);

/** Returns the current parameter set
*
* @return Current parameter set
*/
extern ParamSet SetParameters_getCurrentParamSet(void);

#endif /* SETPARAMETERS_H */
