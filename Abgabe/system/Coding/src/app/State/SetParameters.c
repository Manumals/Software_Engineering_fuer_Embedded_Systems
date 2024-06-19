/***************************************************************************************************
  (c) Team 🏁~~ ō͡≡o\ (Maurice Ott, Simon Walderich, Thorben Päpke) 2024
***************************************************************************************************/
/**
 * @file       SetParameters.c
 *
 *    Module handles setting of parametersets and returns the currently selected parameter set on request
 */
/**************************************************************************************************/

/* INCLUDES ***************************************************************************************/
#include "SetParameters.h"

#include <stdio.h>
#include "os/SoftTimer.h"
#include "service/Display.h"

/* CONSTANTS **************************************************************************************/
#define DISP_TXT_LEN_MAX ((DISPLAY_MAX_LENGTH) + 1U) /**< Maximum text length for one line on the display */

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/
static UInt8 gParamSetIdx = 0U; /**< Index of the current parameter set */

/** Array to store different parameter sets */
static ParamSet gParamSetArray[PARAM_SETS_COUNT] = {
    { 60, 20, 0},
    {100, 10, 0},
    {100, 15, 0},
    {100, 20, 0},
    {100, 25, 0},
    {100, 30, 0},
    {100, 35, 0},
    {100, 40, 0},
    {100, 45, 0},
    {100, 50, 0},
    {100, 55, 0},
    {100, 60, 0},
    {100, 20, 0}
};

/* EXTERNAL FUNCTIONS *****************************************************************************/
void SetParameters_setNextParamSet(void)
{
    gParamSetIdx++;
  
    if (PARAM_SETS_COUNT <= gParamSetIdx)
    {
        gParamSetIdx = 0;
    }
}

void SetParameters_displayParamSet(void)
{
    char txt[DISP_TXT_LEN_MAX] = {0};
    UInt8 length = sprintf(txt, "ParamSet: %2u ", gParamSetIdx+1U);
    Display_gotoxy(0, 3);
    Display_writeWithLength(txt, length);
}

ParamSet SetParameters_getCurrentParamSet(void)
{
    return gParamSetArray[gParamSetIdx];
}

void SetParameters_setCurrentParamSet(ParamSet paramSet)
{
    gParamSetArray[gParamSetIdx] = paramSet;
}

UInt8 SetParameters_getCurrentParamSetIdx(void)
{
    return gParamSetIdx;
}

/* INTERNAL FUNCTIONS *****************************************************************************/
