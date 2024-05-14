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

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/
typedef struct tag_ParamSet
{
  UInt8 maxSpeed;
  UInt8 minCurveRadius;
} ParamSet;

/* PROTOTYPES *************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/

/** Cycle to the next parameter set
* entry function of the state
*/
extern void SetParameters_SetNextParamSet(void);

/** Display the current parameter set
* exit function of the state
*/
extern void SetParameters_DisplayParamSet(void);

/** Returns the current parameter set
*
* @return Current parameter set
*/
extern ParamSet SetParameters_getCurrentParamSet(void);

#endif /* SETPARAMETERS_H */
