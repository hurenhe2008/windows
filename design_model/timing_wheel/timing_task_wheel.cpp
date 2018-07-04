#include "time_task_wheel.h"

std::mutex TimeTaskWheel::ms_mutex;
std::auto_ptr<TimeTaskWheel> TimeTaskWheel::ms_self;

TimeTaskWheel* TimeTaskWheel::instance()
{
	if (!ms_self.get()) {

		std::lock_guard<std::mutex> guard(ms_mutex);

		if (!ms_self.get()) { //double check
			
			TimeTaskWheel *tmp = new(std::nothrow)TimeTaskWheel;
			if (!tmp || !tmp->init()) 
			{
				delete tmp;
				tmp = nullptr;
			}

			ms_self.reset(tmp);
		}

	}

	return ms_self.get();
}

TimeTaskWheel::TimeTaskWheel()
	: m_curr_pos(0)
	, m_wait_event(NULL)
{

}

TimeTaskWheel::~TimeTaskWheel()
{
	uninit();
}

bool TimeTaskWheel::init()
{
	//not mutex here, avoid dead mutex.
	m_wait_event = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (NULL == m_wait_event) return false;

	for (int i = 0; i < TASK_QUEUE_SIZE; ++i) 
	{
		m_task_queue[i].clear();
	}

	m_curr_pos = 0;

	return true;
}

bool TimeTaskWheel::uninit()
{
	if (!stop())
	{
		abort(); //stop failed.
		return false;
	}

	std::lock_guard<std::mutex> guard(ms_mutex);

	for (int i = 0; i < TASK_QUEUE_SIZE; ++i) 
	{
		m_task_queue[i].clear();
	}

	m_curr_pos = 0;

	if (m_wait_event)
	{
		CloseHandle(m_wait_event);
		m_wait_event = NULL;
	}

	return true;
}

unsigned TimeTaskWheel::run()
{
	unsigned last_time = 0;
	int interval = 0;
	bool bfirst = true;
	int retval = 0;

	while (is_to_run())   
	{
		if (!bfirst) 
		{
			interval = last_time + PERIOD_TIME - GetTickCount();
			if (interval < 0)    //adjust interval.
			{
				interval = 0;
			}
			last_time += PERIOD_TIME;
		}
		else //init.
		{
			last_time = GetTickCount();
			interval = PERIOD_TIME;
			bfirst = false;
		}

#ifdef _DEBUG 
		printf("current wait time: %d\n", interval);
#endif 

		WaitForSingleObject(m_wait_event, interval);

		handle_time_tasks();
	}
	
	cancel_time_tasks();  //cancel task 

	return retval;
}

void TimeTaskWheel::handle_time_tasks()
{
	std::lock_guard<std::mutex> guard(ms_mutex);

	if (m_curr_pos >= TASK_QUEUE_SIZE) 
	{	
		abort();   /* error happen */
	}

	task_queue_t &queue = m_task_queue[m_curr_pos];
	task_queue_t::iterator it = queue.begin();

#ifdef _DEBUG
	unsigned cnt = 0;
#endif 

	for (; it != queue.end(); ) 
	{
		if (it->after_cycles <= 0) 
		{
#ifdef _DEBUG
			++cnt;
#endif 
			if (it->handle_func && !it->handle_func(it->data)) 
			{
				if (it->error_func) it->error_func(it->data);
			}
			it = queue.erase(it);
		}
		else 
		{
			it->after_cycles -= 1;
			++it;
		}
	}

#ifdef _DEBUG
	printf("position:%d  -->  handle task count: %d\n", m_curr_pos, cnt);
#endif 

	if (m_curr_pos + 1 < TASK_QUEUE_SIZE)
		++m_curr_pos;
	else
		m_curr_pos = 0;
}

void TimeTaskWheel::cancel_time_tasks()
{
	std::lock_guard<std::mutex> guard(ms_mutex);

	for (int i = 0; i < TASK_QUEUE_SIZE; ++i)
	{
		task_queue_t &queue = m_task_queue[i];
		for (auto &it : queue)
		{
			if (it.cancel_func)
			{
				it.cancel_func(it.data);
			}
		}

		m_task_queue[i].clear();
	}
}

bool TimeTaskWheel::insert_task(time_task_t *task)
{
	std::lock_guard<std::mutex> guard(ms_mutex);

	if (m_curr_pos >= TASK_QUEUE_SIZE) 
	{
		abort();   /* error happen */
	}

	unsigned total_seconds = task->after_seconds;

	//calc cycles.
	unsigned cycles = total_seconds / TASK_QUEUE_SIZE;

	//calc position to insert.
	unsigned insert_pos = (m_curr_pos + total_seconds) % TASK_QUEUE_SIZE;

	task->after_cycles = cycles;

	m_task_queue[insert_pos].push_back(*task);

	return true;
}
