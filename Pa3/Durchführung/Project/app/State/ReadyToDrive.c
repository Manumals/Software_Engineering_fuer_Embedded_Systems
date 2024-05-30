/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       ReadyToDrive.c
 *
 *    Robot is ready to drive and is waiting for user input
 */
/**************************************************************************************************/

/* INCLUDES ***************************************************************************************/
#include "ReadyToDrive.h"

#include "Display.h"
#include "Button.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

static bool WasButtonPressed(ButtonID);

/* VARIABLES **************************************************************************************/

static ButtonState lastState = BUTTON_STATE_UNKNOWN;

/* EXTERNAL FUNCTIONS *****************************************************************************/

EventEnum ReadyToDrive_CheckStateOfButtons(void)
{
    EventEnum retEvent = NO_EVENT_HAS_HAPPEND;

    if (TRUE == WasButtonPressed(BUTTON_ID_A))
    {
        retEvent = START_BUTTON_HAS_BEEN_RELEASED;
    }
    else if (TRUE == WasButtonPressed(BUTTON_ID_B))
    {
        retEvent = PARAM_BUTTON_HAS_BEEN_RELEASED;
    }
    else if (TRUE == WasButtonPressed(BUTTON_ID_C))
    {
        retEvent = CALIBRATION_BUTTON_HAS_BEEN_RELEASED;  
    }
    
    return retEvent;
}
/* INTERNAL FUNCTIONS *****************************************************************************/

bool WasButtonPressed(ButtonID ID)
{
    bool retBool = FALSE;

    ButtonState activeState = Button_getState(ID);
    
    if (BUTTON_STATE_PRESSED == lastState && BUTTON_STATE_RELEASED == activeState)
    {
        retBool = TRUE;
    }
    lastState = activeState;

    return retBool;
}