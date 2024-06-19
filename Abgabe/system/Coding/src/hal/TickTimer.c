/***************************************************************************************************
  (c) NewTec GmbH 2019   -   www.newtec.de
  $URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/trunk/system/50_Implementierung/Projekte/Linienfolger/20_Beistellung/Delivery/Beistellung_r300/Coding/lib/hal_target/hal/TickTimer.c $
  
  (c) Team 🏁~~ ō͡≡o\ (Maurice Ott, Simon Walderich, Thorben Päpke) 2024
***************************************************************************************************/
/**
@addtogroup Hal
@{
@file       TickTimer.c

Driver for the system timer tick

For a detailed description see the detailed description in @ref TickTimer.h

@version    %$Id: TickTimer.c 300 2024-02-01 12:58:40Z leglaz $
* @}
***************************************************************************************************/

/* INCLUDES ***************************************************************************************/
#include "TickTimer.h"

#include <avr/io.h>
#include "Irq.h"

/* CONSTANTS **************************************************************************************/
/** Timer resolution in microseconds */
#define TICKTIMER_RESOLUTION_US  (20U)
#define TIME_CONST (1200U)
#define MS_PER_SEC (1000U)
#define CPU_FREQ_HZ (16000000)

#if  (CPU_FREQ_HZ == F_CPU)
    #define TICKTIMER_PRESCALER   ((0<<CS02)|(1<<CS01)|(0<<CS00))      /* Prescaler 8 -> 2Mhz. */
    #define TICKTIMER_OCR_PRELOAD (((TICKTIMER_RESOLUTION_US) * 2U)-1U)   /* Update every res us. */
#else
    #error unknown Frequency
#endif

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/
/** Number of ticks since timer (interrupts) got enabled im milliseconds. */
static volatile UInt32 gTickCounterMs;

/** Number of ticks im us resolution. */
static volatile UInt8 gTickCounterUs;

/** Callback function that gets called during timer tick interrupt. */
TimerTickCallback gTickCallback;

/* EXTERNAL FUNCTIONS *****************************************************************************/
void TickTimer_init(void)
{
    gTickCounterMs = 0U;
    gTickCounterUs = 0U;
    gTickCallback = NULL;

    TCCR0A = (1 << WGM01);         /* CTC mode. */
    OCR0A = TICKTIMER_OCR_PRELOAD;
    TIMSK0 |= (1 << OCIE0A);       /* Interrupt at COMPA. */
    TCCR0B =  TICKTIMER_PRESCALER; /* Init prescaler to start timer. */
}

void TickTimer_setCallback(TimerTickCallback callback)
{
    IRQ_ENTER_ATOMIC_SECTION();
    gTickCallback = callback;
    IRQ_LEAVE_ATOMIC_SECTION();
}

UInt64 TickTimer_get(void)
{
    UInt64 ticksMS = 0ULL;
    UInt8  ticksUS = 0U;

    /* Load tick variables in a section where interrupts are locked. */
    IRQ_ENTER_ATOMIC_SECTION();
    ticksUS = gTickCounterUs;
    ticksMS = gTickCounterMs;
    IRQ_LEAVE_ATOMIC_SECTION();

    return ticksMS * TIME_CONST + ticksUS * TICKTIMER_RESOLUTION_US;
}

UInt16 TickTimer_getTicks(void)
{
    UInt16 ticksMS;
    UInt8  ticksUS;

    /* Load tick variables in a section where interrupts are locked. */
    IRQ_ENTER_ATOMIC_SECTION();
    ticksUS = gTickCounterUs;
    ticksMS = gTickCounterMs;
    IRQ_LEAVE_ATOMIC_SECTION();

    return ticksMS * (TIME_CONST / TICKTIMER_RESOLUTION_US) + ticksUS;
}

void TickTimer_delay(UInt8 seconds)
{
    while (seconds--)
    {
        _delay_ms(MS_PER_SEC);
    }
}

/* INTERNAL FUNCTIONS *****************************************************************************/
/**
 * Interrupt service routine which is called for every timer overflow.
 */
ISR (TIMER0_COMPA_vect)  /* Timer0 COMPA match interrupt. */
{
    /* Note: gTickCounterUs is modified during interrupts and therefore volatile.
     *       It is best practice to minimize accesses to volatile data as these need to
     *       be loaded/stored to memory on each access. We therefore copy it to a local,
     *       modifiy the local and only write it back when we are done.
     */
    UInt8 tick = gTickCounterUs;

    ++tick;

    if (tick >= (TIME_CONST / TICKTIMER_RESOLUTION_US))
    {
        tick = 0U;
        ++gTickCounterMs;

        if (NULL != gTickCallback)
        {
            gTickCallback();
        }
    }

    gTickCounterUs = tick;
}
