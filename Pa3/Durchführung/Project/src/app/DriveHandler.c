/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       DriveHandler.c
 *
 *    Module takes care of all driving functions
 */
/**************************************************************************************************/

/* INCLUDES ***************************************************************************************/
#include "DriveHandler.h"

#include "Common/Types.h"
#include "service/DriveControl.h"
#include "State/SetParameters.h"
#include "State/CalibrateLineSensors.h"

/* CONSTANTS **************************************************************************************/
#define OPTIMAL_POS 2000
#define SENSOR_WEIGHT_SCALE (1000u)

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/**
 * Determine position of line under sensor.
 *
 * This function uses a weighted average to compute a logical line position. The value moves from
 * 0 to 4000 if the line moves from left to right of the robot.
 * https://en.wikipedia.org/wiki/Weighted_arithmetic_mean
 *
 *     0 * sensor1 + 1000 * sensor1 + 2000 * sensor2 + ... + 4000 * sensor4
 *     ----------------------------------------------------------------
 *                      sum(sensor0, ... , sensor4)
 *
 * @param sensorValues the current sensor values.
 * @return logical position between 0 and 4000.
 */
static UInt32 calculatePosition(const LineSensorValues *sensorValues);

/**
 * Controller loop to calculate motor speeds
 * 
 * @param[in]  posError  position error
 * @param[out] leftSpeed New speed for left motor
 * @param[out] rightSpeed New speed for right motor
 */
static void regulateSpeed(Int32 posError, UInt16 * leftSpeed, UInt16 * rightSpeed);

/* VARIABLES **************************************************************************************/
UInt32 gCurrentPos = OPTIMAL_POS;
UInt32 gLastPosition = OPTIMAL_POS;
UInt16 gLeftSpeed, gRightSpeed;
Int16 gLastError = 0;


/* EXTERNAL FUNCTIONS *****************************************************************************/

void DriveHandler_StopDriving(void)
{
    DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT, 0, DRIVE_CONTROL_FORWARD);
    DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, 0, DRIVE_CONTROL_FORWARD);
}

void DriveHandler_FindGuideLine(void)
{
    ParamSet gParam = SetParameters_getCurrentParamSet();
    DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT, gParam.maxMotorSpeed, DRIVE_CONTROL_FORWARD);
    DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, gParam.maxMotorSpeed, DRIVE_CONTROL_FORWARD);  
}

void DriveHandler_FollowGuideLine(const LineSensorValues *sensorValues)
{
    gCurrentPos = calculatePosition(sensorValues);
    regulateSpeed(gCurrentPos - OPTIMAL_POS, &gLeftSpeed, &gRightSpeed);
    DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT, gLeftSpeed, DRIVE_CONTROL_FORWARD);
    DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, gRightSpeed, DRIVE_CONTROL_FORWARD);
}

/* INTERNAL FUNCTIONS *****************************************************************************/
static UInt32 calculatePosition(const LineSensorValues *sensorValues)
{
    UInt32 position = 0u;
    UInt32 sum = 0u;
    UInt32 weight = 0u;
    Bool foundLine = FALSE;

    for (UInt8 sensor = 0; sensor < LINESENSOR_COUNT; ++sensor)
    {
        UInt32 val = sensorValues->value[sensor];

        if (CALIB_OVER_LINE(val))
        {
            foundLine = TRUE;
        }

        position += val * weight;
        sum += val;

        weight += SENSOR_WEIGHT_SCALE;
    }

    if (!foundLine)
    {
        position = gLastPosition;
    }
    else
    {
        /* build weighted average */
        position /= sum;
        gLastPosition = position;
    }

    return position;
}

static void regulateSpeed(Int32 error, UInt16 * leftSpeed, UInt16 * rightSpeed)
{
    ParamSet gParam = SetParameters_getCurrentParamSet();
    /* PID controller */
    Int32 proportional = (error * gParam.proNumerator) / gParam.proDenominator;
    Int32 derivative   = ((error - gLastError) * gParam.derNumerator) / gParam.derDenominator;
    Int32 integral     = 0;  /* not needed */
    Int32 speedDifference = proportional + derivative + integral;

    Int32 left = 0; //Todo: gParam->maxMotorSpeed + speedDifference;
    Int32 right = 0; //Todo:gParam->maxMotorSpeed - speedDifference;

    if (left < 0)
    {
        left = 0;
    }
    if (right < 0)
    {
        right = 0;
    }

    if (left > gParam.maxMotorSpeed)
    {
        left = gParam.maxMotorSpeed;
    }
    if (right > gParam.maxMotorSpeed)
    {
        right = gParam.maxMotorSpeed;
    }

    *leftSpeed = left;
    *rightSpeed = right;

    gLastError = error;
}