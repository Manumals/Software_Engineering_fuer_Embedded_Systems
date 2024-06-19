/***************************************************************************************************
  (c) Team 🏁~~ ō͡≡o\ (Maurice Ott, Simon Walderich, Thorben Päpke) 2024
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
  NO_EVENT_HAS_HAPPEND = 0, /**< No event has happened */
  /* Normal events */
  INIZALIZATION_DONE, /**< The initialization is done */
  CALIBRATION_DONE, /**< The calibration of the line sensors is done */
  COUNTDOWN_IS_FINISHED, /**< The countdown before starting to drive is finished */
  POWER_TO_THE_MOTORS_HAS_BEEN_STOPPED, /**< The power to the driving motors was stopped */
  START_BUTTON_HAS_BEEN_RELEASED, /**< The user pushed and released the the start button */
  CALIBRATION_BUTTON_HAS_BEEN_RELEASED, /**< The user pushed and released the the calibration button */
  PARAM_BUTTON_HAS_BEEN_RELEASED, /**< The user pushed and released the the parameter set button */
  START_FINISH_LINE_WAS_RECOGINZED, /**< The start/finish line was recognized by the line sensors */
  GUIDELINE_WAS_RECOGNIZED, /**< The guide line was recognized after it was lost before */
  CALIBRATION_FAILED, /**< There was an error during the calibration of the line sensors */
  INIZALIZATION_FAILED, /**< There was an error during the initialization */
  GUIDELINE_WAS_LOST, /**< The guide line was lost because no line sensor recognized a line over a short time */
  /* Errors */
  LAPTIMER_INIT_FAILED, /**< There was an error during the initialization of the lap timer */
  LAPTIME_IS_TOO_LONG, /**< Driving one lap did take too long */
  DRIVE_OVER_GAP_IS_ACTIVE_FOR_TOO_LONG, /**< During a lap the guide line was lost for too long */
  DRIVE_TO_START_IS_ACTIVE_FOR_TOO_LONG /**< Driving to the start the start line was not recognized for too long */
} EventEnum;

/* PROTOTYPES *************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/

#endif /* EVENTENUM_H */
