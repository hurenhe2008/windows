#ifndef _TIMING_TASK_H_
#define _TIMING_TASK_H_


/* task handle when time touch */
typedef bool(*task_touch_f)(void *);

/* task cancel with exit */
typedef void(*task_cancel_f)(void *);

/* task error when call task_touch_f */
typedef void(*task_error_f)(void *);


#pragma pack(4)

typedef struct timing_task_s {
    task_touch_f    touch_func;
    task_error_f    error_func;
    task_cancel_f   cancel_func;
    void*           data;
    union {
        int         touch_after_seconds;   
        int         touch_after_cycles;
    };  
}timing_task_t;

#pragma pack()


#endif //_TIMING_TASK_H_



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
