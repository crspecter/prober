#ifndef __LOG__
#define __LOG__

#include <stdio.h>
#include <time.h>
#define NONE "\033[m"

#define RED "\033[0;32;31m"
#define LIGHT_RED "\033[1;31m"
#define GREEN "\033[0;32;32m"
#define LIGHT_GREEN "\033[1;32m"
#define BLUE "\033[0;32;34m"
#define LIGHT_BLUE "\033[1;34m"
#define DARY_GRAY "\033[1;30m"
#define CYAN "\033[0;36m"
#define LIGHT_CYAN "\033[1;36m"
#define PURPLE "\033[0;35m"
#define LIGHT_PURPLE "\033[1;35m"
#define BROWN "\033[0;33m"
#define YELLOW "\033[1;33m"
#define LIGHT_GRAY "\033[0;37m"
#define WHITE "\033[1;37m"

#define LOG_ERR(fmt, args...) do{\
	char log_time_buf[64];\
    time_t log_time = time(NULL);\
    struct tm ltm = {0};\
	localtime_r(&log_time, &ltm);\
	strftime(log_time_buf, sizeof(log_time_buf), "%Y-%m-%d %H:%M:%S", &ltm);\
	fprintf(stderr, LIGHT_RED  "[===%s===]%s:%d "fmt"\n"  NONE , log_time_buf, __FILE__, __LINE__, ##args); fflush(stderr);\
	}\
	while(0)

#define LOG_INFO(fmt, args...) do{\
	char log_time_buf[64];\
    time_t log_time = time(NULL);\
    struct tm ltm = {0};\
	localtime_r(&log_time, &ltm);\
	strftime(log_time_buf, sizeof(log_time_buf), "%Y-%m-%d %H:%M:%S", &ltm);\
	fprintf(stdout, LIGHT_GREEN  "[===%s===]%s:%d "fmt"\n"  NONE , log_time_buf, __FILE__, __LINE__, ##args); fflush(stdout);\
	}\
	while(0)

#define LOG_WARN(fmt, args...) do{\
	char log_time_buf[64];\
    time_t log_time = time(NULL);\
    struct tm ltm = {0};\
	localtime_r(&log_time, &ltm);\
	strftime(log_time_buf, sizeof(log_time_buf), "%Y-%m-%d %H:%M:%S", &ltm);\
	fprintf(stdout, YELLOW  "[===%s===]%s:%d "fmt"\n"  NONE , log_time_buf, __FILE__, __LINE__, ##args); fflush(stdout);\
	}\
	while(0)

		



#endif
