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
static void driveToStartEvent(EventEnum gCurrentEvent);

/** The next state is decided on the basis of the transferred parameter 
 * @param[in] gCurrentEvent Event that occurred in Drive to Finish
*/
static void driveToFinishEvent(EventEnum gCurrentEvent);

/** The next state is decided on the basis of the transferred parameter 
 * @param[in] gCurrentEvent Event that occurred in Drive Over Gap
*/
static void driveOverGapEvent(EventEnum gCurrentEvent);

/** The next state is decided on the basis of the transferred parameter 
 * @param[in] gCurrentEvent Event that occurred in Count Down Event
*/
static void displayCountDownEvent(EventEnum gCurrentEvent);

/** The next state is decided on the basis of the transferred parameter 
 * @param[in] gCurrentEvent Event that occurred in Ready to Drive
*/
static void readyToDriveEvent(EventEnum gCurrentEvent);

/** The next state is decided on the basis of the transferred parameter 
 * @param[in] gCurrentEvent Event that occurred in Calibrate Line Sensors
*/
static void calibrateLineSensorsEvent(EventEnum gCurrentEvent);
/* VARIABLES **************************************************************************************/
static EventEnum gCurrentEvent = NO_EVENT_HAS_HAPPEND; 
static StateEnum gCurrentState = STATE_INIZALIZATION_MCU; /**< First state when the MCU is started */
static SoftTimer* gLapTimer;
static Bool gEntryDone = FALSE;

/* EXTERNAL FUNCTIONS *****************************************************************************/

void StateHandler_process(void)
{
    switch (gCurrentState)
    { 
    case STATE_INIZALIZATION_MCU: 
        InitializeMcu_initializeAll(); /* entry */
        InitializeMcu_displayTeamName(); /* exit */
        gCurrentState = STATE_CALIBRATE_LINE_SENSORS;
        break;

    case STATE_CALIBRATE_LINE_SENSORS:
        if (FALSE == gEntryDone)
        {
            gEntryDone = TRUE;
            CalibrateLineSensors_initialize(); /* entry */
        }
        gCurrentEvent = CalibrateLineSensors_calibrateSensors();
        calibrateLineSensorsEvent(gCurrentEvent); /* do */
        break;

    case STATE_READY_TO_DRIVE:
        gCurrentEvent = ReadyToDrive_checkStateOfButtons();
        readyToDriveEvent(gCurrentEvent); /* do */
        break;

    case STATE_SET_PARAMETERS:
        SetParameters_setNextParamSet(); /* entry */
        SetParameters_displayParamSet(); /* exit */
        gCurrentState = STATE_READY_TO_DRIVE;
        break;

    case STATE_DISPLAY_COUNTDOWN:      
        if (FALSE == gEntryDone)
        {
            gEntryDone = TRUE;
            DisplayCountdown_startCountdown(); /* entry */
        }
        gCurrentEvent = DisplayCountdown_displayCountdown();
        displayCountDownEvent(gCurrentEvent); /* do */
        break;

    case STATE_DRIVE_TO_START:        
        if (FALSE == gEntryDone)
        {
          gEntryDone = TRUE;
          DriveToStart_startMotorsAndTimer(); /* entry */
        }
        gCurrentEvent = DriveToStart_followGuideLine();
        driveToStartEvent(gCurrentEvent); /* do */
        break;

    case STATE_DRIVE_TO_FINISH:
        gCurrentEvent = DriveToFinish_followGuideLine(gLapTimer);
        driveToFinishEvent(gCurrentEvent); /* do */
      break;

    case STATE_ERROR_HANDLER:
        ErrorHandlerState_callErrorHandler(gCurrentEvent); /* entry */
        break;

    case STATE_DRIVE_OVER_GAP:
        if (FALSE == gEntryDone)
        {
            gEntryDone = TRUE;
            DriveOverGap_saveCurrentLapTime(gLapTimer); /* entry */
        }
        gCurrentEvent = DriveOverGap_driveOverGap(gLapTimer);
        driveOverGapEvent(gCurrentEvent); /* do */
        break;

    case STATE_DISPLAY_LAP_TIME:
        gCurrentEvent = DisplayLapTime_stopAfterLap(gLapTimer); /* entry */
        if (POWER_TO_THE_MOTORS_HAS_BEEN_STOPPED == gCurrentEvent)
        {
            DisplayLapTime_displayLapTime(gLapTimer); /* exit */
        }
        break;

    default:
        break;
  }
}
/* INTERNAL FUNCTIONS *****************************************************************************/ 

static void driveToStartEvent(EventEnum gCurrentEvent)
{
    if (DRIVE_TO_START_IS_ACTIVE_FOR_TOO_LONG == gCurrentEvent)
        {
            gCurrentState = STATE_ERROR_HANDLER;
            gEntryDone = FALSE;
            DriveToStart_stopTimer(); /* exit */
        }
        else if (START_FINISH_LINE_WAS_RECOGINZED == gCurrentEvent)
        {
            gCurrentState = STATE_DRIVE_TO_FINISH;
            gEntryDone = FALSE;
            DriveToStart_stopTimer(); /* exit */
            gLapTimer = DriveToStart_startTimerAndBeep(); /* State drive to finish is next */
        }
        else
        {
            /* nothing should happen */
        }
}

static void driveToFinishEvent(EventEnum gCurrentEvent)
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

static void driveOverGapEvent(EventEnum gCurrentEvent)
{
    if (THE_GUIDELINE_WAS_RECOGNIZED == gCurrentEvent)
    {
        gCurrentState = STATE_DRIVE_TO_FINISH;
        gEntryDone = FALSE;
    } 
    else if (DRIVE_OVER_GAP_IS_ACTIVE_FOR_TOO_LONG == gCurrentEvent)
    {
        gCurrentState = STATE_ERROR_HANDLER;
        gEntryDone = FALSE;
    }
    else
    {
        /* nothing should happen */
    }
}

static void displayCountDownEvent(EventEnum gCurrentEvent)
{
    if (COUNTDOWN_IS_FINISHED == gCurrentEvent)
    {
        gEntryDone = FALSE;
        DisplayCountdown_stopCountdown(); /* exit */
        gCurrentState = STATE_DRIVE_TO_START;
    }
    else
    {
        /* nothing should happen */
    }
}

static void readyToDriveEvent(EventEnum gCurrentEvent)
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
        /* nothing should happen */
        break; 
    }
}

static void calibrateLineSensorsEvent(EventEnum gCurrentEvent)
{
    switch (gCurrentEvent)
    {
    case CALIBRATION_FAILED:
        gEntryDone = FALSE;
        gCurrentState = STATE_ERROR_HANDLER;
        break;

    case CALIBRATION_DONE:
        gEntryDone = FALSE;
        gCurrentState = STATE_READY_TO_DRIVE;
        break;

    default:
        /* nothing should happen */
        break; 
    }
}
