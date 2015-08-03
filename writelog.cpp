#include "writelog.h"
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <sys/stat.h> 
#include <sys/types.h> 
#include <sys/prctl.h>




template <typename TYPE, void (TYPE::*_Run)()>

void* _thread_t(void *param)
{
	TYPE* This = (TYPE*) param;
	This->_Run();
	return NULL;
}

write_log::write_log()
{

}



void write_log::redirect_log()
{
#define LOG_PATH  "./log"
		if( access(LOG_PATH, 0) != 0 )
		{
			int ret = _mkdir_(LOG_PATH);
			if(ret == -1)
			  exit(-1);
		}

		char log_time_buf[64];
	    time_t log_time = time(NULL);
	    struct tm ltm = {0};
		localtime_r(&log_time, &ltm);
		strftime(log_time_buf, sizeof(log_time_buf), "%Y%m%d", &ltm);
		char process_name[64] = {0};
		get_process_name(process_name);
		
		std::string log_path;
		log_path += LOG_PATH;
		log_path += "/";
		log_path += process_name;
		log_path += "_";
		log_path += log_time_buf;
		log_path += ".txt";
		
		
			
		freopen(log_path.c_str(), "a+", stdout);
		freopen(log_path.c_str(), "a+", stderr);
	
}


void write_log::start(void)
{
	redirect_log();
	pthread_create(&tid, NULL, _thread_t<write_log, &write_log::_Run>, this);
	pthread_detach(tid);
}

void write_log::_Run(void)
{

	prctl(PR_SET_NAME,"write_log") ;

    for(;;)
    {
		redirect_log();
		sleep(1);
	}
	
}

void write_log::get_process_name(char *name)
{
    char buf[1024] = { 0 };
    int n;
 
    n = readlink("/proc/self/exe" , buf , sizeof(buf));

    buf[n] = '\0';
    for(n=n-1;n>0;n--)
    {
       if(buf[n] == '/')
       break;

    }
    
    strncpy(name, buf + n + 1 , strlen(buf+n) - 1);
}


int write_log::_mkdir_(const char * dir)
{
    if( NULL == dir )
		return -1;
	char *tp_dir = strdup(dir);
	char *pos = tp_dir;

    //skip start comb such as / ./
	if( strncmp(tp_dir, "/", 1) == 0 )
		pos +=1;
	else if (strncmp(tp_dir, "./", 2) == 0)
		pos +=2;

	for(; *pos != '\0'; ++ pos)
	{
		if ( *pos == '/' )
		{
			*pos ='\0';
			mkdir (tp_dir, 0644);
			*pos='/';
		}	
	}
	if( *(pos - 1) != '/')
		mkdir (tp_dir, 0644);
	free (tp_dir);
	return 0 ;
}
