#ifndef __SYS_INFO_STRUCT_H__
#define __SYS_INFO_STRUCT_H__
#include <cstdint>


/*
	cpuʱ����� user nice sys idle iowait irq softirq 
	cpu����ʱ�� idle

	���㷽��:����1s��cpuʱ��仯��
*/
struct _cpu_occupy_info
{
	void init()
	{

		 cpu_user = 0;
		 cpu_nice = 0;
		 cpu_sys = 0;
		 cpu_idle = 0;
		 cpu_iowait = 0;
		 cpu_irq = 0;
		 cpu_softirq = 0;
	}

	uint64_t cpu_user;
	uint64_t cpu_nice;
	uint64_t cpu_sys;
	uint64_t cpu_idle;
	uint64_t cpu_iowait;
	uint64_t cpu_irq;
	uint64_t cpu_softirq;
};

//ʹ��_cpu_occupy_info����Ϣ��д���
struct _cpu_occupy_result
{
	_cpu_occupy_result()
	{
		
		cpu_occupy_rate = 0;
	}
	void init()
	{

		cpu_occupy_rate = 0;
	}
	char name[16];
	double cpu_occupy_rate;
};


struct _sys_mem_result
{
	_sys_mem_result()
	{
		time = 0;
		mem_total = 0;	
		mem_used = 0;
	}
void init()
	{
		time = 0;
		mem_total = 0;	
		mem_used = 0;
	}
	uint64_t time;
	uint64_t mem_total;
	uint64_t mem_used;
};


struct _sys_disk_result
{
	_sys_disk_result()
	{
		time = 0;
		disk_total = 0;	
		disk_used = 0;
	}
void init()
	{
		time = 0;
		disk_total = 0;	
		disk_used = 0;
	}
	uint64_t time;
	uint64_t disk_total;
	uint64_t disk_used;
};


//��ȡϵͳcpu memory diskռ�����
typedef struct _nic_stat_sys
{
	_nic_stat_sys()
	{
		time = 0;
	 	cpu_info.init();	
		mem_info.init();
		disk_info.init();
	}
	void init()
	{
		time = 0;
	 	cpu_info.init();	
		mem_info.init();
		disk_info.init();
	}


	uint64_t time;
	_cpu_occupy_result cpu_info;
	_sys_mem_result    mem_info;
	_sys_disk_result   disk_info;
}st_nic_stat_sys;
#endif