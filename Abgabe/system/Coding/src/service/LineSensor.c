/***************************************************************************************************
  (c) NewTec GmbH System-Entwicklung und Beratung 2015   -   www.newtec.de
  $URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/branches/%23OLED/system/Coding/src/service/LineSensor.c $
  
  (c) Team 🏁~~ ō͡≡o\ (Maurice Ott, Simon Walderich, Thorben Päpke) 2024
***************************************************************************************************/
/**
@addtogroup Service
@{
@file       LineSensor.c

Functions to control the 5 Line sensors on the Pololu Zumo.

@version    %$Id: LineSensor.c 316 2024-03-04 14:38:23Z dembinsky $
* @}
***************************************************************************************************/

/* INCLUDES ***************************************************************************************/
#include "LineSensor.h"

#include <stdio.h>
#include <avr/pgmspace.h>
#include "Common/Debug.h"
#include "hal/Gpio.h"
#include "hal/TickTimer.h"
#include "os/Scheduler.h"
#include "os/SoftTimer.h"
#include "os/Task.h"
#include "service/Display.h"

/* CONSTANTS **************************************************************************************/
#define HW_INDEPENDENT (0)
#if HW_INDEPENDENT
    /** Minimum range to consider a line sensor as calibrated */
    #define RANGE_MIN (LINESENSOR_READ_TIMEOUT_US / 4U) //This value is too big for the (bad?) sensors
#else
    /** Minimum range to consider a line sensor as calibrated */
    #define RANGE_MIN (50U)
#endif

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/
/** Calibration task worker function.
 *
 * @param[in] data Instance data (unused).
 */
static void calibrationTaskWorker (void * data);

/** Reset calibration data
 */
static void resetCalibration(void);

/* VARIABLES **************************************************************************************/
/** Line signals in read mode. */
static const Gpio_ID readPins[LINESENSOR_COUNT] PROGMEM =
{
    GPIO_LINE_IN_DOWN1,
    GPIO_LINE_IN_DOWN2,
    GPIO_LINE_IN_DOWN3,
    GPIO_LINE_IN_DOWN4,
    GPIO_LINE_IN_DOWN5
};

/** Line signals in write mode. */
static const Gpio_ID writePins[LINESENSOR_COUNT] PROGMEM =
{
    GPIO_LINE_OUT_DOWN1,
    GPIO_LINE_OUT_DOWN2,
    GPIO_LINE_OUT_DOWN3,
    GPIO_LINE_OUT_DOWN4,
    GPIO_LINE_OUT_DOWN5
};

/** Calibration min/max values for each sensor.
* If these values fit the current hardware, the calibration might be skipped, but this is not recommended
*/
static LineSensorCalibrationData gCalibrationData[LINESENSOR_COUNT] = {
    {3,121,118},
    {5,102,97},
    {5,68,63},
    {9,106,97},
    {25,202,177}
};
//values for good algorithm after decreasing bit sizes = {11,168,157}, {11,114,103}, {8,75,67}, {10,117,107}, {21,209,188}
//values for good algorithm before decreasing bit sizes = {{34,332,298}, {18,238,220}, {12,158,146}, {25,236,211}, {50,428,378}};
//values for bad algorithm = {{80, 880, 800}, {80, 660, 580}, {20, 460, 440}, {80, 660, 580}, {100, 1000, 900}};


/** Calibration task structure. */
static Task gCalibrationTask;

/** Calibration poll intervall. */
static SoftTimer gCalibrationTimer;

/* EXTERNAL FUNCTIONS *****************************************************************************/
extern void LineSensor_init(void)
{
    Task_init(&gCalibrationTask, calibrationTaskWorker, TASK_STATE_SUSPENDED, NULL);
    Scheduler_addTask(&gCalibrationTask);
    SoftTimer_init(&gCalibrationTimer);
    SoftTimerHandler_register(&gCalibrationTimer);
}

extern void LineSensor_deinit(void)
{
    SoftTimerHandler_unRegister(&gCalibrationTimer);
    Scheduler_removeTask(&gCalibrationTask);
}

extern void LineSensor_read(LineSensorValues *sensorValues, LineSensorCalibration *calib)
{
#if 0
    char line[LINESENSOR_COUNT+1];
#endif
    LineSensor_readRaw(sensorValues);

    /* Normalize values based on calibration data. */
    for (UInt8 idxSensor = 0U; idxSensor < LINESENSOR_COUNT; ++idxSensor)
    {
        const LineSensorCalibrationData *caldata = &gCalibrationData[idxSensor];

        if (0U != caldata->range)
        {
            UInt8 val = sensorValues->value[idxSensor];
            if (val < caldata->minVal)
            {
                val = 0U;
            } else {
                val -= caldata->minVal;
                UInt16 tmpVal = (UInt16)val * LINESENSOR_NORMALIZED_RANGE / caldata->range;

                if (tmpVal > LINESENSOR_NORMALIZED_RANGE)
                {
                    val = LINESENSOR_NORMALIZED_RANGE;
                } else {
                    val = tmpVal;
                }
            }

            sensorValues->value[idxSensor]      = val;
            if (NULL != calib)
            {
                calib->calibrated[idxSensor] = true;
            }
#if 0
            line[idxSensor] = '1';
#endif
        }
        else
        {
            /* Uncalibrated sensor does not produce anything. */
            sensorValues->value[idxSensor]      = 128U;
            if (NULL != calib)
            {
                calib->calibrated[idxSensor] = false;
            }
#if 0
            line[idxSensor] = '0';
#endif
        }
    }

#if 0
    line[LINESENSOR_COUNT] = 0;
    Display_gotoxy(0, 1);
    Display_write(line, sizeof(line));
#endif
}

#if HW_INDEPENDENT
    extern void LineSensor_readRaw(LineSensorValues * data)
    {
        UInt8 pin;
        UInt8 dummyState;
        UInt64 startTimeUs;
        UInt64 delay;
        bool reloop;

        /* Prepare for reading. */
        for (pin = 0U; pin < LINESENSOR_COUNT; ++pin)
        {
            /* Init to max discharge delay. */
            data->value[pin] = LINESENSOR_NORMALIZED_RANGE;//Todo: This was the original value: LINESENSOR_READ_TIMEOUT_US;

            /* Switch all line pins into write mode for charging. */
            Gpio_alloc(pgm_read_byte(&writePins[pin]), &dummyState);
        }

        TICKTIMER_DELAY_US(10U);   /* Charge sensors for at least 10 micro seconds. */

        /* Switch all line pins into read mode for discharging. */
        for (pin = 0; pin < LINESENSOR_COUNT; ++pin)
        {
            Gpio_alloc(pgm_read_byte(&readPins[pin]), &dummyState);
        }

        startTimeUs = TickTimer_get();

        /** Keep reading pins to measure time until they drop to zero. */
        do
        {
            delay = TickTimer_get() - startTimeUs;
            reloop = false;

            for (pin = 0; pin < LINESENSOR_COUNT; ++pin)
            {
                Gpio_State state;
                Gpio_read(pgm_read_byte(&readPins[pin]), &state);

                if (GPIO_STATE_OFF == state)
                {
                    if (delay < data->value[pin] )
                    {
                        data->value[pin] = delay;
                    }

                }
                else
                {
                    reloop = true; /* At least one sensor is not drained. */
                }
            }
        }
        while (reloop && (delay < LINESENSOR_READ_TIMEOUT_US));
    }

#else

    extern void LineSensor_readRaw(LineSensorValues * data)
    {
        #define COUNT_MAX 511U //Max:511, lower values reduce sensitivity and max processing time of function
        UInt8 pin;
        UInt8 dummyState;

        /* Prepare for reading. */
        UInt16 tmpDelay[LINESENSOR_COUNT];
        for (pin = 0U; pin < LINESENSOR_COUNT; ++pin)
        {
            /* Init to max discharge delay. */
            tmpDelay[pin] = COUNT_MAX;

            /* Switch all line pins into write mode for charging. */
            Gpio_alloc(pgm_read_byte(&writePins[pin]), &dummyState);
        }
        TICKTIMER_DELAY_US(10U);   /* Charge sensors for at least 10 micro seconds. */


        
        #define _PIN1 (1<<PORTF7)
        #define _PIN2 (1<<PORTF5)
        #define _PIN3 (1<<PORTF4)
        #define _PIN4 (1<<PORTF4)
        #define _PIN5 (1<<PORTF6)
        UInt8 todoF = _PIN1 | _PIN2 | _PIN3;
        UInt8 todoD = _PIN4 | _PIN5;

        UInt16 count = 0U;

        /* Switch all line pins into read mode for discharging. */
        for (pin = 0; pin < LINESENSOR_COUNT; ++pin)
        {
            Gpio_alloc(pgm_read_byte(&readPins[pin]), &dummyState);
        }

        /* read all line sensors repeatedly
        if one turns to to 0 for the first time,
        the current count is stored as its value
         */
        do
        {
            UInt8 bits = (~PINF) & todoF;
            if (bits & _PIN1)
            {
                tmpDelay[0] = count;
                todoF &= ~_PIN1;
            }
            if (bits & _PIN2)
            {
                tmpDelay[1] = count;
                todoF &= ~_PIN2;
            }
            if (bits & _PIN3)
            {
                tmpDelay[2] = count;
                todoF &= ~_PIN3;
            }

            bits = (~PIND) & todoD;
            if (bits & _PIN4)
            {
                tmpDelay[3] = count;
                todoD &= ~_PIN4;
            }
            if (bits & _PIN5)
            {
                tmpDelay[4] = count;
                todoD &= ~_PIN5;
            }
            count++;
        }
        while ((todoF || todoD) && (count < COUNT_MAX));

        for (pin = 0; pin < LINESENSOR_COUNT; ++pin)
        {
            data->value[pin] = tmpDelay[pin] / 2U;
        }

        #ifdef SHOW_CALIB_RAW_VALS
            Debug_showMsgNum(tmpDelay[0], "count0");
            Debug_showMsgNum(tmpDelay[1], "count1");
            Debug_showMsgNum(tmpDelay[2], "count2");
            Debug_showMsgNum(tmpDelay[3], "count3");
        #endif
    }
#endif

extern void LineSensor_startCalibration(void)
{
    resetCalibration();

    SoftTimer_start(&gCalibrationTimer, 10U);
    TASK_SET_STATE(&gCalibrationTask, TASK_STATE_RUNNING);
    LineSensor_enableEmitter();
}

extern void LineSensor_stopCalibration(void)
{
    SoftTimer_Stop(&gCalibrationTimer);
    TASK_SET_STATE(&gCalibrationTask, TASK_STATE_SUSPENDED);
    LineSensor_disableEmitter();
}

extern bool LineSensor_getCalibrationState(void)
{
  
     bool result = true;
  

    for (UInt8 idxSensor = 0U; idxSensor < LINESENSOR_COUNT; ++idxSensor)
    {
        if (0U == gCalibrationData[idxSensor].range)
        {
            result = false;
 
            #if 0
                /* Show calibration status (newtec code) */
                char txt[] = "0";
                txt[0] = '0' + idxSensor;
                Display_gotoxy(0, 0);
                Display_write(txt, 1);
                Display_gotoxy(0, 1);
                Display_write("CALIB", 5);
            #endif
            break;
        }
    }

    #ifdef SHOW_CALIB_DATA_LIVE
        /* Show calibration values */
        if (0 && !result)
        {
            for (UInt8 idxSensor = 0U; idxSensor < LINESENSOR_COUNT; ++idxSensor)
            {
                char txt[21] = {0};
                sprintf(txt, "s%d %.3d-%.3d=%.3d", idxSensor, gCalibrationData[idxSensor].maxVal, gCalibrationData[idxSensor].minVal, gCalibrationData[idxSensor].range);
                Display_gotoxy(0, idxSensor);
                Display_write(txt);
            }
        }
    #endif

    return result;
}

extern LineSensorCalibrationData LineSensor_getCalibrationData(UInt8 sensorNo)
{
    LineSensorCalibrationData res = {0};
    if (LINESENSOR_COUNT > sensorNo)
    {
        res = gCalibrationData[sensorNo];
    }
    return res;
}

extern void LineSensor_enableEmitter(void)
{
    GPIO_SET(GPIO_LINE_EMITTER);
    TICKTIMER_DELAY_US(200U);
}

extern void LineSensor_disableEmitter(void)
{
    GPIO_CLEAR(GPIO_LINE_EMITTER);
    TICKTIMER_DELAY_US(200U);
}

/* INTERNAL FUNCTIONS *****************************************************************************/
static void calibrationTaskWorker(void *data)
{
    (void)data;

    LineSensorValues values;

    if (SOFTTIMER_IS_EXPIRED(&gCalibrationTimer))
    {
        SoftTimer_restart(&gCalibrationTimer);

        LineSensor_readRaw(&values);

        for (UInt8 idxSensor = 0; idxSensor < LINESENSOR_COUNT; ++idxSensor)
        {
            LineSensorCalibrationData *caldata = &gCalibrationData[idxSensor];

            if (values.value[idxSensor] < caldata->minVal)
            {
                caldata->minVal = values.value[idxSensor];
            }

            if (values.value[idxSensor] > caldata->maxVal)
            {
                caldata->maxVal = values.value[idxSensor];
            }

            /* Accept calibration if we've seen a big enough range. */
            if (caldata->maxVal > caldata->minVal)
            {
                UInt8 range = caldata->maxVal - caldata->minVal;
                if (range >= RANGE_MIN)
                {
                    caldata->range = range;
                }
            }
        }
    }
}

static void resetCalibration(void)
{
    for (UInt8 idx = 0U; idx < LINESENSOR_COUNT; ++idx)
    {
        gCalibrationData[idx].minVal = LINESENSOR_NORMALIZED_RANGE;
        gCalibrationData[idx].maxVal = 0U;
        gCalibrationData[idx].range  = 0U;
    }
}
