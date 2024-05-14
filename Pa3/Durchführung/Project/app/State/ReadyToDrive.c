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

/* VARIABLES **************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/

EventEnum ReadyToDrive_CheckStateOfButtons(void)
{
    EventEnum retEvent = NO_EVENT_HAS_HAPPEND;

    if (BUTTON_STATE_RELEASED == Button_getState(BUTTON_ID_A))
    {
        retEvent = START_BUTTON_HAS_BEEN_RELEASED;   
    }
    else if (BUTTON_STATE_RELEASED == Button_getState(BUTTON_ID_B))
    {
        retEvent = PARAM_BUTTON_HAS_BEEN_RELEASED;
    }
    else if (BUTTON_STATE_RELEASED == Button_getState(BUTTON_ID_C))
    {
        retEvent = CALIBRATION_BUTTON_HAS_BEEN_RELEASED;  
    }
    else 
    {
        retEvent = NO_EVENT_HAS_HAPPEND;
    }

    return retEvent;
}
/* INTERNAL FUNCTIONS *****************************************************************************/