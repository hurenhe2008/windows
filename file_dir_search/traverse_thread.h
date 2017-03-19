#ifndef _TRANSERVE_THREAD_H_
#define _TRANSERVE_THREAD_H_

#include <Windows.h>

typedef struct traverse_data_s {
    unsigned size;
    char *data;
} traverse_data_t;


bool traverse_start(traverse_data_t *data);

bool traverse_stop();


#endif //_TRANSERVE_THREAD_H_
