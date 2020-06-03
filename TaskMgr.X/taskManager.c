/*
 * File:   taskManager.c
 * Author: toshio
 *
 * Created on April 28, 2020, 11:21 AM
 */

#include <stdint.h>
#include <stdbool.h>
#include "taskManager.h"
#include "tasks.h"

typedef void (*functable_t)(void);

#define EXPAND_AS_ENUMERATION(taskid, tarefa, period)      e##taskid,
#define EXPAND_AS_FUNCTIONTABLE(taskid, tarefa, period)    tarefa,
#define EXPAND_AS_PERIODTABLE(taskid, tarefa, period)      period,

enum {
	TASKS_TABLE(EXPAND_AS_ENUMERATION)
	eTASKS_TOTALS
};
               
functable_t const ScheduledTasks[eTASKS_TOTALS] = {
    TASKS_TABLE(EXPAND_AS_FUNCTIONTABLE)
};
      
int const TasksPeriodCounterDefault[eTASKS_TOTALS] = {
    TASKS_TABLE(EXPAND_AS_PERIODTABLE)
};                                                                  

int TasksPeriodCounter[eTASKS_TOTALS] = {
	TASKS_TABLE(EXPAND_AS_PERIODTABLE)
};

struct TskMgrCtx {
    volatile bool tmrIntGenerated;
    int taskTimeout;
    char isTaskExecuting;
} TskMgrCtx;

void TskMgrClearTickEvent()
{
    TskMgrCtx.tmrIntGenerated = false;
}

bool TskMgrGetTickEvent()
{
    return (TskMgrCtx.tmrIntGenerated);
}

void TskMgrExecuteTask(void)
{
	for (uint8_t i=0; i<eTASKS_TOTALS; i++) {
		//Check if it's time to execute a task
		if ((ScheduledTasks[i] != 0) && (TasksPeriodCounter[i] == 0)) {
			TskMgrCtx.isTaskExecuting = 1;
			TskMgrCtx.taskTimeout = TASK_TIMEOUT;
			ScheduledTasks[i]();  //executes the task
			TskMgrCtx.isTaskExecuting = 0;
			TasksPeriodCounter[i] = TasksPeriodCounterDefault[i];  //reagendamento da tarefa
   		}
	}
}

void TskMgrtickCallback()
{
    TskMgrCtx.tmrIntGenerated = true;
		
    //Refresh "time to execute" of each task        
    for (uint8_t i=0; i<eTASKS_TOTALS; i++) {
        if (TasksPeriodCounter[i] > 0)
            TasksPeriodCounter[i]--;			
    }

    if (TskMgrCtx.isTaskExecuting) {
        TskMgrCtx.taskTimeout--;

        if (!TskMgrCtx.taskTimeout) {
            //Timeout has reached (possibly a task has crashed. Microcontroller must be reset)
            while(1);
        }
    }
}

void TskMgrInit()
{
    TskMgrCtx.tmrIntGenerated = false;
    TskMgrCtx.taskTimeout = TASK_TIMEOUT;
    TskMgrCtx.isTaskExecuting = 0;
}
