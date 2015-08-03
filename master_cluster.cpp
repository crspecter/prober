
#include "master_cluster.h"
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include "log.h"
void master_cluster::start_up()
{

	config_.parse_file();
	int dev_num = config_.get_int("cluster", "cluster_num", 1);
	if(dev_num > 16)
	{
		printf("%d device set,device number must lower than 16\n" , dev_num);
		exit(1);
	}
		
	std::string dev_list = config_.get_string("cluster", "device", "eth0");

	char *src = (char*)dev_list.c_str();
	char *dev = NULL;
	char *inner_ptr = NULL;
	while((dev = strtok_r(src, ",", &inner_ptr)) != NULL )
	{
		cluster* pcluster = new cluster(dev);
		cluster_array_.push_back(pcluster);
		src = NULL;
	}

	if(cluster_array_.empty())
	{
		printf("no cluster create\n");
		exit(1);
	}
	
	for(int i = 0; i < cluster_array_.size(); i++)
	{
		cluster_array_[i]->StartUp();
	}
	LOG_INFO("nic_stat start work...");
	
}