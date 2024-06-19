/***************************************************************************************************
  (c) Team 🏁~~ ō͡≡o\ (Maurice Ott, Simon Walderich, Thorben Päpke) 2024
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
#include "Common/Types.h"

/* CONSTANTS **************************************************************************************/
#define PARAM_SETS_COUNT (13) /**< Amount of elements of the parameter set array */

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/
/** Struct to hold all data of a parameter set */
typedef struct tag_ParamSet
{
    UInt8 maxMotorSpeed; /**< Maximum motor speed used, maximum 100 allowed */
    UInt8 pidP;          /**< Proportional factor of PID controller, maximum 63 allowed */
    UInt8 pidD;          /**< Derivative factor of PID controller */
} ParamSet;

/* PROTOTYPES *************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/
/** Cycle to the next parameter set
* entry function of the state
*/
extern void SetParameters_setNextParamSet(void);

/** Display the current parameter set
* exit function of the state
*/
extern void SetParameters_displayParamSet(void);

/** Returns the current parameter set
*
* @return Current parameter set
*/
extern ParamSet SetParameters_getCurrentParamSet(void);

/** Set the current parameter set
*
* @param[paramSet] New current parameter set
*/
extern void SetParameters_setCurrentParamSet(ParamSet paramSet);

/** Returns the current parameter set index
*
* @return Current parameter set index
*/
UInt8 SetParameters_getCurrentParamSetIdx(void);

#endif /* SETPARAMETERS_H */
