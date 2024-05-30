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
#define OFFSET_FOR_CHAR 48
/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/
static UInt8 gParamSetIdx = 0;           /**< internal parametersetindex*/
static ParamSet gParamSetArray[3] = {{255, 5, 5, 5 ,5}, 
{150, 3, 3, 3, 3}, {75, 1, 1, 1, 1}};    /**< array of parameterset, access via gParamSetIdx*/

/* EXTERNAL FUNCTIONS *****************************************************************************/

void SetParameters_SetNextParamSet(void)
{
    gParamSetIdx++;
  
    if (3 >= gParamSetIdx)
    {
        gParamSetIdx = 0;
    }
}

void SetParameters_DisplayParamSet(void)
{
    Display_gotoxy(0,3);
    Display_clearLine();
    char indexString[16] = "Current Index ";
    indexString[15] = OFFSET_FOR_CHAR + gParamSetIdx; /**< Add the current index as a number */
    Display_write(indexString, 16);
}

ParamSet SetParameters_getCurrentParamSet(void)
{
    return gParamSetArray[gParamSetIdx];
}