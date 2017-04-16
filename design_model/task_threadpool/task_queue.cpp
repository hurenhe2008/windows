#include "task_queue.h"
#include <new>
#include <assert.h>

TaskQueue::TaskQueue(unsigned size)
: m_read_pos(0)
, m_write_pos(0)
, m_queue_size(size + 1)  //1 for judge full state
, m_task_queue(nullptr)
{

}

TaskQueue::~TaskQueue()
{
    UnInit();
}

bool TaskQueue::Init()
{
    if (m_queue_size <= 0) {
        return false;
    }

    if (m_task_queue) {
        return true;
    }

    m_task_queue = new(std::nothrow) task_info_t[m_queue_size];
    
    return (m_task_queue) ? true : false;
}

bool TaskQueue::UnInit()
{
    if (m_task_queue) {
        delete[] m_task_queue;
        m_task_queue = nullptr;
    }

    m_read_pos = m_write_pos = m_queue_size = 0;
    return true;
}

bool TaskQueue::Enqueue(const task_info_t *task)
{
    const unsigned nextwritepos =
        (m_write_pos + 1) % m_queue_size;

    if (nextwritepos == m_read_pos) {
        return false;   //queue full
    }

    m_task_queue[m_write_pos] = *task;
    m_write_pos = nextwritepos;

    return true;
}

task_info_t TaskQueue::Dequeue()
{
    if (m_read_pos == m_write_pos) {
        return task_info_t(); //queue empty
    }

    return m_task_queue[m_read_pos++];
}

bool TaskQueue::Full()
{
    return (m_write_pos + 1) % m_queue_size == m_read_pos;
}

bool TaskQueue::Empty()
{
    return m_write_pos == m_read_pos;
}