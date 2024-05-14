/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       StateHandler.c
 *
 *    Decides which states are called next depending on the returned events  
 */
/**************************************************************************************************/

/* INCLUDES ***************************************************************************************/

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/
#include "app/StateHandler.h"

#include "app/EventEnum.h"
#include "app/State/InitializeMcu.h"
#include "app/State/CalibrateLineSensors.h"
#include "app/State/eadyToDrive.h"
#include "app/State/DriveToStart.h"
#include "app/State/DriveToFinish.h"
#include "app/State/DriveOverGap.h"
#include "app/State/DisplayCountdown.h"
#include "app/State/DisplayLapTime.h"
#include "app/State/ErrorHandlerState.h"
#include "app/State/SetParameters.h"
/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/
static EventEnum gCurrentEvent = NO_EVENT_HAS_HAPPEND; 
static StateEnum gCurrentState = STATE_INIZALIZATION_MCU; //First state when the MCU is started
static SoftTimer* gLapTimer;
static bool gEntryDone = false;

/* EXTERNAL FUNCTIONS *****************************************************************************/

extern void StateHandler_process(void)
{
  switch(gCurrentState)
  { 
    case STATE_INIZALIZATION_MCU: 
      InitializeMcu_InitializeAll(void); //entry
      InitializeMcu_DisplayTeamName(void); //exit
      gCurrentState = STATE_CALIBRATE_LINE_SENSORS;
      break;
    case STATE_CALIBRATE_LINE_SENSORS: 
      CalibrateLineSensors_CalibrateSensors(void); //do
      gCurrentState = STATE_READY_TO_DRIVE;
      break;
    case STATE_READY_TO_DRIVE:         
      gCurrentEvent = ReadyToDrive_CheckStateOfButtons(void); //do
      switch(gCurrentEvent)
      {
        case CALIBRATION_BUTTON_HAS_BEEN_RELEASED:
          gCurrentState = STATE_CALIBRATE_LINE_SENSORS;
          break;
        case START_BUTTON_HAS_BEEN_RELEASED:
          gCurrentState = STATE_DISPLAY_COUNTDOWN;
          break;
        case PARAM_BUTTON_HAS_BEEN_RELEASED:
          gCurrentState = STATE_SET_PARAMETERS;
          break;
        default:
        break; 
      }
      break;
    case STATE_CALIBRATE_LINE_SENSORS:
      SetParameters_SetNextParamSet(void); //entry
      SetParameters_DisplayParamSet(void); //exit
      gCurrentState = STATE_READY_TO_DRIVE;
      break;
    case STATE_DISPLAY_COUNTDOWN:      
      if(gEntryDone == false)
      {
        gEntryDone = true;
        DisplayCountdown_StartCountdown(void); //entry
      }
      gCurrentEvent = DisplayCountdown_DisplayCountdown(void); //do

      if(gCurrentEvent == COUNTDOWN_IS_FINISHED)
      {
        gEntryDone = false;
        DisplayCountdown_StopCountdown(void); //exit
        gCurrentState = STATE_DRIVE_TO_START;
      }
      break;
    case STATE_DRIVE_TO_START:        
      if(gEntryDone == false)
      {
        gEntryDone = true;
        DriveToStart_StartMotorsAndTimer(void); //entry
      }
      gCurrentEvent = DriveToStart_FollowGuideLine(void); //do

      if(gCurrentEvent == DRIVE_TO_START_IS_ACTIVE_FOR_TOO_LONG)
      {
        gCurrentState = STATE_ERROR_HANDLER;
        gEntryDone = false;
        DriveToStart_StopTimer(void); //exit
      }else if(gCurrentEvent == START_FINISH_LINE_WAS_RECOGINZED)
      {
        gCurrentState = STATE_DRIVE_TO_FINISH;
        gEntryDone = false;
        DriveToStart_StopTimer(void); //exit
        gLapTimer = DriveToStart_StartTimerAndBeep(void); //State drive to finish next
      }
      break;
    case STATE_DRIVE_TO_FINISH: 
      gCurrentEvent = DriveToFinish_FollowGuideLine(gLapTimer); //do
      switch(gCurrentEvent)
      {
        case GUIDELINE_WAS_LOST:
          gCurrentState = STATE_DRIVE_OVER_GAP;
        break;
        case LAPTIME_IS_TOO_LONG:
          gCurrentState = STATE_ERROR_HANDLER;
        break;
        case START_FINISH_LINE_WAS_RECOGINZED:
          gCurrentState = STATE_DISPLAY_LAP_TIME;
        break;
        default:
        break;
      }
      break;
    case STATE_ERROR_HANDLER:
      if(gEntryDone == false)
      {
        gEntryDone = true;
        ErrorHandlerState_CallErrorHandler(gCurrentEvent); //entry
      }
      ErrorHandlerState_WaitForReset(void); //do
      break;
    case STATE_DRIVE_OVER_GAP:
      if(gEntryDone == false)
      {
        gEntryDone = true;
        DriveOverGap_SaveCurrentLapTime(gLapTimer); //entry
      }
      gCurrentEvent = DriveOverGap_DriveOverGap(gLapTimer); //do

      if(gCurrentEvent == THE_GUIDELINE_WAS_RECOGNIZED)
      {
        gCurrentState = STATE_DRIVE_TO_FINISH;
        gEntryDone = false;
      }else if(gCurrentEvent == DRIVE_OVER_GAP_IS_ACTIVE_FOR_TOO_LONG)
      {
        gCurrentState = STATE_ERROR_HANDLER;
        gEntryDone = false;
      }
      break;
    case STATE_DISPLAY_LAP_TIME:
        gCurrentEvent = DisplayLapTime_StopAfterLap(void); //entry
        if(gCurrentEvent == POWER_TO_THE_MOTORS_HAS_BEEN_STOPPED)
        {
          DisplayLapTime_DisplayLapTime(void); //exit
        }
      break;
    default:
      //Shit happens
    break;
  }
}
/* INTERNAL FUNCTIONS *****************************************************************************/