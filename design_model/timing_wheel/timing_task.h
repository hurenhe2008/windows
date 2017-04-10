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
