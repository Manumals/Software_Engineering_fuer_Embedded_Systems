/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       CalibrateLineSensors.h
 *
 * Calibrates the five line sensors so that the line is recognized under the current light conditions
 */
/**************************************************************************************************/
#ifndef CALIBRATELINESENSORS_H
#define CALIBRATELINESENSORS_H

/* INCLUDES ***************************************************************************************/
#include "app/EventEnum.h"
/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/

/** Initializes all variables that are needed for the calibration 
* entry function of the state
* @return NO_EVENT_HAS_HAPPEND: Initialization was successful
* @return CALIBRATION_FAILED : Initialization failed
*/
extern EventEnum CalibrateLineSensors_Initialize(void);

/** Calibrates the five line sensors so that the line is recognized under the current light conditions 
* do function of the state
* @return NO_EVENT_HAS_HAPPEND: Calibration is not completed yet
* @return CALIBRATION_FAILED : Calibration failed
* @return CALIBRATION_DONE : Calibration is complete
*/
extern EventEnum CalibrateLineSensors_CalibrateSensors(void);

#endif /*CALIBRATELINESENSORS_H*/