/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       DisplayLapTime.h
 *
 *    Module handles displaying the time needed to complete one lap on the OLED display
 *    and stops the timer as well as the power to the motors 
 */
/**************************************************************************************************/
#ifndef DISPLAYLAPTIME_H
#define DISPLAYLAPTIME_H

/* INCLUDES ***************************************************************************************/
#include "os/SoftTimer.h"
#include "app/EventEnum.h"
/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/

/** Stop the lapTime timer, the power to the motors and emits a short beep
 * entry function of the state
 * @param[in] lapTimer The lap timer with the completed lap time
 * @return POWER_TO_THE_MOTORS_HAS_BEEN_STOPPED : The power to the motors was cut
*/
extern EventEnum DisplayLapTime_stopAfterLap(SoftTimer* lapTimer);

/** Display the laptime on the OLED-display
* exit function of the state
*/
extern void DisplayLapTime_displayLapTime(SoftTimer* lapTimer);

#endif /* DISPLAYLAPTIME_H */

