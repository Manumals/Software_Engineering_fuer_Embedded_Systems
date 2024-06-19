/***************************************************************************************************
  (c) Team 🏁~~ ō͡≡o\ (Maurice Ott, Simon Walderich, Thorben Päpke) 2024
***************************************************************************************************/
/**
 * @file       StateHandler.c
 *
 *    Decides which states are called next depending on the returned events  
 */
/**************************************************************************************************/

/* INCLUDES ***************************************************************************************/
#include "StateHandler.h"

#include "app/EventEnum.h"
#include "app/State/CalibrateLineSensors.h"
#include "app/State/InitializeMcu.h"
#include "app/State/DriveOverGap.h"
#include "app/State/DriveToStart.h"
#include "app/State/DriveToFinish.h"
#include "app/State/DisplayCountdown.h"
#include "app/State/DisplayLapTime.h"
#include "app/State/ErrorHandlerState.h"
#include "app/State/ReadyToDrive.h"
#include "app/State/SetParameters.h"
#include "Common/Debug.h"

#ifdef TEST_SPEEDS
    #include <util/delay.h>
    #include "service/DriveControl.h"
#endif

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/
/** The next state is decided on the basis of the transferred parameter 
 * @param[in] currentEvent Event that occurred in Drive to Start
*/
static void handleDriveToStartEvent(EventEnum currentEvent);

/** The next state is decided on the basis of the transferred parameter 
 * @param[in] currentEvent Event that occurred in Drive to Finish
*/
static void handleDriveToFinishEvent(EventEnum currentEvent);

/** The next state is decided on the basis of the transferred parameter 
 * @param[in] currentEvent Event that occurred in Drive Over Gap
*/
static void handleDriveOverGapEvent(EventEnum currentEvent);

/** The next state is decided on the basis of the transferred parameter 
 * @param[in] currentEvent Event that occurred in Count Down Event
*/
static void handleDisplayCountDownEvent(EventEnum currentEvent);

/** The next state is decided on the basis of the transferred parameter 
 * @param[in] currentEvent Event that occurred in Ready to Drive
*/
static void handleReadyToDriveEvent(EventEnum currentEvent);

/** The next state is decided on the basis of the transferred parameter 
 * @param[in] currentEvent Event that occurred in Calibrate Line Sensors
*/
static void handleCalibrateLineSensorsEvent(EventEnum currentEvent);

/* VARIABLES **************************************************************************************/
/** The current event of the state machine
 * Uses NO_EVENT_HAS_HAPPEND at the beginning and every time no event did occur
 */
static EventEnum gCurrentEvent = NO_EVENT_HAS_HAPPEND; 

/** The current state of the state machine
 *  The first state when the MCU is started must be STATE_INIZALIZATION_MCU */
static StateEnum gCurrentState = STATE_INIZALIZATION_MCU;

/** Pointer to the timer that measures the lap time (time needed to drive from start to finsh line) */
static SoftTimer* gLapTimer = NULL;

/** Variable to save if the entry function of a state was already called */
static Bool gEntryDone = FALSE;

#ifdef COUNT_GAPS
    static UInt8 gGapCount = 0U; /**< Counts how often a gap was detected while running a lap */
#endif

/* EXTERNAL FUNCTIONS *****************************************************************************/
void StateHandler_process(void)
{
    switch (gCurrentState)
    { 
    case STATE_INIZALIZATION_MCU:
        InitializeMcu_initializeAll(); /* entry */
        InitializeMcu_displayTeamName(); /* exit */
        #ifdef NO_CALIB
            gCurrentState = STATE_READY_TO_DRIVE;
        #else
            gCurrentState = STATE_CALIBRATE_LINE_SENSORS;
        #endif
        
        #ifdef TEST_SPEEDS
            for (UInt8 speed = 0; speed <= 100; speed++) {
                Debug_showMsgNum(speed, "speed");
                DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT,  speed, DRIVE_CONTROL_FORWARD);
                DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, speed, DRIVE_CONTROL_FORWARD);
                _delay_ms(1000U);
            }
        #endif
        break;

    case STATE_CALIBRATE_LINE_SENSORS:
        if (FALSE == gEntryDone)
        {
            gEntryDone = TRUE;
            CalibrateLineSensors_initialize(); /* entry */
        }
        gCurrentEvent = CalibrateLineSensors_calibrateSensors();
        handleCalibrateLineSensorsEvent(gCurrentEvent); /* do */
        break;

    case STATE_READY_TO_DRIVE:
        #ifdef SHOW_POSITION
            Debug_showPosition();
        #endif
        #ifdef SHOW_POSITION_BAR
            Debug_showPositionBar();
        #endif

        if (FALSE == gEntryDone)
        {
            gEntryDone = TRUE;
            ReadyToDrive_activateButtons(); /* entry */
        }       
        gCurrentEvent = ReadyToDrive_checkStateOfButtons();
        #ifdef NO_READY_TO_DRIVE
            gCurrentEvent = START_BUTTON_HAS_BEEN_RELEASED;
        #endif
        handleReadyToDriveEvent(gCurrentEvent); /* do */
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
        handleDisplayCountDownEvent(gCurrentEvent); /* do */
        break;

    case STATE_DRIVE_TO_START: 
        if (FALSE == gEntryDone)
        {
            gEntryDone = TRUE;
            DriveToStart_startMotorsAndTimer(); /* entry */
        }
        gCurrentEvent = DriveToStart_followGuideLine();
        handleDriveToStartEvent(gCurrentEvent); /* do */     
        break;

    case STATE_DRIVE_TO_FINISH:
        if (NULL == gLapTimer)
        {
            gCurrentEvent = LAPTIMER_INIT_FAILED;
            gCurrentState = STATE_ERROR_HANDLER;
        }
        else
        {
            gCurrentEvent = DriveToFinish_followGuideLine(gLapTimer);
            handleDriveToFinishEvent(gCurrentEvent); /* do */
        }
      break;

    case STATE_ERROR_HANDLER:
        if (gEntryDone == FALSE)
        {
            gEntryDone = TRUE;
            ErrorHandlerState_callErrorHandler(gCurrentEvent); /* entry */
        }
        ErrorHandlerState_waitForReset(); /* do */
        break;

    case STATE_DRIVE_OVER_GAP:
        if (NULL == gLapTimer)
        {
            gCurrentEvent = LAPTIMER_INIT_FAILED;
            gCurrentState = STATE_ERROR_HANDLER;
        }
        else
        {
            if (FALSE == gEntryDone)
            {
                gEntryDone = TRUE;
                DriveOverGap_saveCurrentLapTime(gLapTimer); /* entry */
            }
            gCurrentEvent = DriveOverGap_driveOverGap(gLapTimer);
            handleDriveOverGapEvent(gCurrentEvent); /* do */
        }
        break;

    case STATE_DISPLAY_LAP_TIME:
        if (NULL == gLapTimer)
        {
            gCurrentEvent = LAPTIMER_INIT_FAILED;
            gCurrentState = STATE_ERROR_HANDLER;
        }
        else
        {
            gCurrentEvent = DisplayLapTime_stopAfterLap(gLapTimer); /* entry */
            if (POWER_TO_THE_MOTORS_HAS_BEEN_STOPPED == gCurrentEvent)
            {
                if (NO_EVENT_HAS_HAPPEND == DisplayLapTime_displayLapTime(gLapTimer)) /* exit */
                {
                    gCurrentState = STATE_READY_TO_DRIVE;
                } else {
                    gCurrentState = STATE_DISPLAY_COUNTDOWN;
                }
            }
        }
        break;

    default:
        break;
    }
}

/* INTERNAL FUNCTIONS *****************************************************************************/ 
static void handleDriveToStartEvent(EventEnum currentEvent)
{
    if (DRIVE_TO_START_IS_ACTIVE_FOR_TOO_LONG == currentEvent)
        {
            gCurrentState = STATE_ERROR_HANDLER;
            gEntryDone = FALSE;
            DriveToStart_stopTimer(); /* exit */
        }
        else if (START_FINISH_LINE_WAS_RECOGINZED == currentEvent)
        {
            gCurrentState = STATE_DRIVE_TO_FINISH;
            #ifdef COUNT_GAPS
                gGapCount = 0U;
            #endif
            gEntryDone = FALSE;
            DriveToStart_stopTimer(); /* exit */
            gLapTimer = DriveToStart_startTimerAndBeep(); /* State drive to finish is next */
            if (NULL == gLapTimer)
            {
                gCurrentEvent = LAPTIMER_INIT_FAILED;
                gCurrentState = STATE_ERROR_HANDLER;
            }
        }
        else
        {
            /* do nothing because nothing is supposed to happen */
        }
}

static void handleDriveToFinishEvent(EventEnum currentEvent)
{
    switch (currentEvent)
    {
    case GUIDELINE_WAS_LOST:
        gCurrentState = STATE_DRIVE_OVER_GAP;
        #ifdef COUNT_GAPS
            gGapCount++;
        #endif
        break;

    case LAPTIME_IS_TOO_LONG:
        gCurrentState = STATE_ERROR_HANDLER;
        break;

    case START_FINISH_LINE_WAS_RECOGINZED:
        gCurrentState = STATE_DISPLAY_LAP_TIME;
        #ifdef COUNT_GAPS
            /* Check if the correct amount of gaps were detected during the lap */
            if (1U != gGapCount) {
                Debug_showMsgNum(gGapCount, "gapCnt");
                #ifdef GET_LAP_DATA
                    DriveHandler_stopDriving();
                    while (TRUE) {}
                #endif
            }
        #endif
        break;

    default:
        break;
    }    
}

static void handleDriveOverGapEvent(EventEnum currentEvent)
{
    if (GUIDELINE_WAS_RECOGNIZED == currentEvent)
    {
        gCurrentState = STATE_DRIVE_TO_FINISH;
        gEntryDone = FALSE;
    } 
    else if (DRIVE_OVER_GAP_IS_ACTIVE_FOR_TOO_LONG == currentEvent)
    {
        gCurrentState = STATE_ERROR_HANDLER;
        gEntryDone = FALSE;
    }
    else
    {
        /* do nothing because nothing is supposed to happen */
    }
}

static void handleDisplayCountDownEvent(EventEnum currentEvent)
{
    if (COUNTDOWN_IS_FINISHED == currentEvent)
    {
        gEntryDone = FALSE;
        DisplayCountdown_stopCountdown(); /* exit */
        gCurrentState = STATE_DRIVE_TO_START;
    }
    else
    {
        /* do nothing because nothing is supposed to happen */
    }
}

static void handleReadyToDriveEvent(EventEnum currentEvent)
{
    switch (currentEvent)
    {
    case CALIBRATION_BUTTON_HAS_BEEN_RELEASED:
        gCurrentState = STATE_CALIBRATE_LINE_SENSORS;
        gEntryDone = FALSE;
        ReadyToDrive_deactivateButtons(); /* exit */
        break;

    case START_BUTTON_HAS_BEEN_RELEASED:
        gCurrentState = STATE_DISPLAY_COUNTDOWN;
        gEntryDone = FALSE;
        ReadyToDrive_deactivateButtons(); /* exit */
        break;

    case PARAM_BUTTON_HAS_BEEN_RELEASED:
        gCurrentState = STATE_SET_PARAMETERS;
        gEntryDone = FALSE;
        ReadyToDrive_deactivateButtons(); /* exit */
        break;

    default:
        /* do nothing because nothing is supposed to happen */
        break; 
    }
}

static void handleCalibrateLineSensorsEvent(EventEnum currentEvent)
{
    switch (currentEvent)
    {
    case CALIBRATION_FAILED:
        gEntryDone = FALSE;
        CalibrateLineSensors_deinitialize();
        gCurrentState = STATE_ERROR_HANDLER;
        break;

    case CALIBRATION_DONE:
        gEntryDone = FALSE;
        CalibrateLineSensors_deinitialize();
        gCurrentState = STATE_READY_TO_DRIVE;

        #ifdef SHOW_CALIB_DATA_RES
            Debug_showCalibData();
            while (TRUE) {}
        #endif
        break;

    default:
        /* do nothing because nothing is supposed to happen */
        break; 
    }
}
