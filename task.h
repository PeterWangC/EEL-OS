
/***********************************************************************************************************************
* File Name    : task.h
* Description  : This file  for EEL_OS.
* Creation Date: 2019/6/3
* Editer:WangTao
* Version:(v1.00)
*(C)Copyright TAO.WANG 2019 All rights reserved
***********************************************************************************************************************/
#ifndef TASK_H
#define TASK_H

#include "r_cg_macrodriver.h"

#define TCB_MAX (5) /* Adjust according need */


typedef struct{
	uint8_t task_ID;
	uint8_t index;
}EEL_ID;

void EEL_Init(void);
void EEL_Add_Task(void ( * pFunction)(),EEL_ID *me);
void EEL_Start_Task(void);
void EEL_Delete_Task(EEL_ID *me);
void EEL_Task_Scheduler(void);

#endif
