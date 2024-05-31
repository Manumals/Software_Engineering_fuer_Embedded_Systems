/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       CalibrateLineSensors.c
 *
 * Calibrates the five line sensors so that the line is recognized under the current light conditions
 */
/**************************************************************************************************/

/* INCLUDES ***************************************************************************************/
#include "CalibrateLineSensors.h"

#include "service/DriveControl.h"
#include "service/LineSensor.h"

#include "os/Task.h"
#include "os/Scheduler.h"
#include "os/SoftTimer.h"

/* CONSTANTS **************************************************************************************/
#define CALIB_SPEED (33u)       /**< Motor speed while calibrating */
#define CALIB_SPEED_SLOW (25u)  /**< Motor speed while centering on line  */
#define TIME_OUT_TIME (5000u)    /**< Time out for the state */
/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/
/** Calibration state. */
typedef enum tag_CalibrationState
{
    CALIBRATION_STATE_INIT,                             /**< Initial calibration state.           */
    CALIBRATION_STATE_TURN_RIGHT_UNTIL_LEFT_SENSOR,     /**< State TURN_RIGHT_UNTIL_LEFT_SENSOR.  */
    CALIBRATION_STATE_TURN_LEFT_UNTIL_RIGHT_SENSOR,     /**< State TURN_LEFT_UNTIL_RIGHT_SENSOR.  */
    CALIBRATION_STATE_CENTER_ON_LINE,                   /**< State CENTER_ON_LINE.                */
    CALIBRATION_STATE_FINISHED,                         /**< State FINISHED.                      */
    CALIBRATION_STATE_TIMEOUT,                          /**< State TIMEOUT.                       */
    CALIBRATION_STATE_DONE                              /**< State DONE.                          */
} CalibrationState;

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/

/** Calibration state of local state machine */
static CalibrationState gState;

/** Calibration result of local state machine */
static EventEnum gResult;

/** Timer used by calibration steps. */
static SoftTimer gTimer;

/* EXTERNAL FUNCTIONS *****************************************************************************/

void CalibrateLineSensors_initialize(void)
{
    gState = CALIBRATION_STATE_INIT;
}

EventEnum CalibrateLineSensors_calibrateSensors(void)
{
    LineSensorValues values;
    gResult = NO_EVENT_HAS_HAPPEND;

    switch (gState)
    {
        case CALIBRATION_STATE_INIT:
            if (SOFTTIMER_IS_EXPIRED(&gTimer))
            {
                gState = CALIBRATION_STATE_TURN_RIGHT_UNTIL_LEFT_SENSOR;
                SoftTimer_start(&gTimer, TIME_OUT_TIME);
                LineSensor_startCalibration();
            }
            break;

        case CALIBRATION_STATE_TURN_RIGHT_UNTIL_LEFT_SENSOR:
            DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT, CALIB_SPEED, DRIVE_CONTROL_FORWARD);
            DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, CALIB_SPEED, DRIVE_CONTROL_BACKWARD);

            if (SOFTTIMER_IS_EXPIRED(&gTimer))
            {
                gState = CALIBRATION_STATE_TIMEOUT;
            }

            LineSensor_read(&values);
            if (TRUE == (values.calibrated[LINESENSOR_LEFT] && CALIB_OVER_LINE(values.value[LINESENSOR_LEFT])))
            {
                SoftTimer_restart(&gTimer);
                gState = CALIBRATION_STATE_TURN_LEFT_UNTIL_RIGHT_SENSOR;
            }
            break;

        case CALIBRATION_STATE_TURN_LEFT_UNTIL_RIGHT_SENSOR:
            DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT, CALIB_SPEED, DRIVE_CONTROL_BACKWARD);
            DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, CALIB_SPEED, DRIVE_CONTROL_FORWARD);

            if (SOFTTIMER_IS_EXPIRED(&gTimer))
            {
                gState = CALIBRATION_STATE_TIMEOUT;
            }

            LineSensor_read(&values);
            if (TRUE == (values.calibrated[LINESENSOR_RIGHT] &&  CALIB_OVER_LINE(values.value[LINESENSOR_RIGHT])))
            {
                if (!LineSensor_getCalibrationState())
                {
                    /** restart sequence, some sensors not yet calibrated. */
                    gState = CALIBRATION_STATE_TURN_RIGHT_UNTIL_LEFT_SENSOR;
                }
                else 
                {
                    SoftTimer_restart(&gTimer);
                    gState = CALIBRATION_STATE_CENTER_ON_LINE;
                }
            }
            break;

        case CALIBRATION_STATE_CENTER_ON_LINE:
            DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT, CALIB_SPEED_SLOW, DRIVE_CONTROL_FORWARD);
            DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, CALIB_SPEED_SLOW, DRIVE_CONTROL_BACKWARD);

            if (SOFTTIMER_IS_EXPIRED(&gTimer))
            {
                gState = CALIBRATION_STATE_TIMEOUT;
            }

            LineSensor_read(&values);

            /** stop if only middle sensor sees a line */

            if (CALIB_NO_LINE(values.value[LINESENSOR_LEFT]) &&
                CALIB_NO_LINE(values.value[LINESENSOR_MIDDLE_LEFT]) &&
                CALIB_OVER_LINE(values.value[LINESENSOR_MIDDLE]) &&
                CALIB_NO_LINE(values.value[LINESENSOR_MIDDLE_RIGHT]) &&
                CALIB_NO_LINE(values.value[LINESENSOR_RIGHT]))
            {
                DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT, 0u, DRIVE_CONTROL_FORWARD);
                DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, 0u, DRIVE_CONTROL_BACKWARD);

                SoftTimer_Stop(&gTimer);
                SoftTimerHandler_unRegister(&gTimer);

                gState = CALIBRATION_STATE_FINISHED;
            }
            break;

        case CALIBRATION_STATE_TIMEOUT:

            DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT, 0u, DRIVE_CONTROL_FORWARD);
            DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, 0u, DRIVE_CONTROL_BACKWARD);
            LineSensor_stopCalibration();

            gState = CALIBRATION_STATE_DONE;
            gResult = CALIBRATION_FAILED;
            break;

        case CALIBRATION_STATE_FINISHED:
            LineSensor_stopCalibration();
            SoftTimerHandler_unRegister(&gTimer);

            gState = CALIBRATION_STATE_DONE;
            gResult = CALIBRATION_DONE;
            break;
        case CALIBRATION_STATE_DONE:
            break;
    }
    return gResult;
}

/* INTERNAL FUNCTIONS *****************************************************************************/