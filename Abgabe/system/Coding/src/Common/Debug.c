/***************************************************************************************************
  (c) Team 🏁~~ ō͡≡o\ (Maurice Ott, Simon Walderich, Thorben Päpke) 2024
***************************************************************************************************/
/**
 * @file       Debug.c
 *
 *    Module takes care of all debug functions
 */
/**************************************************************************************************/

/* INCLUDES ***************************************************************************************/
#include "Common/Debug.h"

#include <stdio.h>
#include <string.h>
#include "app/DriveHandler.h"
#include "hal/TickTimer.h"
#include "service/Display.h"

/* CONSTANTS **************************************************************************************/
#define LINE_MIN (4U) /**< Minimum line on Display to output debug information */
#define LINE_MAX (7U) /**< Maximum line on Display to output debug information */
#define DISP_TXT_LEN_MAX ((DISPLAY_MAX_LENGTH) + 1U) /**< Maximum text length for one line on the display */

/* Magic number avoidance */
#define DEC (10) /**< Amount of possible different ciphers in the decimal system */

/** This defines the text position for a temporarily used
 * string that is only used once in the upcoming code.
 * One would think that it would be okay to just define the string position
 * directly at the declaration of said string variable. And this would also be
 * really easy to read and understand, because the position would be written down
 * exactly at the point where it is used and not hundreds of lines of codes away
 * from that. But that is only what a very naive and inexperienced programmer would think.
 * Because than this would count as a magic number, a number that comes from nowhere
 * and is not explained anywhere, which is really bad! Generations of programmers
 * would come across this magic number and wonder why it has this exact value.
 * For most of them it would be an unsolveable riddle, some of them would most likely
 * go insane, wondering about this number until their deathbed, wheezing
 * "What does it mean!?" after taking their last breath. And this would just
 * lead to an endless loop of suffering. Their offspring would go on a journey
 * of revenge just to suffer the exact same fate.
 * This is the reason why this definition and the corresponding doxygen comment
 * is so vitally important for you, who is reading this, and mankind.
 * But the programmer who put a lot of thought into this knows precisely
 * why he choose the exact number down below and not a bit
 * more or less. And the good thing is, he is willing to share
 * this invaluable knowledge:
 * The programmer thought the hereby defined position would be right to get the
 * position of the next number in the string */
#define TXT_POS (6U)


/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/
static UInt16 gLineNo = 1U; /**< Number that increments for every written debug message */
static UInt8 gDispLine = LINE_MIN; /**< Current line number to print on the display */
static UInt64 gTicksStart = 0ULL; /**< Stores a timestamp to be able to stop past time */

/* EXTERNAL FUNCTIONS *****************************************************************************/
void Debug_showMsg(const char * msg)
{
    /* go to next line */
    if (LINE_MAX < gDispLine)
    {
        gDispLine = LINE_MIN;
    }
    Display_gotoxy(0, gDispLine++);
    Display_clearLine();

    /* add line number and print message */
    char txt[DISP_TXT_LEN_MAX] = {0};
    UInt8 length = snprintf(txt, DISP_TXT_LEN_MAX, "%d: %s", gLineNo++, msg);
    Display_writeWithLength(txt, length);
}

void Debug_showMsgNum(const UInt16 num, const char * line)
{
    /* go to next line */
    if (LINE_MAX < gDispLine)
    {
        gDispLine = LINE_MIN;
    }
    Display_gotoxy(0, gDispLine++);
    Display_clearLine();

    /* add line number and print message */
    char txt[DISP_TXT_LEN_MAX] = {0};
    UInt8 length = snprintf(txt, DISP_TXT_LEN_MAX, "%d: %d:%s", gLineNo++, num, line);
    Display_writeWithLength(txt, length);
}

void Debug_showCalibData()
{
    char txt[DISP_TXT_LEN_MAX] = {0};
    Display_gotoxy(0, 0);
    Display_clearLine();
    Display_write("#  min max rng");

    /* Show calibration data (minimum, maximum and range) for every line sensor */
    LineSensorCalibrationData calibData;
    for (UInt8 sensorNo = 0U; sensorNo < LINESENSOR_COUNT; sensorNo++) {
        calibData = LineSensor_getCalibrationData(sensorNo);
        UInt8 length = snprintf(txt, DISP_TXT_LEN_MAX, "%d: %3d %3d %3d", gLineNo++, calibData.minVal, calibData.maxVal, calibData.range);

        Display_gotoxy(0, sensorNo + 1U);
        Display_clearLine();
        Display_writeWithLength(txt, length);
    }
}

void Debug_showLineRecognition(LineSensorValues *sensorValues)
{
    char txt[LINESENSOR_COUNT + 1U] = {0};
    for (UInt8 idxSensor = 0; idxSensor < LINESENSOR_COUNT; idxSensor++)
    {
        if (CALIB_NO_LINE(sensorValues->value[idxSensor])) {
            txt[idxSensor] = '-';
        } else if(CALIB_OVER_LINE(sensorValues->value[idxSensor])) {
            txt[idxSensor] = '+';
        } else {
            txt[idxSensor] = '0';
        }
    }
    Debug_showMsg(txt);
}

void Debug_showPosition()
{
    LineSensor_enableEmitter();
    LineSensorValues sensorValues;
    while (TRUE)
    {
        LineSensor_read(&sensorValues, NULL);
        //LineSensor_readRaw(&sensorValues);
        UInt32 pos = DriveHandler_calculatePosition(&sensorValues);

        char txt[DISP_TXT_LEN_MAX] = {0};
        UInt8 length = snprintf(txt, DISP_TXT_LEN_MAX, "%4ld:   ,  ,  ,  ,  ", pos);

        UInt8 txtPos = TXT_POS;
        for (UInt8 sens = 0U; sens < LINESENSOR_COUNT; sens++)
        {
            UInt16 tmp = sensorValues.value[sens];
            tmp = (tmp + (DEC/2)) / DEC;
            if (10 > tmp) {
                txtPos++;
                txt[txtPos++] = '0' + tmp;
            } else if ((DEC*DEC) > tmp) {
                txt[txtPos++] = '0' + (tmp / DEC);
                txt[txtPos++] = '0' + (tmp % DEC);
            } else {
                txt[txtPos-1] = '0' + (tmp / (DEC*DEC));
                tmp %= (DEC*DEC);
                txt[txtPos++] = '0' + (tmp / DEC);
                txt[txtPos++] = '0' + (tmp % DEC);
            }
            txtPos++;
        }

        /* Print message */
        Display_gotoxy(0, LINE_MIN);
        Display_clearLine();
        Display_writeWithLength(txt, length);
    }
}

void Debug_showPositionBar()
{
    LineSensor_enableEmitter();
    LineSensorValues sensorValues;
    while (TRUE)
    {
        LineSensor_read(&sensorValues, NULL);
        //LineSensor_readRaw(&sensorValues);
        Display_write5Bars(&sensorValues, LINE_MIN);

        UInt32 pos = DriveHandler_calculatePosition(&sensorValues);
        Display_gotoxy(0, LINE_MIN+3);
        Display_writeBar(pos);
    }
}

void Debug_showLineValues()
{
    LineSensor_enableEmitter();
    LineSensorValues sensorValues;
    UInt8 posX = 0, posY = 0;
    while (TRUE)
    {
        //LineSensor_read(&sensorValues, NULL);
        LineSensor_readRaw(&sensorValues);

        char txt[DISP_TXT_LEN_MAX] = {0};
        UInt8 length = snprintf(txt, DISP_TXT_LEN_MAX, "%4d", sensorValues.value[LINESENSOR_RIGHT]);
        Display_gotoxy(posX, posY);
        Display_writeWithLength(txt, length);
        posX += length;
        if (DISPLAY_MAX_LENGTH - length < posX)
        {
            posX = 0U;
            posY++;
            if (DISPLAY_MAX_LINES < posY)
            {
                posY = 0U;
            }
        }
    }
}

void Debug_showMinMax(const UInt16 min, const UInt16 max)
{
    Display_gotoxy(0, 3);
    char txt[DISP_TXT_LEN_MAX] = {0};
    UInt8 length = snprintf(txt, DISP_TXT_LEN_MAX, "min:%4d, max:%4d", min, max);
    Display_writeWithLength(txt, length);
}





void Debug_showTimeStart()
{
    gTicksStart = TickTimer_get();
}


void Debug_showTimeStop(const char * msg)
{
    UInt32 ticks = TickTimer_get() - gTicksStart;
    
    char txt[DISP_TXT_LEN_MAX] = {0};
    snprintf(txt, DISP_TXT_LEN_MAX, "%6ldu %s", ticks, msg);
    Debug_showMsg(txt);

    gTicksStart = TickTimer_get();
}

/* INTERNAL FUNCTIONS *****************************************************************************/
