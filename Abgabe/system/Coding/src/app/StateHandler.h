/***************************************************************************************************
  (c) Team 🏁~~ ō͡≡o\ (Maurice Ott, Simon Walderich, Thorben Päpke) 2024
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
/** All the states of the state handler's state machine */
typedef enum 
{
  STATE_INIZALIZATION_MCU = 0, /**< Initializing the Mcu */
  STATE_CALIBRATE_LINE_SENSORS, /**< Calibrating the line sensors */
  STATE_READY_TO_DRIVE, /**< Ready to drive - waiting for user input */
  STATE_DISPLAY_COUNTDOWN, /**< Display the countdown before driving */
  STATE_DRIVE_TO_START, /**< Driving to the start line */
  STATE_DRIVE_TO_FINISH, /**< Driving a regular lap to the finish line */
  STATE_DRIVE_OVER_GAP, /**< Driving over a gap without line sensor input */
  STATE_DISPLAY_LAP_TIME, /**< Display the time it took to drive one lap */
  STATE_SET_PARAMETERS, /**< Switching through the parameter sets */
  STATE_ERROR_HANDLER /**< Handling an error */
} StateEnum;

/* PROTOTYPES *************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/
/** Function that decides which state comes next
*/
extern void StateHandler_process(void);

#endif /* STATEHANDLER_H */
