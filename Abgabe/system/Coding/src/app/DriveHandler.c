/***************************************************************************************************
  (c) Team 🏁~~ ō͡≡o\ (Maurice Ott, Simon Walderich, Thorben Päpke) 2024
***************************************************************************************************/
/**
 * @file       DriveHandler.c
 *
 *    Module takes care of all driving functions
 */
/**************************************************************************************************/

/* INCLUDES ***************************************************************************************/
#include "DriveHandler.h"

#include "Common/Debug.h"
#include "Common/Types.h"
#include "hal/TickTimer.h"
#include "service/DriveControl.h"
#include "State/SetParameters.h"

/* CONSTANTS **************************************************************************************/
/** Weight increment per line sensor to build the weighted average for position calculation */
#define SENSOR_WEIGHT_SCALE ((LINESENSOR_NORMALIZED_RANGE) + 1U)
/** Optimal position - the middle between the minimum and maximum position */
#define POS_OPTIMAL ((LINESENSOR_COUNT)/2U * (SENSOR_WEIGHT_SCALE))
/** Minimum possible position determined by the position calculation */
#define POS_MIN (0U)
/** Maximum possible position determined by the position calculation */
#define POS_MAX (((LINESENSOR_COUNT)-1U) * (SENSOR_WEIGHT_SCALE))

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/
/**
 * Controller loop to calculate motor speeds
 * 
 * @param[in]  posError  position error
 * @param[out] leftSpeed New speed for left motor
 * @param[out] rightSpeed New speed for right motor
 */
static void regulateSpeed(Int16 posError, UInt8 * leftSpeed, UInt8 * rightSpeed);

/* VARIABLES **************************************************************************************/
/** Store last position for DriveHandler_calculatePosition to return if no new position can be calculated
 *  because of insufficient line sensor data */
static UInt16 gLastPosition = POS_OPTIMAL;

#ifndef PID_D_INACTIVE
    static Int16 gLastError = 0; /**< Store last error to be able to calculate derivative for PID */
    static UInt16 gTickLast = 0U; /**< Store last tick to calculate current cycle time for PID */
#endif

#ifdef SHOW_DRIVE_CYCLE_TIMES_MINMAX
    static UInt64 gDbgTickLast = 0ULL; /**< Store last tick to calculate current cycle time */
    static UInt16 gDbgTicksMin = 0xFFFFU; /** Store minimum measures cycle time until now */
    static UInt16 gDbgTicksMax = 0U; /** Store maximum measures cycle time until now*/
#endif

/* EXTERNAL FUNCTIONS *****************************************************************************/
void DriveHandler_stopDriving(void)
{
    DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT, 0U, DRIVE_CONTROL_FORWARD);
    DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, 0U, DRIVE_CONTROL_FORWARD);
}

void DriveHandler_findGuideLine(void)
{
    ParamSet param = SetParameters_getCurrentParamSet();
    DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT, param.maxMotorSpeed, DRIVE_CONTROL_FORWARD);
    DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, param.maxMotorSpeed, DRIVE_CONTROL_FORWARD);  
}

void DriveHandler_followGuideLine(const LineSensorValues *sensorValues)
{
    static UInt8 speedLeft = 0U, speedRight = 0U;
    Int16 currentPos = DriveHandler_calculatePosition(sensorValues);
    regulateSpeed(currentPos - POS_OPTIMAL, &speedLeft, &speedRight);
    DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT,  speedLeft,  DRIVE_CONTROL_FORWARD);
    DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, speedRight, DRIVE_CONTROL_FORWARD);
}

UInt8 DriveHandler_driveFastLap(UInt16 time, const LineSensorValues *sensorValues, const Bool right)
{
    const UInt16 NoMagicNumber = 18351U;
    const UInt8  NoMagicNumbre = 100U;
    const UInt8  NoMagicNuwber = 25U;
    const UInt16 NoMagicBumner = 19100U;
    const UInt8  NoMagicMunber = 8U;
    const UInt8  NoMagicRumber = 0U;
    if (right)
    {
        DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT,  NoMagicNumbre, DRIVE_CONTROL_FORWARD);
        DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, NoMagicNuwber, DRIVE_CONTROL_FORWARD);
    }
    else
    {
        DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT,  NoMagicNuwber, DRIVE_CONTROL_FORWARD);
        DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, NoMagicNumbre, DRIVE_CONTROL_FORWARD);
    }
    

    if (((NoMagicBumner < time) && (CALIB_OVER_LINE(sensorValues->value[LINESENSOR_RIGHT*right]))) || (NoMagicNumber < time))
    {
        return NoMagicRumber;
    } else {
        return NoMagicMunber;
    }
}

UInt16 DriveHandler_calculatePosition(const LineSensorValues *sensorValues)
{
    UInt16 retPos = 0U;
    UInt16 sumWeighted = 0U;
    UInt16 sumNormal = 0U;
    Bool foundLineMid = FALSE;

    for (UInt8 sensor = 1U; sensor < LINESENSOR_COUNT-1; sensor++)
    {
        UInt8 val = sensorValues->value[sensor];

        if (CALIB_OVER_LINE(val))
        {
            foundLineMid = TRUE;
        }

        sumWeighted += val * sensor;
        sumNormal += val;
    }

    Bool foundLineLft = CALIB_OVER_LINE(sensorValues->value[LINESENSOR_LEFT]);
    Bool foundLineRgt = CALIB_OVER_LINE(sensorValues->value[LINESENSOR_RIGHT]);

    if (!foundLineMid)
    {
        if (foundLineLft == foundLineRgt)
        {
            retPos = gLastPosition;
        } else {
            retPos = foundLineLft ? POS_MIN : POS_MAX;
            gLastPosition = retPos;
        }
    } else {
        /* build weighted average, SENSOR_WEIGHT_SCALE was factored out before for better performance */
        retPos = ((UInt32)sumWeighted) * SENSOR_WEIGHT_SCALE / sumNormal;
        gLastPosition = retPos;
    }

    return retPos;
}

/* INTERNAL FUNCTIONS *****************************************************************************/
static void regulateSpeed(Int16 error, UInt8 * leftSpeed, UInt8 * rightSpeed)
{
    #ifdef SHOW_DRIVE_CYCLE_TIMES_MINMAX
        //Get min and max time between calls of this function
        UInt64 currTick = TickTimer_get();
        if (0ULL != gDbgTickLast)
        {
            UInt16 ticks = (currTick - gDbgTickLast);
            #ifdef SHOW_DRIVE_CYCLE_TIMES
                Debug_showMsgNum(ticks, "t");
                currTick = 0ULL; //Displaying stuff takes long and affects therefore next measured cycle time
            #endif

            Bool change = FALSE;
            if (gDbgTicksMax < ticks) {
                gDbgTicksMax = ticks;
                change = TRUE;
            }
            if (gDbgTicksMin > ticks) {
                gDbgTicksMin = ticks;
                change = TRUE;
            }
            if (change) {
                currTick = 0ULL; //Displaying stuff takes long and affects therefore next measured cycle time
                Debug_showMinMax(gDbgTicksMin, gDbgTicksMax);
            }
        }
        gDbgTickLast = currTick;
    #endif



    ParamSet param = SetParameters_getCurrentParamSet();

    #ifndef PID_D_INACTIVE
        /* cycle times can differ by a factor of 2 or more, this must be compensated for PidD and PidI.
        For example LineSensor_readRaw is way faster if no line is detected.
        This means without compensation the PID would be different if lines are detected then if they are not.*/
        UInt16 tickNew = TickTimer_getTicks();
        UInt16 ticks = tickNew - gTickLast;
        gTickLast = tickNew;
    #endif

    /* PID controller */
    Int16 proportional = error * param.pidP / 32;
    //Int32 integral += error;  /* decreases the remaining error over time, but it's unimportant if the line is followed exactly */
    #ifdef PID_D_INACTIVE
        Int16 speedDifference = proportional;
    #else
        Int16 derivative   = ((Int32)(error - gLastError)) * param.pidD / ticks;
        Int16 speedDifference = proportional - derivative;
        gLastError = error;
    #endif


    /* Begin with the maximum allowed motor speed,
    subtract the speedDifference from the correct side,
    considering the minimum allowed speed value */
    UInt8 left = param.maxMotorSpeed;
    UInt8 right = param.maxMotorSpeed;

    if (speedDifference >= right)
    {
        right = 0U;
    }
    else if (speedDifference <= -left)
    {
        left = 0U;
    }
    else if (0 <= speedDifference)
    {
        right -= speedDifference;
    }
    else
    {
        left += speedDifference;
    }

    *leftSpeed  = left;
    *rightSpeed = right;
}
