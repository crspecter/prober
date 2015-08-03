#include "sys_info_parser.h"
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <cstring>
#include "log.h"

void sys_info_parser::init()
{

	config_.parse_file();
	std::string file_sys_config_string = config_.get_string("sys_info", "file_sys", "");


	char *src = (char *)file_sys_config_string.c_str();
	char *file_sys = NULL;
	char *inner_ptr = NULL;
	
	while((file_sys = strtok_r(src, ",", &inner_ptr)) != NULL )
	{
		list_file_sys_point.push_back(file_sys);
		src = NULL;
	}
}


void sys_info_parser::get_sys_info()
{
	calc_cpu_occupy();
	calc_memory_occupy();
	calc_disk_occupy();
}


void sys_info_parser::calc_cpu_occupy()
{
	FILE *cpufd;
	char buffer[256];
	cpu_info[0].init();
	cpu_info[1].init();
	
	if((cpufd = fopen("/proc/stat", "r")) == NULL )
	{
		LOG_ERR("can not open /proc/stat failed");
		exit(1);
	}

	fgets(buffer, 256, cpufd);
	sscanf(buffer, "%*s %lu %lu %lu %lu %lu %lu %lu", &cpu_info[0].cpu_user,
		&cpu_info[0].cpu_nice, &cpu_info[0].cpu_sys, &cpu_info[0].cpu_idle,
		&cpu_info[0].cpu_iowait, &cpu_info[0].cpu_irq, &cpu_info[0].cpu_softirq);

	fclose(cpufd);
	
	uint64_t total_0 = get_cpu_sum(cpu_info[0]);

	

	sleep(2);



	bzero(buffer, 256);

	if((cpufd = fopen("/proc/stat", "r")) == NULL )
	{
		LOG_ERR("can not open /proc/stat failed");
		exit(1);
	}

	fgets(buffer, 256, cpufd);
	sscanf(buffer, "%*s %lu %lu %lu %lu %lu %lu %lu", &cpu_info[1].cpu_user,
		&cpu_info[1].cpu_nice, &cpu_info[1].cpu_sys, &cpu_info[1].cpu_idle,
		&cpu_info[1].cpu_iowait, &cpu_info[1].cpu_irq, &cpu_info[1].cpu_softirq);

	fclose(cpufd);
	
	uint64_t total_1= get_cpu_sum(cpu_info[1]);	
	uint64_t idle = cpu_info[1].cpu_idle - cpu_info[0].cpu_idle;
	
	
	stat_sys.cpu_info.cpu_occupy_rate = 1 - idle/((total_1 - total_0)*1.0);
		
	
}


void sys_info_parser::calc_memory_occupy()
{
	FILE *memfd;
	char buffer[128] = {0};
	
	if((memfd = fopen("/proc/meminfo", "r")) == NULL )
	{
		LOG_ERR("can not open /proc/meminfo failed");
		exit(1);
	}	
	fgets(buffer, 128, memfd);

	sscanf(buffer, "%*s %lu", &stat_sys.mem_info.mem_total);

	bzero(buffer, 128);
	fgets(buffer, 128, memfd);

	uint64_t used = 0;
	sscanf(buffer, "%*s %lu", &used);	
	
	stat_sys.mem_info.mem_used = stat_sys.mem_info.mem_total - used;
	
	fclose(memfd);
	
}


void sys_info_parser::calc_disk_occupy()
{
	const_file_sys_list_it it;
	uint64_t total_disk = 0;
	uint64_t total_free = 0;
	
	for(it = list_file_sys_point.begin(); it!= list_file_sys_point.end(); it++)
	{
		if(statfs(it->c_str(), &disk_info) != 0)
		  LOG_ERR("statfs failed");
		
		//f_bsize 每个块的字节数，f_blocks总块数, f_bfree可用的块数
		total_disk += disk_info.f_bsize * disk_info.f_blocks;
		total_free += disk_info.f_bsize * disk_info.f_bfree;
	}	
	stat_sys.disk_info.disk_total = total_disk/1024/1024;
	stat_sys.disk_info.disk_used  = (total_disk-total_free)/1024/1024;
}
