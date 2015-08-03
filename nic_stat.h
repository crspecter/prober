#ifndef __NIC_STAT_H__
#define __NIC_STAT_H__
#include <cstdint>
#include <string>
//统计每个源ip对应的流量
struct _nic_stat_user
{
	_nic_stat_user()
	{
	
		src_ip_ = 0;
		dst_ip_ = 0;
		time_ = 0;
		current_flow_ = 0;

	}

	_nic_stat_user(uint32_t srcip, uint32_t dstip, uint64_t time, uint64_t flow):
		src_ip_(srcip), dst_ip_(dstip),
		time_(time),current_flow_(flow)
	{

	}

	void init()
	{
		src_ip_ = 0;
		dst_ip_ = 0;
		time_ = 0;
		current_flow_ = 0;
	}
	uint32_t	src_ip_ ;
	uint32_t	dst_ip_ ;
	uint64_t 	time_;
	uint64_t 	current_flow_;

	
};


//统计传输层协议每项协议对应的流量
struct _nic_stat_protocol
{
	_nic_stat_protocol()
	{

		time_ = 0;
		
		current_flow_ = 0;
		
	}
	_nic_stat_protocol(std::string& protocol, uint64_t time, uint64_t flow):
		
		protocol_(protocol),
		time_(time),current_flow_(flow)
	{

	}	
	void init()
	{

		time_ = 0;
		
		current_flow_ = 0;
		
	}


	std::string protocol_;
	uint64_t    time_;
	uint64_t    current_flow_;
	

};



struct _nic_stat_session_tuple
{
	 _nic_stat_session_tuple()
	 {
		src_ip_ = 0 ;
		dst_ip_ = 0;
		src_port_ = 0;
		dst_port_ = 0;
	    protocol_ = 0;
		flow_ = 0;
	 }
	void init()
	{
		src_ip_ = 0 ;
		dst_ip_ = 0;
		src_port_ = 0;
		dst_port_ = 0;
	    protocol_ = 0;
		flow_ = 0;
	}
	uint8_t     version;
	uint32_t	src_ip_ ;
	uint32_t	dst_ip_ ;
	uint16_t	src_port_;
	uint16_t	dst_port_;
	uint32_t    protocol_;
	uint64_t    flow_;
};

//统计一定时间段内的流量

struct _nic_flow_stat
{
	_nic_flow_stat()
	{
		time = 0;
		flow_byte = 0;
		flow_rate = 0; 
		
	}
	void init()
	{
		time = 0;
		flow_byte = 0;
		flow_rate = 0; 
	}
	

	uint64_t time;
	uint64_t flow_byte ;
	double flow_rate ; 

};

#endif
