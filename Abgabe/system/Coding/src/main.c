/***************************************************************************************************
  (c) NewTec GmbH 2019   -   www.newtec.de
  $URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/trunk/system/50_Implementierung/Projekte/Linienfolger/20_Beistellung/Coding/src/main.c $
***************************************************************************************************/
/**
@addtogroup App
@{
@file       main.c

Main function for pololu Line follower.

@version    %$Id: main.c 281 2024-01-12 12:09:03Z leglaz $
* @}
***************************************************************************************************/

/* INCLUDES ***************************************************************************************/
#include "app/MainTask.h"
#include "Common/Debug.h"
#include "hal/Hal.h"
#include "os/ErrorHandler.h"
#include "os/Scheduler.h"
#include "os/Os.h"
#include "Service/Service.h"

int main(void)
{
    /* Init everything. */
    Hal_init();
    OS_init();
    Service_init(); /* Service_init must be called after OS_init()! */
    MainTask_init();

    /* Run Scheduler. */
    Scheduler_execute();

    /* Should not be reachable. */
    Debug_showMsg("ERR:SchedEnd");
    ErrorHandler_halt(ERRORHANDLER_MAIN_SCHEDULER_EXIT);
}
