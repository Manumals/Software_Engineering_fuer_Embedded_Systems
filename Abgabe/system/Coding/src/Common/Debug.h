/***************************************************************************************************
  (c) Team 🏁~~ ō͡≡o\ (Maurice Ott, Simon Walderich, Thorben Päpke) 2024
***************************************************************************************************/
/**
 * @file       Debug.h
 *
 *    Module takes care of all debug functions
 */
/**************************************************************************************************/
#ifndef DEBUG_H
#define DEBUG_H

/* INCLUDES ***************************************************************************************/
#include "Common/Types.h"
#include "service/LineSensor.h"

/* CONSTANTS **************************************************************************************/
/* Debug functionality
 * undefine all to guarantee normal and specified behaviour
 * e.g. printing to the display takes time and affects driving
 * and many functions are just against specification but allow (easier) debugging
 */

  //#define BUZZER_VOLUME_LOW /**< Lower buzzer volume to a not so annoying level */
//#define COUNT_GAPS /**< Count detected gaps of one round and show an error if it was the incorrect amount */
  //#define FAST_GAP_ERROR /**< Detect a gap error very fast to avoid running far off track */
  #define FAST_LAP /**< Drive only fast laps, ignoring the guide line, if the last paramSet is active */
//#define GET_LAP_DATA /**< Drive multiple laps, adjust paramSet and display average lap times on display */
//#define MIN_SPEED /**< Use a minimum motor speed so that they always move if the set speed > 0 */
  //#define NO_CALIB /**< Skip calibration and use previously determined calibration values */
  //#define NO_COUNTDOWN /**< Skip the countdown to save the 3s of time */
//#define NO_READY_TO_DRIVE /**< Skip the state ReadyToDrive, so driving begins without waiting for user pushing the start button */
  #define PID_D_INACTIVE /**< Deactivate D part of PID to save unnecessary complexity and ~5% cycle time */
//#define SHOW_CALIB_DATA_LIVE /**< Show calibration data on display during the calibration process */
//#define SHOW_CALIB_DATA_RES /**< Show calibration data on display after calibration process and freeze afterwards */
//#define SHOW_CALIB_LINE_RECOGNITION /**< Show for every line sensor if they recognize a line while calibrating when moving back to central line */
//#define SHOW_CALIB_RAW_VALS /** Show raw values of line sensors when read */
//#define SHOW_CALIB_STATES /**< Show debug messages when changing calibration states */
//#define SHOW_CYCLE_TIMES /**< Show average cycle times directly from the main task worker */
//#define SHOW_DRIVE_CYCLE_TIMES /**< Show current cycle time while driving, affects PID, only works with SHOW_DRIVE_CYCLE_TIMES_MINMAX */
//#define SHOW_DRIVE_CYCLE_TIMES_MINMAX /**< Show min and max cycle time while driving, affects PID */
//#define SHOW_GAP_MSGS /**< Show debug messages during DriveOverLine */
//#define SHOW_POSITION /**< Show calculated position as numbers after calibration, other functionality is deactivated */
//#define SHOW_POSITION_BAR /**< Show calculated position as bars after calibration, other functionality is deactivated */
//#define TEST_SPEEDS /**< Test motor speeds after initialization  */
//#define SHOW_START_LINE_RECOGNITION /**< Show for every line sensor if they recognize a line when moving to the start line */
//#define SHOW_START_MSGS /**< Show debug messages during DriveToStart */

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/
/** Show debug message on display
* @param[in] line The text to display.
*/
extern void Debug_showMsg(const char * msg);

/** Show debug message with number on display
* @param[in] num The number to display.
* @param[in] line The text to display.
*/
extern void Debug_showMsgNum(const UInt16 num, const char * line);

/** Show calibration data on display
*/
void Debug_showCalibData();

/** Show debug message with line recognition results on display
* @param[in] sensorValues The line sensor values
*/
void Debug_showLineRecognition(LineSensorValues *sensorValues);

/** Show calculated positions as numbers on display
*/
void Debug_showPosition();

/** Show calculated positions on display with bars
* This is very easy to read but the actual numbers can't be determined
*/
void Debug_showPositionBar();

/** Show line sensor values on display
*/
void Debug_showLineValues();

/** Show two numbers, a min and max value on display
* @param[in] min The minimum value to show
* @param[in] max The minimum value to show
*/
void Debug_showMinMax(const UInt16 min, const UInt16 max);

/** Start to measure the time, needed for Debug_showTimeStop
*/
void Debug_showTimeStart();

/** Show passed time since Debug_showTimeStart was called and an additional message on the display
* Multiple calls are possible, then Debug_showTimeStart is only needed to be called once at first
* @param[in] line The text to display.
*/
void Debug_showTimeStop(const char * msg);

#endif /* DEBUG_H */
