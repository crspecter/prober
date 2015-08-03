#ifndef __FLOW_STAT_PARSER_H__
#define __FLOW_STAT_PARSER_H__
#include "nic_stat.h"

class flow_stat_parser
{
public:
	flow_stat_parser(int rate):rate_(rate){ stat_.init(); }
	void stat_flow(uint32_t len) { stat_.flow_byte += len;}
	
	void stat_reset(){	stat_.init();	}
	_nic_flow_stat* get_total_flow(){	

		stat_.flow_rate = stat_.flow_byte/ 1024/ 1024 / (rate_*1.0);
		return &stat_;	
	}
	
	int rate_;
	_nic_flow_stat stat_;
};

#endif