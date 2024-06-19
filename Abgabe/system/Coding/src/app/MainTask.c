/***************************************************************************************************
  (c) NewTec GmbH 2019   -   www.newtec.de
  $URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/trunk/system/50_Implementierung/Projekte/Linienfolger/20_Beistellung/Delivery/Beistellung_r300/Coding/src/app/MainTask.c $

  (c) Team 🏁~~ ō͡≡o\ (Maurice Ott, Simon Walderich, Thorben Päpke) 2024
***************************************************************************************************/
/**
@addtogroup App
@{
@file       MainTask.c

Main Application task.
For a detailed description see the detailed description in @ref MainTask.h.

@version    %$Id: MainTask.c 300 2024-02-01 12:58:40Z leglaz $
* @}
***************************************************************************************************/

/* INCLUDES ***************************************************************************************/
#include "app/MainTask.h"

#include "app/StateHandler.h"
#include "Common/Debug.h"
#include "os/ErrorHandler.h"
#include "os/Scheduler.h"
#include "os/Task.h"
#include "service/Button.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/
/** Internal Task cyclic worker function.
 * @param[in] data task data (ignored).
 */
static void mainTaskWork (void * data);

/* VARIABLES **************************************************************************************/
/** MainTask task structure. */
static Task gMainTask = {0};
#ifdef SHOW_CYCLE_TIMES
    static UInt16 gCycles = 0U; /**< Store the amount of cycles to be able to calculate the average cycle time */
#endif

/* EXTERNAL FUNCTIONS *****************************************************************************/
extern MainTask_Ret MainTask_init(void)
{
    MainTask_Ret result = MAINTASK_RET_INTERNAL_ERROR;

    if (TASK_RET_SUCCESS == Task_init(&gMainTask, mainTaskWork, TASK_STATE_RUNNING, NULL))
    {
        if ( SCHEDULER_RET_SUCCESS != Scheduler_addTask(&gMainTask))
        {
            result = MAINTASK_RET_ADD_TASK_FAIL;
        }
    }
    else
    {
        result = MAINTASK_RET_INIT_TASK_FAIL;
    }

    return result;
}

/* INTERNAL FUNCTIONS *****************************************************************************/
static void mainTaskWork(void * data)
{
    (void) data;
    #ifdef SHOW_CYCLE_TIMES
        /* Show cycle time */
        if (0U == gCycles)
        {
            Debug_showTimeStart();
        }
        if (100U == ++gCycles)
        {
            gCycles = 0U;
            Debug_showTimeStop("100Cyc");
        }
    #endif
    StateHandler_process();
}
