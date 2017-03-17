#ifndef _EXCEPT_FILTER_H_
#define _EXCEPT_FILTER_H_

struct _EXCEPTION_POINTERS;

long except_filter(unsigned long, _EXCEPTION_POINTERS *);

#endif //_EXCEPT_FILTER_H_