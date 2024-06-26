/***************************************************************************************************
  (c) Team 🏁~~ ō͡≡o\ (Maurice Ott, Simon Walderich, Thorben Päpke) 2024
***************************************************************************************************/
/**
 * @file       InitializeMcu.c
 *
 *    Module initializes the microcomputingunit and its components
 */
/**************************************************************************************************/

/* INCLUDES ***************************************************************************************/
#include "InitializeMcu.h"

#include "app/State/SetParameters.h"
#include "app/TeamName.h"
#include "hal/Hal.h"
#include "os/Os.h"
#include "service/Button.h"
#include "service/Display.h"
#include "service/Service.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/
EventEnum InitializeMcu_initializeAll(void)
{
    /* The following lines are already done in main.c */
    //Hal_init();     /* Initialize HAL modules */
    //OS_init();      /* Initialize OS modules */
    //Service_init(); /* Initialize Service modules */

    /* Deinit unused modules that were already initilized to save ressources */
    Button_deinit();
    LineSensor_deinit();

    SetParameters_displayParamSet();

    return INIZALIZATION_DONE;
} 

void InitializeMcu_displayTeamName(void)
{
    Display_gotoxy(0, 0);
    Display_write("Team ");
    /* The team name uses unicode chars, therefore the normal write won't work */
    Display_writeData(teamName, sizeof(teamName));
}

/* INTERNAL FUNCTIONS *****************************************************************************/
