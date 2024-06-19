/***************************************************************************************************
  (c) Team 🏁~~ ō͡≡o\ (Maurice Ott, Simon Walderich, Thorben Päpke) 2024
***************************************************************************************************/
/**
 * @file       DisplayCountdown.c
 *
 *    Module handles displaying the countdown on the OLED-display 
 */
/**************************************************************************************************/

/* INCLUDES ***************************************************************************************/
#include "DisplayCountdown.h"

#include <stdio.h>
#include "Common/Debug.h"
#include "os/SoftTimer.h"
#include "service/Display.h"

/* CONSTANTS **************************************************************************************/
#define TIME_MAX_MS (3000U) /**< Duration of the countdown in milliseconds */
#define MS_PER_SEC (1000U) /**< Amount of milliseconds per one second */

/** This defines the maximum text length for a temporarily used
 * string that is only used once in the upcoming code.
 * One would think that it would be okay to just define the string length
 * directly at the declaration of said string variable. And this would also be
 * really easy to read and understand, because the length would be written down
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
 * The programmer thought the hereby defined length would be enough to hold the
 * countdown */
#define TXT_LEN_MAX_COUNTDOWN (3U)

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/
static SoftTimer gCountdownTimer = {0}; /**< Timer for the count down */
static UInt8 gLastTime_s = 0U; /**< Stores the last displayed countdown time in seconds */

/* EXTERNAL FUNCTIONS *****************************************************************************/
void DisplayCountdown_startCountdown(void)
{
    gLastTime_s = 0U;
    SoftTimer_init(&gCountdownTimer);
    SoftTimerHandler_register(&gCountdownTimer);
    SoftTimer_start(&gCountdownTimer, TIME_MAX_MS);
}

EventEnum DisplayCountdown_displayCountdown(void)
{
    EventEnum retVal = NO_EVENT_HAS_HAPPEND;
    UInt16 time_ms = SoftTimer_get(&gCountdownTimer);
    UInt8  time_s  = (time_ms + (MS_PER_SEC - 1U)) / MS_PER_SEC; /* get seconds, round up to full seconds */

    /* Display countdown in seconds if the second did change */
    if (gLastTime_s != time_s)
    {
        gLastTime_s = time_s;
        Display_gotoxy(0, 1);
        char txt[TXT_LEN_MAX_COUNTDOWN] = {0};
        UInt8 length = sprintf(txt, "%u", time_s);
        Display_writeWithLength(txt, length);
    }

    /* return value for state handle */
    if (SOFTTIMER_IS_EXPIRED(&gCountdownTimer))
    {
        retVal = COUNTDOWN_IS_FINISHED;
    }
    else
    {
        #ifdef NO_COUNTDOWN
            retVal = COUNTDOWN_IS_FINISHED;
        #else
            retVal = NO_EVENT_HAS_HAPPEND;
        #endif
    }
    return retVal;
}

void DisplayCountdown_stopCountdown(void)
{
    SoftTimer_Stop(&gCountdownTimer);
    SoftTimerHandler_unRegister(&gCountdownTimer);
}

/* INTERNAL FUNCTIONS *****************************************************************************/
