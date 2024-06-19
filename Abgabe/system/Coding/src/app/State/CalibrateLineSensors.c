/***************************************************************************************************
  (c) Team 🏁~~ ō͡≡o\ (Maurice Ott, Simon Walderich, Thorben Päpke) 2024
***************************************************************************************************/
/**
 * @file       CalibrateLineSensors.c
 *
 * Calibrates the five line sensors so that the line is recognized under the current light conditions
 */
/**************************************************************************************************/

/* INCLUDES ***************************************************************************************/
#include "CalibrateLineSensors.h"

#include <util/delay.h>
#include "app/DriveHandler.h"
#include "Common/Debug.h"
#include "hal/TickTimer.h"
#include "os/Scheduler.h"
#include "os/SoftTimer.h"
#include "os/Task.h"
#include "service/DriveControl.h"
#include "service/LineSensor.h"

/* CONSTANTS **************************************************************************************/
#define CALIB_SPEED (33U)       /**< Motor speed while calibrating */
#define CALIB_TURN_DELAY (75U)  /**< Dekay in ms before turning the direction while calibration */
#define CALIB_SPEED_SLOW (25U)  /**< Motor speed while centering on line  */
#define TIME_OUT_TIME (5000U)    /**< Time out for the state */
#define CALIB_WAIT_BEGIN (100U)       /**< Waiting time before atarting the calibration */

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/
/** Enum of all Calibration states */
typedef enum tag_CalibrationState
{
    CALIBRATION_STATE_INIT,                             /**< Initial calibration state.           */
    CALIBRATION_STATE_TURN_RIGHT_UNTIL_LEFT_SENSOR,     /**< State TURN_RIGHT_UNTIL_LEFT_SENSOR.  */
    CALIBRATION_STATE_TURN_LEFT_UNTIL_RIGHT_SENSOR,     /**< State TURN_LEFT_UNTIL_RIGHT_SENSOR.  */
    CALIBRATION_STATE_CENTER_ON_LINE,                   /**< State CENTER_ON_LINE.                */
    CALIBRATION_STATE_FINISHED,                         /**< State FINISHED.                      */
    CALIBRATION_STATE_TIMEOUT                           /**< State TIMEOUT.                       */
} CalibrationState;

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/
/** Calibration state of local state machine */
static CalibrationState gState = CALIBRATION_STATE_INIT;

/** Timer used by calibration steps. */
static SoftTimer gTimer = {0};

/* EXTERNAL FUNCTIONS *****************************************************************************/
void CalibrateLineSensors_initialize(void)
{
    LineSensor_init();
    SoftTimer_init(&gTimer);
    SoftTimerHandler_register(&gTimer);
    SoftTimer_start(&gTimer, CALIB_WAIT_BEGIN);
    gState = CALIBRATION_STATE_INIT;
}

EventEnum CalibrateLineSensors_calibrateSensors(void)
{
    LineSensorValues values;
    LineSensorCalibration calib;
    EventEnum result = NO_EVENT_HAS_HAPPEND;

    switch (gState)
    {
        case CALIBRATION_STATE_INIT:
            #ifdef SHOW_CALIB_STATES
                Debug_showMsg("CalState1 Init");
                //Debug_showMsg(SoftTimer_get(&gTimer), "tmr");
            #endif

            if (SOFTTIMER_IS_EXPIRED(&gTimer))
            {
                gState = CALIBRATION_STATE_TURN_RIGHT_UNTIL_LEFT_SENSOR;
                #ifdef SHOW_CALIB_STATES
                    Debug_showMsg("->CalState1 RUL");
                #endif

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

            LineSensor_read(&values, &calib);
            if (calib.calibrated[LINESENSOR_LEFT] && CALIB_OVER_LINE(values.value[LINESENSOR_LEFT]))
            {
                _delay_ms(CALIB_TURN_DELAY);
                SoftTimer_restart(&gTimer);
                gState = CALIBRATION_STATE_TURN_LEFT_UNTIL_RIGHT_SENSOR;
                #ifdef SHOW_CALIB_STATES
                    Debug_showMsg("->CalState2 LUR");
                #endif
            }
            break;

        case CALIBRATION_STATE_TURN_LEFT_UNTIL_RIGHT_SENSOR:
            DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT, CALIB_SPEED, DRIVE_CONTROL_BACKWARD);
            DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, CALIB_SPEED, DRIVE_CONTROL_FORWARD);

            if (SOFTTIMER_IS_EXPIRED(&gTimer))
            {
                gState = CALIBRATION_STATE_TIMEOUT;
            }

            LineSensor_read(&values, &calib);
            if (calib.calibrated[LINESENSOR_RIGHT] && CALIB_OVER_LINE(values.value[LINESENSOR_RIGHT]))
            {
                _delay_ms(CALIB_TURN_DELAY);
                if (!LineSensor_getCalibrationState())
                {
                    /** restart sequence, some sensors not yet calibrated. */
                    gState = CALIBRATION_STATE_TURN_RIGHT_UNTIL_LEFT_SENSOR;
                    #ifdef SHOW_CALIB_STATES
                        Debug_showMsg("->CalState1 RUL!");
                    #endif
                }
                else 
                {
                    SoftTimer_restart(&gTimer);
                    gState = CALIBRATION_STATE_CENTER_ON_LINE;
                    #ifdef SHOW_CALIB_STATES
                        Debug_showMsg("->CalState3 COL");
                    #endif
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

            LineSensor_read(&values, NULL);

            /** stop if only middle sensor sees a line */
            #ifdef SHOW_CALIB_LINE_RECOGNITION
                Debug_showLineRecognition(&values);
            #endif

            if (CALIB_NO_LINE(values.value[LINESENSOR_LEFT]) &&
                CALIB_NO_LINE(values.value[LINESENSOR_MIDDLE_LEFT]) &&
                CALIB_OVER_LINE(values.value[LINESENSOR_MIDDLE]) &&
                CALIB_NO_LINE(values.value[LINESENSOR_MIDDLE_RIGHT]) &&
                CALIB_NO_LINE(values.value[LINESENSOR_RIGHT]))
            {
                DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT, 0U, DRIVE_CONTROL_FORWARD);
                DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, 0U, DRIVE_CONTROL_BACKWARD);

                SoftTimer_Stop(&gTimer);

                gState = CALIBRATION_STATE_FINISHED;
                #ifdef SHOW_CALIB_STATES
                    Debug_showMsg("Cal done");
                #endif
            }
            break;

        case CALIBRATION_STATE_TIMEOUT:

            DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT, 0U, DRIVE_CONTROL_FORWARD);
            DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, 0U, DRIVE_CONTROL_BACKWARD);
            LineSensor_stopCalibration();

            result = CALIBRATION_FAILED;
            break;

        case CALIBRATION_STATE_FINISHED:
            LineSensor_stopCalibration();

            result = CALIBRATION_DONE;
            break;
    }
    return result;
}

void CalibrateLineSensors_deinitialize(void)
{
    LineSensor_stopCalibration();
    SoftTimer_Stop(&gTimer);
    SoftTimerHandler_unRegister(&gTimer);
    LineSensor_deinit();
}

/* INTERNAL FUNCTIONS *****************************************************************************/
