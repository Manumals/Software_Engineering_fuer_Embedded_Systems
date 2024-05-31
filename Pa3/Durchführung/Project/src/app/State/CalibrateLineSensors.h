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
#define CALIB_NO_LINE(value) ((value) <= 500)
#define CALIB_OVER_LINE(value) ((value) > 500)

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/

/** Initializes all variables that are needed for the calibration 
* entry function of the state
*/
extern void CalibrateLineSensors_initialize(void);

/** Calibrates the five line sensors so that the line is recognized under the current light conditions 
* do function of the state
* @return NO_EVENT_HAS_HAPPEND: Calibration is not completed yet
* @return CALIBRATION_FAILED : Calibration failed
* @return CALIBRATION_DONE : Calibration is complete
*/
extern EventEnum CalibrateLineSensors_calibrateSensors(void);

#endif /*CALIBRATELINESENSORS_H*/