/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       SetParameters.c
 *
 *    Module handles setting of parametersets and returns the currently selected parameter set on request
 */
/**************************************************************************************************/

/* INCLUDES ***************************************************************************************/
#include "SetParameters.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/
static UInt8 gParamSetIdx;           /** internal parametersetindex*/
static ParamSet gParamSetArray[];    /** array of parameterset, access via gParamSetIdx*/
static SoftTimer* gDisplayTimer;

/* EXTERNAL FUNCTIONS *****************************************************************************/

extern void SetParameters_entry(void)
{

}

extern void SetParameters_do(void)
{
  
}

extern void SetParameters_exit(void)
{
  
}

extern ParamSet SetParameters_getCurrentParamSet(void)
{

}

/* INTERNAL FUNCTIONS *****************************************************************************/

/** Switch to the next paramSet. If the last paramSet was active, activate the first paramSet
 * @return nothing
*/
static void nextParamSet(void);

/** Display the index of the current paramSet on the oled display
 * @return nothing
*/
static void displayParamSetIdx(void);