
#ifndef TASK_MANAGER_H
#define	TASK_MANAGER_H

#include <stdbool.h>

#define TASK_TIMEOUT        10000;

void TskMgrClearTickEvent(void);
bool TskMgrGetTickEvent(void);
void TskMgrExecuteTask(void);
void TskMgrtickCallback(void);
void TskMgrInit(void);

#endif	/* TASK_MANAGER_H */