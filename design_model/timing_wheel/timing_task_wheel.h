#ifndef _TIME_TASK_WHEEL_H_
#define _TIME_TASK_WHEEL_H_

#include <list>
#include <mutex>
#include <memory>

#include "time_task.h"
#include "thread.h"

class TimeTaskWheel : public Thread
{
public:
	static TimeTaskWheel* instance();
	~TimeTaskWheel();

	bool insert_task(time_task_t *);

private:
	TimeTaskWheel();

	bool init();
	bool uninit();
	void handle_time_tasks();
	void cancel_time_tasks();

	virtual unsigned run();

private:
	static std::mutex ms_mutex;
	static std::auto_ptr<TimeTaskWheel> ms_self;  //use auto_ptr to auto destroy.

	unsigned m_curr_pos;
	HANDLE   m_wait_event;

#define PERIOD_TIME      1000    //ms
#define TASK_QUEUE_SIZE  3600    //s, one hour time wheel
	typedef std::list<time_task_t> task_queue_t;
	task_queue_t m_task_queue[TASK_QUEUE_SIZE];

	//disable assign operator and copy.
	TimeTaskWheel(const TimeTaskWheel &) = delete;
	TimeTaskWheel& operator=(const TimeTaskWheel&) = delete;
};

#endif //_TIME_TASK_WHEEL_H_
