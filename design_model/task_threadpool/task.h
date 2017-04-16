#ifndef __TASK_H__
#define __TASK_H__

//task handle function
typedef bool(*task_handle_f)(void *);

//task meet error when handle/call task interface. 
typedef void(*task_error_f)(void *);

//task will be canceled when threadpool force to exit.
typedef void(*task_cancel_f)(void *);

#pragma pack(4)
typedef struct task_info_s {
    task_handle_f  handle_fun;
    task_error_f  error_fun;
    task_cancel_f cancel_fun;
    void *data;
}task_info_t;
#pragma pack()


#endif //__TASK_H__
