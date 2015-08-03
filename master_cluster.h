#ifndef __MASTER_CLUSTER_H__
#define __MASTER_CLUSTER_H__

#include <vector>
#include "config.h"
#include "cluster.h"


class master_cluster
{
public:

void start_up();

public:
	Config_parser config_;
	std::vector<cluster*> cluster_array_;
};

#endif