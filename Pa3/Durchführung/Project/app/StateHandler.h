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

#endif /* STATEHANDLER_H */