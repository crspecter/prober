#ifndef __DAEMON_H__
#define __DAEMON_H__

#include "writelog.h"

class init_dae
{
public:
	FILE* check_process();
	void parse_cmd_line(int argc, char *argv[]);
	void write_pid_log(FILE *fp);
private:
	void init_daemon();
	int status_;
	write_log logger;
};

#endif