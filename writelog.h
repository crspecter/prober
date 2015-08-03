#ifndef __WRITELOG_H__
#define __WRITELOG_H__

#include <pthread.h>
#include <stdio.h>

class write_log
{
private:
	void get_process_name(char *name);
	int _mkdir_(const char * dir);
	void redirect_log();
public:
	write_log();
	
	void _Run(void);
	void start(void);
private:
	pthread_t tid;
	
};

#endif