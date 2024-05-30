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

#include "service/Display.h"
#include "os/SoftTimer.h"
#include "app/EventEnum.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/
/** Switch to the next paramSet. If the last paramSet was active, activate the first paramSet
 * @return nothing
*/
static void nextParamSet(void);

/** Display the index of the current paramSet on the oled display
 * @return nothing
*/
static void displayParamSetIdx(void);

/* VARIABLES **************************************************************************************/
static UInt8 gParamSetIdx;           /** internal parametersetindex*/
static ParamSet gParamSetArray[3];    /** array of parameterset, access via gParamSetIdx*/

/* EXTERNAL FUNCTIONS *****************************************************************************/

void SetParameters_SetNextParamSet(void)
{

}

void SetParameters_DisplayParamSet(void)
{
  
}

ParamSet SetParameters_getCurrentParamSet(void)
{

}

/* INTERNAL FUNCTIONS *****************************************************************************/

static void nextParamSet(void)
{

}

static void displayParamSetIdx(void)
{

}