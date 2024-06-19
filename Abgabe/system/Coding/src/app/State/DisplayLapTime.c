/***************************************************************************************************
  (c) Team 🏁~~ ō͡≡o\ (Maurice Ott, Simon Walderich, Thorben Päpke) 2024
***************************************************************************************************/
/**
 * @file       DisplayLapTime.c
 *
 *    Module handles displaying the time needed to complete one lap on the OLED display
 *    and stops the timer as well as the power to the motors 
 */
/**************************************************************************************************/

/* INCLUDES ***************************************************************************************/
#include "DisplayLapTime.h"

#include <stdio.h>
#include "app/DriveHandler.h"
#include "Common/Debug.h"
#include "service/Buzzer.h"
#include "service/Display.h"

#ifdef GET_LAP_DATA
    #include <util/delay.h>
    #include <stdio.h>
    #include <service/DriveControl.h>
    #include "app/State/SetParameters.h"
    #include "hal/TickTimer.h"
#endif

/* CONSTANTS **************************************************************************************/
#ifdef GET_LAP_DATA
    #define TRIALS 6U /**< Trials per set: amount of laps driven before calculating and displaying the average */
#endif
#define MS_PER_SEC (1000U) /**< Amount of milliseconds per one second */
#define DISP_TXT_LEN_MAX ((DISPLAY_MAX_LENGTH) + 1U) /**< Maximum text length for one line on the display */

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/
#ifdef GET_LAP_DATA
    static UInt8 gTrial = 0U; /**< Stores the current trial number */
    static UInt8 gSet = 0U; /**< Stores the current set number */
    static Bool  gTurnRight = FALSE; /**< Store if the last turn was clockwise, to be able to alternate turning directions */
    static UInt8 gDispX = 0U; /**< Store the current x position on the display */
    static UInt8 gDispY = 2U; /**< Store the current y position on the display */
    static UInt32 gLapTimeSum = 0UL; /**< Sum of the lap times per set to be able to calculate the average */
#endif

/* EXTERNAL FUNCTIONS *****************************************************************************/
EventEnum DisplayLapTime_stopAfterLap(SoftTimer* lapTimer)
{
    /* Stop the lapTime timer */
    SoftTimer_Stop(lapTimer);

    /* Emit a short beep */
    Buzzer_beep(BUZZER_NOTIFY);



    #ifdef GET_LAP_DATA
        /* Continue driving for a few centimeters */
        LineSensorValues sensorValues;
        DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT,  100, DRIVE_CONTROL_FORWARD);
        DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, 100, DRIVE_CONTROL_FORWARD);
        UInt16 ticksEnd;
        ticksEnd = TickTimer_getTicks() + 5000U;
        while (0 < (Int16)(ticksEnd - TickTimer_getTicks())) {
            LineSensor_read(&sensorValues, NULL);
            DriveHandler_followGuideLine(&sensorValues);
        }
        /* Come slowly to a stop */
        for (UInt8 speed = 100U; speed > 0U; speed--) {
            DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT,  speed, DRIVE_CONTROL_FORWARD);
            DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, speed, DRIVE_CONTROL_FORWARD);
            ticksEnd = TickTimer_getTicks() + 150U;
            while (0 < (Int16)(ticksEnd - TickTimer_getTicks())) {
                LineSensor_read(&sensorValues, NULL);
                DriveHandler_followGuideLine(&sensorValues);
            }
        }
        /* Turn around */
        for (UInt8 speed = 0U; speed <= 50U; speed++) {
            DriveControl_drive(DRIVE_CONTROL_MOTOR_LEFT,  speed, !gTurnRight);
            DriveControl_drive(DRIVE_CONTROL_MOTOR_RIGHT, speed, gTurnRight);
            _delay_ms(3);
        }
        gTurnRight = !gTurnRight;
        _delay_ms(100);
        do {
            LineSensor_read(&sensorValues, NULL);
        } while (CALIB_NO_LINE(sensorValues.value[LINESENSOR_MIDDLE]));
        /* Stop turning */
        DriveHandler_stopDriving();
    #endif



    /* Stop the power supply to the DriveMotors */
    DriveHandler_stopDriving();

    LineSensor_disableEmitter();

    return POWER_TO_THE_MOTORS_HAS_BEEN_STOPPED;
}

EventEnum DisplayLapTime_displayLapTime(SoftTimer* lapTimer)
{
    EventEnum retVal = NO_EVENT_HAS_HAPPEND;

    #ifdef GET_LAP_DATA
        /* Init variables */
        retVal = START_BUTTON_HAS_BEEN_RELEASED;	
        char txt[DISP_TXT_LEN_MAX] = {0};

        if ((0U == gTrial) && (0U == gSet))
        {
            /* Init display */
            Display_clear();
            gDispX = 0U;
            gDispY = 2U;

            /* Print parameters on display */
            ParamSet params = SetParameters_getCurrentParamSet();
            UInt8 length = sprintf(txt, "%02u+5 ", params.pidD);
            Display_gotoxy(gDispX, gDispY);
            Display_writeWithLength(txt, length);
            gDispX += length;
            gLapTimeSum = 0UL;
        }

        UInt16 time = lapTimer->threshHold - lapTimer->counter;
        gLapTimeSum += time;

        gTrial++;
        if (gTrial == TRIALS) {
            /* Print result */
            UInt8 length = sprintf(txt, "%04u ", (UInt16)(gLapTimeSum/TRIALS));
            Display_gotoxy(gDispX, gDispY);
            Display_writeWithLength(txt, length);
            gDispX += length;
            if (21U < gDispX + 4U) {
                gDispX = 0U;
                gDispY++;
            }

            /* Change paramSet */
            ParamSet params = SetParameters_getCurrentParamSet();
            params.pidD += 5U;
            SetParameters_setCurrentParamSet(params);

            /* Go to next trial */
            gSet++;
            gTrial = 0U;
            gLapTimeSum = 0UL;
            if (22U == gSet) {
                gSet = 0U;
                retVal = NO_EVENT_HAS_HAPPEND;
            }
        }
    #else
        /* Shows the completed lap time on the Display */
        UInt16 time = lapTimer->threshHold - lapTimer->counter;
        Display_gotoxy(0, 2);
        char txt[DISP_TXT_LEN_MAX] = {0};
        UInt8 length = sprintf(txt, "Lap time %2u:%03u", time / MS_PER_SEC, time % MS_PER_SEC);
        Display_writeWithLength(txt, length);
    #endif

    return retVal;
}

/* INTERNAL FUNCTIONS *****************************************************************************/
