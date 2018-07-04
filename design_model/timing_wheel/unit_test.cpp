#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>

#include "time_task_wheel.h"

bool test_handle(void *data)
{
	char *pstr = (char *)data;

	printf("test_handle: %s\n", pstr);

	free(data);

	return true;
}

void test_cancel(void *data)
{
	char *pstr = (char *)data;

	printf("test_cancel: %s\n", pstr);

	free(data);
}


int main(int argc, const char *argv[])
{
	for (int i = 0; i < 20; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			time_task_t task;
			task.handle_func = test_handle;
			task.cancel_func = test_cancel;
			task.after_seconds = i;

			char *pstr = (char *)malloc(50);
			sprintf_s(pstr, 50, "id[%d]: task[%d]", i, j);
			task.data = (void *)pstr;

			TimeTaskWheel::instance()->insert_task(&task);
		}	
	}
	
	TimeTaskWheel::instance()->start();

	::Sleep(15000);

	TimeTaskWheel::instance()->stop();

	system("pause");

	return 0;
}
