/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       EventEnum.h
 *
 *    All possible events that can occur in the states 
 */
/**************************************************************************************************/
#ifndef EVENTENUM_H
#define EVENTENUM_H

/* INCLUDES ***************************************************************************************/

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/
/** Enum for all events that are needed for the normal function of the state handler
*/
typedef enum
{
  NO_EVENT_HAS_HAPPEND = 0,
  /* Normal events */
  INIZALIZATION_DONE,
  CALIBRATION_DONE,
  COUNTDOWN_IS_FINISHED,
  POWER_TO_THE_MOTORS_HAS_BEEN_STOPPED,
  START_BUTTON_HAS_BEEN_RELEASED,
  CALIBRATION_BUTTON_HAS_BEEN_RELEASED,
  PARAM_BUTTON_HAS_BEEN_RELEASED,
  START_FINISH_LINE_WAS_RECOGINZED,
  THE_GUIDELINE_WAS_RECOGNIZED,
  /* Errors */
  CALIBRATION_FAILED,
  INIZALIZATION_FAILED,
  GUIDELINE_WAS_LOST,
  LAPTIME_IS_TOO_LONG,
  DRIVE_OVER_GAP_IS_ACTIVE_FOR_TOO_LONG,
  DRIVE_TO_START_IS_ACTIVE_FOR_TOO_LONG
} EventEnum;

/* PROTOTYPES *************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/


#endif /* EVENTENUM_H */