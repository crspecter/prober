#ifndef __CPU_INFO_PARSER_H__
#define __CPU_INFO_PARSER_H__

#include <sys/statfs.h>  
#include "sys_info_struct.h"
#include "config.h"
#include <list>


class sys_info_parser
{
typedef std::list<std::string>::const_iterator  const_file_sys_list_it;
public:
	
	void init();

	void get_sys_info();
	
	_nic_stat_sys*  get_sys_state() {return &stat_sys;}
	
private:
	void calc_cpu_occupy();
	uint64_t get_cpu_sum(_cpu_occupy_info &info)
	{
		return info.cpu_user + info.cpu_nice + info.cpu_sys + info.cpu_idle + info.cpu_iowait
			 + info.cpu_irq + info.cpu_softirq;
	}

	void calc_memory_occupy();
	void calc_disk_occupy();

private:
	Config_parser config_;

private:
	//计算结果集
	_nic_stat_sys  			stat_sys;
private:
	//读取2次cpu信息做差值计算
	
	_cpu_occupy_info 		cpu_info[2];
	//统计磁盘信息
	struct statfs  			disk_info;
private:
	std::list<std::string>  list_file_sys_point;
};

#endif
