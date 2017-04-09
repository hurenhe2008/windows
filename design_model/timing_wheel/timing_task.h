#ifndef _TIMING_TASK_H_
#define _TIMING_TASK_H_


/* task handle when time touch */
typedef bool(*task_deal_f)(void *);

/* task not finished when exit */
typedef void(*task_nofinish_f)(void *);

/* task error when call task_deal_f */
typedef void(*task_error_f)(void *);


typedef struct timing_task_s {
    task_deal_f     deal_func;  
    task_error_f    error_func;
    task_nofinish_f nofinish_func;
    void*           data;
    unsigned        touch_after_seconds;
}timing_task_t;


#endif //_TIMING_TASK_H_