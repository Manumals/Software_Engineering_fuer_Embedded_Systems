/***************************************************************************************************
  (c) Team 🏁~~ ō͡≡o\ (Maurice Ott, Simon Walderich, Thorben Päpke) 2024
***************************************************************************************************/
/**
 * @file       ReadyToDrive.c
 *
 *    Robot is ready to drive and is waiting for user input
 */
/**************************************************************************************************/

/* INCLUDES ***************************************************************************************/
#include "ReadyToDrive.h"

#include "Common/Debug.h"
#include "service/Button.h"
#include "service/Display.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/
void ReadyToDrive_activateButtons(void)
{
    Button_init();
}

EventEnum ReadyToDrive_checkStateOfButtons(void)
{
    EventEnum retEvent = NO_EVENT_HAS_HAPPEND;

    /* Convert the application errors to those of the error handler */
    if (BUTTON_STATE_TRIGGERED == Button_getState(BUTTON_ID_A))
    {
        retEvent = START_BUTTON_HAS_BEEN_RELEASED;
    }
    else if (BUTTON_STATE_TRIGGERED == Button_getState(BUTTON_ID_B))
    {
        retEvent = PARAM_BUTTON_HAS_BEEN_RELEASED;
    }
    else if (BUTTON_STATE_TRIGGERED == Button_getState(BUTTON_ID_C))
    {
        retEvent = CALIBRATION_BUTTON_HAS_BEEN_RELEASED;  
    }
    
    return retEvent;
}

void ReadyToDrive_deactivateButtons(void)
{
    /* Deinitialize the buttons because they are not needed anymore */
    Button_deinit();
}

/* INTERNAL FUNCTIONS *****************************************************************************/
