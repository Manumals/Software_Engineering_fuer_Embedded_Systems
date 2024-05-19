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
#include "StateHandler.h"
#include "EventEnum.h"

#include "app/State/InitializeMcu.h"
#include "app/State/CalibrateLineSensors.h"
#include "app/State/ReadyToDrive.h"
#include "app/State/DriveToStart.h"
#include "app/State/DriveToFinish.h"
#include "app/State/DriveOverGap.h"
#include "app/State/DisplayCountdown.h"
#include "app/State/DisplayLapTime.h"
#include "app/State/ErrorHandlerState.h"
#include "app/State/SetParameters.h"
/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/
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
/* VARIABLES **************************************************************************************/
static EventEnum gCurrentEvent = NO_EVENT_HAS_HAPPEND; 
static StateEnum gCurrentState = STATE_INIZALIZATION_MCU; /**< First state when the MCU is started */
static SoftTimer* gLapTimer;
static bool gEntryDone = false;

/* EXTERNAL FUNCTIONS *****************************************************************************/

void StateHandler_process(void)
{
    switch (gCurrentState)
    { 
    case STATE_INIZALIZATION_MCU: 
        InitializeMcu_InitializeAll(); /**< entry */
        InitializeMcu_DisplayTeamName(); /**< exit */
        gCurrentState = STATE_CALIBRATE_LINE_SENSORS;
        break;

    case STATE_CALIBRATE_LINE_SENSORS:
        if (false == gEntryDone)
        {
            gEntryDone = true;
            gCurrentEvent = CalibrateLineSensors_Initialize(); /**< entry */
        }
        gCurrentEvent = CalibrateLineSensors_CalibrateSensors();
        CalibrateLineSensorsEvent(gCurrentEvent); /**< do */
        break;

    case STATE_READY_TO_DRIVE:
        gCurrentEvent = ReadyToDrive_CheckStateOfButtons();
        ReadyToDriveEvent(gCurrentEvent); /**< do */
        break;

    case STATE_SET_PARAMETERS
        SetParameters_SetNextParamSet(); /**< entry */
        SetParameters_DisplayParamSet(); /**< exit */
        gCurrentState = STATE_READY_TO_DRIVE;
        break;

    case STATE_DISPLAY_COUNTDOWN:      
        if (false == gEntryDone)
        {
            gEntryDone = true;
            DisplayCountdown_StartCountdown(); /**< entry */
        }
        gCurrentEvent = DisplayCountdown_DisplayCountdown();
        DisplayCountDownEvent(gCurrentEvent); /**< do */
        break;

    case STATE_DRIVE_TO_START:        
        if (false == gEntryDone)
        {
          gEntryDone = true;
          DriveToStart_StartMotorsAndTimer(); /**< entry */
        }
        gCurrentEvent = DriveToStart_FollowGuideLine();
        DriveToStartEvent(gCurrentEvent); /**< do */
        break;

    case STATE_DRIVE_TO_FINISH:
        gCurrentEvent = DriveToFinish_FollowGuideLine(gLapTimer);
        DriveToFinishEvent(gCurrentEvent); /**< do */
      break;

    case STATE_ERROR_HANDLER:
        if (false == gEntryDone)
        {
            gEntryDone = true;
            ErrorHandlerState_CallErrorHandler(gCurrentEvent); /**< entry */
        }
        ErrorHandlerState_WaitForReset(); /**< do */
        break;

    case STATE_DRIVE_OVER_GAP:
        if (false == gEntryDone)
        {
            gEntryDone = true;
            DriveOverGap_SaveCurrentLapTime(gLapTimer); /**< entry */
        }
        gCurrentEvent = DriveOverGap_DriveOverGap(gLapTimer);
        DriveOverGapEvent(gCurrentEvent); /**< do */
        break;

    case STATE_DISPLAY_LAP_TIME:
        gCurrentEvent = DisplayLapTime_StopAfterLap(); /**< entry */
        if (POWER_TO_THE_MOTORS_HAS_BEEN_STOPPED == gCurrentEvent)
        {
            DisplayLapTime_DisplayLapTime(); /**< exit */
        }
        break;

    default:
        break;
  }
}
/* INTERNAL FUNCTIONS *****************************************************************************/ 

static void DriveToStartEvent(EventEnum gCurrentEvent)
{
    if (DRIVE_TO_START_IS_ACTIVE_FOR_TOO_LONG == gCurrentEvent)
        {
            gCurrentState = STATE_ERROR_HANDLER;
            gEntryDone = false;
            DriveToStart_StopTimer(); /**< exit */
        }
        else if (START_FINISH_LINE_WAS_RECOGINZED == gCurrentEvent)
        {
            gCurrentState = STATE_DRIVE_TO_FINISH;
            gEntryDone = false;
            DriveToStart_StopTimer(); /**< exit */
            gLapTimer = DriveToStart_StartTimerAndBeep(); /**< State drive to finish is next */
        }
        else
        {
            /** nothing should happen */
        }
}

static void DriveToFinishEvent(EventEnum gCurrentEvent)
{
    switch (gCurrentEvent)
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
}

static void DriveOverGapEvent(EventEnum gCurrentEvent)
{
    if (THE_GUIDELINE_WAS_RECOGNIZED == gCurrentEvent)
    {
        gCurrentState = STATE_DRIVE_TO_FINISH;
        gEntryDone = false;
    } 
    else if (DRIVE_OVER_GAP_IS_ACTIVE_FOR_TOO_LONG == gCurrentEvent)
    {
        gCurrentState = STATE_ERROR_HANDLER;
        gEntryDone = false;
    }
    else
    {
        /** nothing should happen */
    }
}

static void DisplayCountDownEvent(EventEnum gCurrentEvent)
{
    if (COUNTDOWN_IS_FINISHED == gCurrentEvent)
    {
        gEntryDone = false;
        DisplayCountdown_StopCountdown(); /**< exit */
        gCurrentState = STATE_DRIVE_TO_START;
    }
    else
    {
        /** nothing should happen */
    }
}

static void ReadyToDriveEvent(EventEnum gCurrentEvent)
{
    switch (gCurrentEvent)
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
        /** nothing should happen */
        break; 
    }
}

static void CalibrateLineSensorsEvent(EventEnum gCurrentEvent)
{
    switch (gCurrentEvent)
    {
    case CALIBRATION_FAILED:
        gEntryDone = false;
        gCurrentState = STATE_ERROR_HANDLER;
        break;

    case CALIBRATION_DONE:
        gEntryDone = false;
        gCurrentState = STATE_READY_TO_DRIVE;
        break;

    default:
        /** nothing should happen */
        break; 
    }
}
