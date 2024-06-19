/***************************************************************************************************
  (c) NewTec GmbH 2019   -   www.newtec.de
  $URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/trunk/system/50_Implementierung/Projekte/Linienfolger/20_Beistellung/Delivery/Beistellung_r300/Coding/lib/hal_target/hal/TickTimer.h $

  (c) Team 🏁~~ ō͡≡o\ (Maurice Ott, Simon Walderich, Thorben Päpke) 2024
***************************************************************************************************/
/**
@addtogroup Hal
@{
@file       TickTimer.h

Driver for the system timer tick

Uses Timer0 to generate periodic interrupts.

@version    %$Id: TickTimer.h 300 2024-02-01 12:58:40Z leglaz $
* @}
***************************************************************************************************/
#ifndef TICKTIMER_H
#define TICKTIMER_H

#ifdef __cplusplus
extern "C"
{
#endif

/* INCLUDES ***************************************************************************************/
#include <util/delay.h>
#include "Types.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/
#define TICKTIMER_DELAY_US(us)  _delay_us((us))
#define TICKTIMER_DELAY_MS(ms)  _delay_ms((ms))

/* TYPES ******************************************************************************************/
/** Callback function type for installing a timer interrupt callback function. */
typedef void (*TimerTickCallback)(void);

/* PROTOTYPES *************************************************************************************/
/** Initialize the tick-timer.
 * Timer interrupt shall increment the tick counter ever millisecond.
 */
extern void TickTimer_init (void);

/** Set timer tick callback function.
 * Calling this function will override former setting.
 *
 * @param[in] callback function pointer to timer callback function.
 */
extern void TickTimer_setCallback (TimerTickCallback callback);

/** Get the current value of the tick Counter in micro seconds since timer started.
 * Function call needs ~26us
 * @return tick counter value in microseconds.
 */
extern UInt64 TickTimer_get (void);

/** Get the current value of the tick Counter since timer started. Overflows every 1.3s!
 * Function call needs ~3.3us
 * @return tick counter value in ticks.
 */
extern UInt16 TickTimer_getTicks(void);

/** Delay for the given time in seconds.
 * @param[in] seconds the time to delay in seconds.
 */
extern void TickTimer_delay(UInt8 seconds);

#ifdef __cplusplus
}
#endif

#endif  /* TICKTIMER_H */
