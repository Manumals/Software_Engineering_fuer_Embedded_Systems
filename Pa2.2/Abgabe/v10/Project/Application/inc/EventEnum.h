/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       EventsEnum.h
 *
 *    All possible events that can occur in the states 
 */
/**************************************************************************************************/
#ifndef EVENTSENUM_H
#define EVENTSENUM_H

/* INCLUDES ***************************************************************************************/

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/
enum EventEnum
{
        INIZALIZATION_DONE
        CALIBRATION_DONE
        COUNTDOWN_IS_FINISHED
        POWER_TO_THE_MOTORS_HAS_BEEN_STOPPED
        START_BUTTON_HAS_BEEN_RELEASED
        CALIBRATION_BUTTON_HAS_BEEN_RELEASED
        PARAM_BUTTON_HAS_BEEN_RELEASED
        START_FINISH_LINE_WAS_RECOGINZED
        THE_GUIDELINE_WAS_RECOGNIZED
        GUIDELINE_WAS_LOST
        LAPTIME_IS_TOO_LONG
        DRIVE_OVER_GAP_IS_ACTIVE_FOR_TOO_LONG
        DRIVE_TO_START_IS_ACTIVE_FOR_TOO_LONG
        NO_EVENT_HAS_HAPPEND
};
/* PROTOTYPES *************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/


#endif /* EVENTSENUM_H */