#ifndef _TIME_TASK_H_
#define _TIME_TASK_H_


/* task handle function. */
typedef bool(*task_handle_f)(void *);

/* task cancel function. */
typedef void(*task_cancel_f)(void *);

/* task error function. */
typedef void(*task_error_f)(void *);


typedef struct time_task_s
{
	union
	{
		int after_seconds;
		int after_cycles;
	};
	void* data;

	task_handle_f handle_func;
	task_cancel_f cancel_func;
	task_error_f  error_func;

	time_task_s()
	{
		memset(this, 0, sizeof(*this));
	}
} time_task_t;


#endif //_TIME_TASK_H_



/*---------------------------------------------------------------------------------
[Design theory]
循环定时轮：
设计为3600个std::list<Task>的数组，Task保存任务调用函数和数据。
数组内保存一个移动指针， 每隔一秒移动指针在数组内移动一个位置，
如果移动位置超出最大值，则返回0位置。
插入任务： 根据任务触发时间，设置插入数组位置（追加任务）。
比如说触发时间为2500s，则插入位置为（当前指针位置 + 2500 ） % 3600
触发时间为3600s, 则cycle = 1, 插入位置为当前指针位置。
触发时间为3608s, 则cycle = 1, 插入位置为（当前位置 + 8）% 3600
触发时间为7201, 则cycle = 2, 插入位置为（当前位置 + 1）% 3600
1s   1s   1s     ... ... ... ...        1s   1s   1s
+----+----+----+----+----+----+----+----+----+----+----+
--->	0    1    2        ... ... ... ...         3597 3598 3599  --->
----------------------------------------------------------------------------------*/
