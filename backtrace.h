#ifndef __BACK_TRACE_H__
#define __BACK_TRACE_H__

#include <string>
#include <fstream>
extern	void writeStackTrace(int sig);
extern  void setbacktrace(int sig);
/*
class backtrace_info
{

public:
	void getbacktrace();
	void writeStackTrace();
private:
	
	std::string stack_info_;
	std::ofstream fs_;
};
*/
#endif