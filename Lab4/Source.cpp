#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <Windows.h>
#include <string.h>

typedef struct {
	char pTask[100];
	uint32_t Delay;
	uint32_t Period;
	uint8_t RunMe;
	uint32_t TaskID;
} sTask;

#define SCH_MAX_TASKS 	40

typedef struct {
	uint32_t time_remain;
	sTask task;
} Scheduler_index;

Scheduler_index scheduler[SCH_MAX_TASKS + 1];
int scheduler_size = 0;

void slide(int index) {
	for (int i = scheduler_size; i > index; i--) {
		scheduler[i] = scheduler[i - 1];
	}
	scheduler_size++;
}

void reslide(void) {
	for (int i = 0; i < SCH_MAX_TASKS - 1; i++) {
		scheduler[i] = scheduler[i + 1];
	}
	scheduler_size--;
}

void scheduler_add(int delay, sTask input_task) {
	if (scheduler_size == 0) {
		scheduler_size = 1;
		scheduler[0].time_remain = delay;
		scheduler[0].task = input_task;
	}
	else {
		int index = 0;
		int time = delay - scheduler[0].time_remain;
		if (time < 0) {
			slide(0);
			scheduler[0].time_remain = delay;
			scheduler[0].task = input_task;
			scheduler[1].time_remain -= delay;
			return;
		}
		while (index < SCH_MAX_TASKS) {

			if (scheduler_size - 1 > index) {
				index++;
				time -= scheduler[index].time_remain;
				if (time < 0) {
					time += scheduler[index].time_remain;
					slide(index);
					scheduler[index].task = input_task;
					scheduler[index].time_remain = time;
					scheduler[index + 1].time_remain -= time;
					return;
				}
			}
			else {
				scheduler_size++;
				index++;
				scheduler[index].task = input_task;
				scheduler[index].time_remain = time;
				break;

			}
		}
	}
}

int scheduler_get_time(void) {
	return scheduler[0].time_remain;
}

void print_sch(void) {
	printf(" =========== print map :%d ============ \n", scheduler_get_time());
	for (int i = 0; i < scheduler_size; i++) {
		printf("%d : %s	\ntime remaning: %d \n\n", i, scheduler[i].task.pTask, scheduler[i].time_remain);
	}
	printf(" =========== end map ============ \n");
}

sTask scheduler_Dispatcher(void) {
	sTask tmp_task = scheduler[0].task;
	reslide();
	if (tmp_task.Period != 0) scheduler_add(tmp_task.Period, tmp_task);
	return tmp_task;
}







int main() {
	sTask task1, task2, task3, task4, task5, taskadd;
	strcpy_s(task1.pTask, "1 one");
	strcpy_s(task2.pTask, "2 two");
	strcpy_s(task3.pTask, "3 three");
	strcpy_s(task4.pTask, "4 four");
	strcpy_s(task5.pTask, "5 five");
	strcpy_s(taskadd.pTask, "task add");

	task1.Period = 500;
	task2.Period = 1000;
	task3.Period = 1500;
	task4.Period = 2000;
	task5.Period = 2500;

	scheduler_add(0, task1); 
	scheduler_add(0, task2);
	scheduler_add(0, task3);
	scheduler_add(0, task4);
	scheduler_add(0, task5);

	print_sch();


	for (int i = 0; i < 10; i++) {
		do {
			char name[100];
			strcpy_s(name, scheduler_Dispatcher().pTask);
			printf(" Run %s \n", name);

		} while (scheduler_get_time() == 0);
		
		print_sch();
		Sleep(500);
	}
	


	return 0;
}
