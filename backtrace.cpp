#include "backtrace.h"
#include <execinfo.h>
#include <signal.h>
#include <time.h>

void writeStackTrace(int sig)
{

	char time_buf[64];
	time_t time_ = time(NULL);
	struct tm ltm = {0};
	localtime_r(&time_, &ltm);
	strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", &ltm);
	
	std::ofstream fs_;
	std::string stack_info_;

	fs_.open("Trace.log", std::ios::app);

	fs_<<"===============get signal: " << sig <<". "<< time_buf<<"=============" <<std::endl;

	const int len = 200;
	void* buffer[len];
	int nptrs = ::backtrace(buffer, len);
	char** strings = ::backtrace_symbols(buffer, nptrs);
	if (strings)
	{
	for (int i = 0; i < nptrs; ++i)
	{
	  
	  stack_info_.append(strings[i]);
	  stack_info_.push_back('\n');
	}
	free(strings);
	}

	fs_<<stack_info_;
	fs_.close();
	exit(1);
}


void setbacktrace(int sig)
{

	signal(sig, writeStackTrace);  

}

/*
void backtrace_info::writeStackTrace()
{
  const int len = 200;
  void* buffer[len];
  int nptrs = ::backtrace(buffer, len);
  char** strings = ::backtrace_symbols(buffer, nptrs);
  if (strings)
  {
    for (int i = 0; i < nptrs; ++i)
    {
      // TODO demangle funcion name with abi::__cxa_demangle
      stack_info_.append(strings[i]);
      stack_info_.push_back('\n');
    }
    free(strings);
  }
  fs_.open("Trace.log", std::ios::app);
  fs_<<stack_info_;
  fs_.close();
}
*/
