/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       StateHandler.h
 *
 *    Decides which states are called next depending on the returned events  
 */
/**************************************************************************************************/
#ifndef STATEHANDLER_H
#define STATEHANDLER_H

/* INCLUDES ***************************************************************************************/

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/
typedef enum 
{
  STATE_INIZALIZATION_MCU = 0
  STATE_CALIBRATE_LINE_SENSORS,
  STATE_READY_TO_DRIVE,
  STATE_DISPLAY_COUNTDOWN,
  STATE_DRIVE_TO_START,
  STATE_DRIVE_TO_FINISH,
  STATE_DRIVE_OVER_GAP,
  STATE_DISPLAY_LAP_TIME,
  STATE_SET_PARAMETERS,
  STATE_ERROR_HANDLER
} StateEnum;
/* PROTOTYPES *************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/

/** Function that decides which state comes next
*/
extern void StateHandler_process(void);

/* INTERNAL FUNCTIONS *****************************************************************************/
/** The next state is decided on the basis of the transferred parameter 
 * @param[in] gCurrentEvent Event that occurred in Drive to Start
*/
static void DriveToStartEvent(EventEnum gCurrentEvent);

/** The next state is decided on the basis of the transferred parameter 
 * @param[in] gCurrentEvent Event that occurred in Drive to Finish
*/
static void DriveToFinishEvent(EventEnum gCurrentEvent);

/** The next state is decided on the basis of the transferred parameter 
 * @param[in] gCurrentEvent Event that occurred in Drive Over Gap
*/
static void DriveOverGapEvent(EventEnum gCurrentEvent);

/** The next state is decided on the basis of the transferred parameter 
 * @param[in] gCurrentEvent Event that occurred in Count Down Event
*/
static void DisplayCountDownEvent(EventEnum gCurrentEvent);

/** The next state is decided on the basis of the transferred parameter 
 * @param[in] gCurrentEvent Event that occurred in Ready to Drive
*/
static void ReadyToDriveEvent(EventEnum gCurrentEvent);

/** The next state is decided on the basis of the transferred parameter 
 * @param[in] gCurrentEvent Event that occurred in Calibrate Line Sensors
*/
static void CalibrateLineSensorsEvent(EventEnum gCurrentEvent);

#endif /* STATEHANDLER_H */