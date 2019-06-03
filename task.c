/***********************************************************************************************************************
* File Name    : task.c
* Description  : This file  for EEL_OS.
* Creation Date: 2017/6/23
* Editer:WangTao
* Version:(v1.00)
*(C)Copyright TAO.WANG 2017 All rights reserved
***********************************************************************************************************************/

#include "task.h"

#define TASK_RUN    (1)
#define TASK_OFF   (0)

#define EEL_ON    (1)
#define EEL_OFF   (0)



typedef  struct
{
	void ( * pTask)(void); /* must be a 'void (void)' function */
	uint8_t task_ID;
	uint8_t Run;			/*  TASK_RUN mean's task is running */
} sTask;

uint8_t Error_code_G;
sTask SCH_TCB[TCB_MAX];
uint8_t EEL_state;

uint8_t processed_f;
/***********************************************************************************************************************
* Function Name: EEL_Delete_Task()
* Description  : delete now step and check TCB to start next step
* Arguments    : 
* Return Value : None
***********************************************************************************************************************/
void EEL_Delete_Task(EEL_ID *me)
{
	uint8_t work;
	/*  out off rang SDH_TCB  */
	if(me->index >= TCB_MAX)
		return;
	/*  is not the right task  */
	if(me->task_ID != SCH_TCB[me->index].task_ID)
		return;
	if(me->task_ID == 0)
		return;	
	/*  task don't run  */
	if(SCH_TCB[me->index].Run == TASK_OFF)/*   */
		return;
		
	SCH_TCB[me->index].pTask		= 0;
	SCH_TCB[me->index].task_ID		= 0;
	SCH_TCB[me->index].Run			= 0;
		
	work = me->index;
	me->index = 0;
	me->task_ID = 0;
	
	while((SCH_TCB[work].pTask == 0) && (work < (TCB_MAX-1))){
		work++;/* find out the next funcion   */
	}
	if(work >= TCB_MAX){      /* the last so eel finish */
		EEL_state = EEL_OFF;
		return;
	}
	if(SCH_TCB[work].pTask != 0){
		SCH_TCB[work].Run = TASK_RUN;
		EEL_state = EEL_ON;
		processed_f = 0;
	}else{
		EEL_state = EEL_OFF;
	}
}

/***********************************************************************************************************************
* Function Name: EEL_Add_Task()
* Description  : register step's function
* Arguments    : 
* Return Value : None
***********************************************************************************************************************/

void EEL_Add_Task(void ( * pFunction)(),EEL_ID *me)
{
	if(me->index >= TCB_MAX)
		return;
	SCH_TCB[me->index].pTask		= pFunction;
	SCH_TCB[me->index].task_ID		= me->task_ID;
	SCH_TCB[me->index].Run			= TASK_OFF;
}



/***********************************************************************************************************************
* Function Name: EEL_Init()
* Description  : clear TCB
* Arguments    : 
* Return Value : None
***********************************************************************************************************************/

void EEL_Init(void)
{
	uint8_t i;

	for(i = 0; i < TCB_MAX; i++){
		SCH_TCB[i].pTask		= 0;
		SCH_TCB[i].task_ID		= 0;
		SCH_TCB[i].Run			= 0;
	}
	Error_code_G = 0;
	EEL_state = 0；
}

/***********************************************************************************************************************
* Function Name: EEL_Start_Task()
* Description  : start EEL moving
* Arguments    : 
* Return Value : None
***********************************************************************************************************************/

void EEL_Start_Task(void)
{
	processed_f = 0;
	EEL_state = EEL_ON;
	SCH_TCB[0].Run	= TASK_RUN;	
}



/***********************************************************************************************************************
* Function Name: EEL_Task_Scheduler()
* Description  : when use EEL_Delete_Task() kill one step,
* Arguments    : 
* Return Value : None
***********************************************************************************************************************/

void EEL_Task_Scheduler(void) 
{
	uint8_t index;
	if(EEL_state == EEL_OFF)
		return;
	for (index = 0; index < TCB_MAX; index++){
		if (SCH_TCB[index].Run == TASK_RUN) {
			if(processed_f == 0){/*  no pressed just pressed once */
				processed_f = 1;  /*   pressed  */
				if(SCH_TCB[index].pTask != 0){
					(*SCH_TCB[index].pTask)();
				}
			}
		}
	}
}
