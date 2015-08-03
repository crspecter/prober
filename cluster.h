#ifndef __CLUSTER_H__
#define __CLUSTER_H__


#include "session.h"
#include <string>
#include <pthread.h>
#include "circle_buffer.h"
#include "pfring.h"
#include <ctime>
#include "ip_decoder.h"
#include "eth_decoder.h"
#include "tcp_decoder.h"
#include "udp_decoder.h"
#include "sqlserver.h"
#include "sys_info_struct.h"
#include "sys_info_parser.h"
#include "flow_stat_parser.h"
#include "nic_stat.h"
#include "protocol_xmlparser.h"





class cluster
{


	
public:
	cluster(char* name);
	void _Run();
	
	void _Run_sys_info();
	
	void StartUp();
	
private:

	bool decode(uint8_t* pData, uint32_t uiLen);

	void fill_tuple(_nic_stat_session_tuple& tuple, uint32_t caplen);
	
	void cluster_stat_flow(_nic_stat_session_tuple* tuple);
	
	void cluster_stat_total_flow(_nic_stat_session_tuple* tuple);
	

	void cluster_stat_user(_nic_stat_session_tuple* tuple);

	void cluster_stat_protocol(_nic_stat_session_tuple* tuple);

	void cluster_out_put();

	
	//ip 流量统计 输出
	void out_put_user();
	//protocol 流量统计 输出
	void out_put_protocol();
	//服务器设备占用统计 输出
	void out_put_sys();
	//总流量统计输出
	void out_put_total_flow_stat();

private:
	
	void get_session_key(session_key &sk, _nic_stat_session_tuple* tuple);

    void process_session(session_key &sk, _nic_stat_session_tuple* tuple);

	void get_protocol_key(_nic_stat_session_tuple* tuple, protocol_key &pk);
		
	void process_protocol(_nic_stat_session_tuple* tuple, protocol_key& pk);
		
private:
	
	session_key_map   map_user_;
	protocol_key_map   map_protocol_;

private:
	eth_decoder eth_decoder_;
	ip_decoder  ip_decoder_;
	tcp_decoder tcp_decoder_;
	udp_decoder udp_decoder_;

	
	pthread_t tid, tid_sys_info;
	std::string device_name;
	pfring *ring;

	uint64_t time_out_put;
	
	circle_buffer<_nic_stat_user>  circle_user_;
	circle_buffer<_nic_stat_protocol>  circle_protocol_;
	circle_buffer<_nic_stat_sys> circle_sys_;
	circle_buffer<_nic_flow_stat> circle_flow_stat_;

	_nic_stat_sys  *stat_sys_;
	_nic_flow_stat *stat_flow_;
	
	//接受circle数据写入数据对象
	sql_server_operator sql_server_;
	//获取系统使用情况数据对象
	sys_info_parser sys_info_parser_;
	//流量统计计算对象
	flow_stat_parser  stat_parser_;

//xml解析器
	protocol_xml_parser xml_parser;
};

#endif