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
#include "DriveControl.h"
#include "LineSensor.h"
#include "EventEnum.h"
/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/

/** Calibrates the five line sensors so that the line is recognized under the current light conditions 
* 
* @return CALIBRATION_DONE : Calibration is complete
*/
extern EventEnum CalibrateLineSensors_entry(void);

#endif /*CALIBRATELINESENSORS_H*/