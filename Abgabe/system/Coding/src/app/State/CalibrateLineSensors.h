/***************************************************************************************************
  (c) Team 🏁~~ ō͡≡o\ (Maurice Ott, Simon Walderich, Thorben Päpke) 2024
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
*/
extern void CalibrateLineSensors_initialize(void);

/** Calibrates the five line sensors so that the line is recognized under the current light conditions 
* do function of the state
* @return NO_EVENT_HAS_HAPPEND: Calibration is not completed yet
* @return CALIBRATION_FAILED : Calibration failed
* @return CALIBRATION_DONE : Calibration is complete
*/
extern EventEnum CalibrateLineSensors_calibrateSensors(void);

/** Deinitializes variables that are needed for the calibration 
* exit function of the state
*/
extern void CalibrateLineSensors_deinitialize(void);

#endif /*CALIBRATELINESENSORS_H*/
