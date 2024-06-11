/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       InitializeMcu.h
 *
 *    Module initializes the microcomputingunit and its components
 */
/**************************************************************************************************/
#ifndef INITIALIZEMCU_H
#define INITIALIZEMCU_H

/* INCLUDES ***************************************************************************************/
#include "app/EventEnum.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/

/** Initialize the microcontroller with all required variables and inits
* entry functio of the state
* @return INIZALIZATION_DONE : The initialization of the MCU is complete 
*/
extern EventEnum InitializeMcu_initializeAll(void);

/** Display the team name on the Oled display 
* exit functio of the state
*/
extern void InitializeMcu_displayTeamName(void);

#endif /* INITIALIZEMCU_H */