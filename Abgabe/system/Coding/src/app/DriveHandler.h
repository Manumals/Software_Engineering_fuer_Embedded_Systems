/***************************************************************************************************
  (c) Team 🏁~~ ō͡≡o\ (Maurice Ott, Simon Walderich, Thorben Päpke) 2024
***************************************************************************************************/
/**
 * @file       DriveHandler.h
 *
 *    Module takes care of all driving functions
 */
/**************************************************************************************************/
#ifndef DRIVEHANDLER_H
#define DRIVEHANDLER_H

/* INCLUDES ***************************************************************************************/
#include "service/LineSensor.h"

/* CONSTANTS **************************************************************************************/
#define MAX_LAP_TIME_MS (20000U) /** Timeout for driving one lap before an error is raised */

/* MACROS *****************************************************************************************/
/* The following is tristate: It is possible to detect a line, no line or neither */
#define CALIB_NO_LINE(value)   ((value) < 128) /**< Return if the given line sensor value is not recognizing a line */
#define CALIB_OVER_LINE(value) ((value) > 128) /**< Return if the given line sensor value is recognizing a line */

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/
/** Stop both driving motors
*/
extern void DriveHandler_stopDriving(void);

/** Travel straight ahead at the maximum specified speed to find the new guide line
*/
extern void DriveHandler_findGuideLine(void);

/** Follow the guide line according to the transferred line sensor values
*
* @param[in] sensorValues Array with the line sensor values
*/
extern void DriveHandler_followGuideLine(const LineSensorValues *sensorValues);

/** Drives a fast lap (I learnt writing comments from newtec)
* @param[in] time The current lap time
* @param[in] sensorValues Array with the line sensor values
*/
extern UInt8 DriveHandler_driveFastLap(UInt16 time, const LineSensorValues *sensorValues);

/**
 * Determine position of line under sensor.
 *
 * This function uses a weighted average to compute a logical line position. The value moves from
 * min to max if the line moves from left to right of the robot.
 * https://en.wikipedia.org/wiki/Weighted_arithmetic_mean
 *
 *     0 * sensor1 + 1000 * sensor1 + 2000 * sensor2 + ... + 4000 * sensor4
 *     ----------------------------------------------------------------
 *                      sum(sensor0, ... , sensor4)
 * Only the inner 3 sensors are used for the weighted average
 * If only one of the outer sensors recognizes a line, the min/max value is returned
 * This makes sense, because a lightly detected outer sensor would always result in a
 * position towards the middle, but it is impossible to determine if the line is only
 * lightly detected because the line is towards the middle or the outside
 *
 * @param sensorValues the current sensor values.
 * @return logical position between POS_MIN and POS_MAX.
 */
extern UInt16 DriveHandler_calculatePosition(const LineSensorValues *sensorValues);

#endif /* DRIVEHANDLER_H */
