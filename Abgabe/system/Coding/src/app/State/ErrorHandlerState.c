/***************************************************************************************************
  (c) Team 🏁~~ ō͡≡o\ (Maurice Ott, Simon Walderich, Thorben Päpke) 2024
***************************************************************************************************/
/**
 * @file       ErrorHandlerState.c
 *
 *    Shows an error message on the display depending on the event
 */
/**************************************************************************************************/

/* INCLUDES ***************************************************************************************/
#include "ErrorHandlerState.h"

#include <util/delay.h>
#include "app/DriveHandler.h"
#include "Common/Debug.h"
#include "os/ErrorHandler.h"
#include "service/Buzzer.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/
void ErrorHandlerState_callErrorHandler(EventEnum errorReason)
{
    /* Stops the power supply to the DriveMotors */
    DriveHandler_stopDriving();

    /* The AlarmSignal is played on the Buzzer */
    Buzzer_beep(BUZZER_ALARM);

    LineSensor_disableEmitter();

    /* An error message is displayed on the OledDisplay */
    ErrorHandlerErrorCode errorCode;
    switch (errorReason)
    {
    case CALIBRATION_FAILED:
        errorCode = ERRORHANDLER_CALIBRATE_FAIL;
        break;
    case INIZALIZATION_FAILED:
        errorCode = ERRORHANDLER_STARTUP_INIT_FAIL;
        break;
    case GUIDELINE_WAS_LOST:
        errorCode = ERRORHANDLER_LINELOST_TIMEOUT;
        break;
    case LAPTIME_IS_TOO_LONG:
        errorCode = ERRORHANDLER_DRIVING_TIMEOUT;
        break;
    case DRIVE_OVER_GAP_IS_ACTIVE_FOR_TOO_LONG:
        errorCode = ERRORHANDLER_LINELOST_TIMEOUT;
        break;
    case DRIVE_TO_START_IS_ACTIVE_FOR_TOO_LONG:
        errorCode = ERRORHANDLER_DRIVING_TIMER_START_FAIL;
        break;
    case LAPTIMER_INIT_FAILED:
        errorCode = ERRORHANDLER_FINISH_TIMER_INIT_FAIL;
        break;
    default:
        errorCode = ERRORHANDLER_UNKNOWN_ERROR;
        break;
    }
    ErrorHandler_show(errorCode);
}

void ErrorHandlerState_waitForReset(void)
{
    /* Do nothing and return, so the scheduler still works */
}

/* INTERNAL FUNCTIONS *****************************************************************************/
