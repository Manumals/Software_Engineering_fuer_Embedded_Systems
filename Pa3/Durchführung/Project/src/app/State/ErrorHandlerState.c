/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       ErrorHandlerState.c
 *
 *    Shows an error message on the display depending on the event
 */
/**************************************************************************************************/

/* INCLUDES ***************************************************************************************/
#include "ErrorHandlerState.h"

#include "app/DriveHandler.h"
#include "service/Buzzer.h"
#include "os/ErrorHandler.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/

void ErrorHandlerState_CallErrorHandler(EventEnum errorReason)
{
    /* Stops the power supply to the DriveMotors */
    DriveHandler_StopDriving();

    /* The AlarmSignal is played on the Buzzer */
    Buzzer_beep(BUZZER_ALARM);

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
    default:
        errorCode = ERRORHANDLER_UNKNOWN_ERROR;
        break;
    }
    ErrorHandler_halt(errorCode);
}

/* INTERNAL FUNCTIONS *****************************************************************************/