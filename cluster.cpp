#include "cluster.h"
#include <cstdlib>
#include <cstdio>
#include <sys/prctl.h>
#include <signal.h>
#include <sys/time.h>
#include "log.h"

#define OUT_PUT_RATE 60
#define TCP_PROTO 6
#define UDP_PROTO 17






template <typename TYPE, void (TYPE::*_Run)()>
void* _thread_t(void *param)
{
	TYPE* This = (TYPE*) param;
	This->_Run();
	return NULL;
}

template <typename TYPE, void (TYPE::*_Run_sys_info)()>
void* _thread_sys_t(void *param)
{
	TYPE* This = (TYPE*) param;
	This->_Run_sys_info();
	return NULL;
}

#define PF_CAP_LEN 1600
cluster::cluster(char* name):device_name(name),circle_user_(20000), 
circle_protocol_(1000),circle_sys_(10),circle_flow_stat_(10),
stat_sys_(NULL),stat_flow_(NULL),stat_parser_(OUT_PUT_RATE)

{
	ring = pfring_open(device_name.c_str(), PF_CAP_LEN, PF_RING_PROMISC | PF_RING_TIMESTAMP);
	if(NULL == ring)
	{
		printf("open %s device failed\n", device_name.c_str());
		exit(1);
	}
	time_out_put = 0; 
}

void cluster::StartUp()
{

	sql_server_.init(&circle_user_, &circle_protocol_, &circle_sys_, &circle_flow_stat_);
	sql_server_.start_up();

	sys_info_parser_.init();
//解析端口对应的协议名称
	xml_parser.startup();	

	pthread_create(&tid, NULL, _thread_t<cluster, &cluster::_Run>, this);
	pthread_detach(tid);
	
	pthread_create(&tid_sys_info, NULL, _thread_sys_t<cluster, &cluster::_Run_sys_info>, this);
	pthread_detach(tid_sys_info);
}


void cluster::_Run()
{
	std::string thread_name = device_name + "_cluster";
	prctl(PR_SET_NAME, thread_name.c_str());

	

	int len = 2048;
    u_char *buffer = new u_char[len];
    struct pfring_pkthdr *hdr = (pfring_pkthdr *)buffer;
    u_char* lpPkt = buffer + sizeof(struct pfring_pkthdr);

    int waitable = 1;           /* 0-->non-blocking, otherwise blocking*/
    //u_int8_t level = 4;         /* 2 of OSI-7*/
    //u_int8_t addtimestamp = 1;  /* 1 --> add timetamp */
    //u_int8_t addhash = 0;       /* 0 --> don't add hash */
	
	pfring_enable_ring(ring);
	//解码的结果集
	_nic_stat_session_tuple tuple;
	for(;;)
	{
		int ret = pfring_recv(ring, (u_char**)&lpPkt, PF_CAP_LEN, hdr, waitable);
		if( ret > 0 )
		{
			decode((uint8_t *)lpPkt, hdr->caplen);
			
			tuple.init();
			
			fill_tuple(tuple, hdr->caplen);
			
			cluster_stat_flow(&tuple);
		}
	}
	
}

bool cluster::decode(uint8_t* pData, uint32_t uiLen)
{

	bool ret = false;
	
	if((ret = eth_decoder_.Decode(pData, uiLen)) != true) 
		return ret;
			
	if((ret = ip_decoder_.Decode(eth_decoder_.payload_data_offset, eth_decoder_.payload_data_len)) != true ) 
		return ret;

	if(ip_decoder_.protocol == TCP_PROTO)
	{
		ret = tcp_decoder_.Decoder(ip_decoder_.payload_data_offset, ip_decoder_.payload_data_len);
	}
	else if (ip_decoder_.protocol == UDP_PROTO)
	{
		ret = udp_decoder_.Decode(ip_decoder_.payload_data_offset, ip_decoder_.payload_data_len);
	}
	return ret;
}

void cluster::fill_tuple(_nic_stat_session_tuple& tuple, uint32_t caplen)
{
	tuple.dst_ip_ = ip_decoder_.dst_ip;
	tuple.src_ip_ = ip_decoder_.src_ip;
	tuple.version = ip_decoder_.version;
	tuple.flow_ = caplen;
	tuple.protocol_ = ip_decoder_.protocol;

	if(tuple.protocol_ == TCP_PROTO)
	{
		tuple.dst_port_ = udp_decoder_.m_usDstPort;
		tuple.src_port_ = udp_decoder_.m_usSrcPort;
	}
	else if(tuple.protocol_ == UDP_PROTO)
	{
		tuple.dst_port_ = tcp_decoder_.dport;
		tuple.src_port_ = tcp_decoder_.sport;		
	}
	
}

//get sys info per second
void cluster::_Run_sys_info()
{
	std::string thread_name = device_name + "_sys_info";
	prctl(PR_SET_NAME, thread_name.c_str());

	for(;;)
	{
		sys_info_parser_.get_sys_info();
		sleep(1);
	}
}


void cluster::cluster_stat_flow(_nic_stat_session_tuple* tuple)
{
	
	//first
	if(time_out_put == 0)
	{
		time_out_put  = time(NULL) + OUT_PUT_RATE;
		//stat ip 
		cluster_stat_user(tuple);
		//stat protocol
		cluster_stat_protocol(tuple);
		//stat flow
		cluster_stat_total_flow(tuple);
		
	}

	else if(time_out_put > time(NULL))
	{
		//stat ip 
		cluster_stat_user(tuple);
		//stat protocol
		cluster_stat_protocol( tuple);
		//stat flow
		cluster_stat_total_flow(tuple);
	}
	
	//time out send result
	else
	{
		cluster_out_put();
		time_out_put = time(NULL) + OUT_PUT_RATE;
	}


	
}


void cluster::cluster_stat_total_flow(_nic_stat_session_tuple* tuple)
{
	stat_parser_.stat_flow(tuple->flow_);
}


void cluster::cluster_stat_user(_nic_stat_session_tuple* tuple)
{

	session_key sk;
	get_session_key(sk, tuple);
	process_session(sk, tuple);
	
}


void cluster::process_session(session_key& sk, _nic_stat_session_tuple* tuple)
{
		//对ip对流量进行统计
		get_session_key(sk, tuple);
		session_key_map_it it;
		if((it = map_user_.find(sk)) == map_user_.end())
		{
			map_user_.insert(std::pair<session_key, _nic_stat_user>(sk, _nic_stat_user(tuple->src_ip_, tuple->dst_ip_, time(NULL), tuple->flow_)));
		}
		else
		{
			it->second.current_flow_ += tuple->flow_;
		}
}


void cluster::get_session_key(session_key &sk, _nic_stat_session_tuple* tuple)
{
	sk.ip_version = tuple->version;
	//sk.src_ip = ip_dec->src_ip;
	//sk.dst_ip = ip_dec->dst_ip;
	sk.ip_pair = tuple->src_ip_ ^ tuple->dst_ip_;
	sk.protocol = tuple->protocol_;	
}


void cluster::get_protocol_key(_nic_stat_session_tuple* tuple, protocol_key &pk)
{
	
	if(tuple->protocol_ == 6 || tuple->protocol_ == 17)
		xml_parser.get_key_port(pk.key_port_protocol, tuple);
	else
		pk.key_port_protocol = tuple->protocol_;
}

	
void cluster::cluster_stat_protocol(_nic_stat_session_tuple* tuple)
{
	protocol_key pk;
	
	get_protocol_key(tuple, pk);
	
	process_protocol(tuple, pk);
}

void cluster::process_protocol(_nic_stat_session_tuple* tuple, protocol_key& pk)
{
	protocol_key_map_it it;
	if((it = map_protocol_.find(pk)) == map_protocol_.end())
	{
		_nic_stat_protocol stat_proc;
		stat_proc.current_flow_ = tuple->flow_;
		//stat_proc.time_ = time(NULL);
		xml_parser.get_protocol_name(pk.key_port_protocol, &stat_proc);
		map_protocol_.insert(std::pair<protocol_key, _nic_stat_protocol>(pk, stat_proc));
	}
	else
	{
		it->second.current_flow_ += tuple->flow_;
	}	
}

void cluster::cluster_out_put()
{	
	out_put_user();
	out_put_protocol();
	out_put_sys();
	out_put_total_flow_stat();
	sql_server_.print_statistics(device_name);
}

void cluster::out_put_user()
{
	session_key_map_it it;
	for(it = map_user_.begin(); it != map_user_.end(); it++)
	{
		circle_user_.push_back(it->second);
	}
	
	map_user_.clear();
}

void cluster::out_put_protocol()
{
	protocol_key_map_it it;
	
	for(it = map_protocol_.begin(); it != map_protocol_.end(); it++)
	{
		circle_protocol_.push_back(it->second);
	}
	
	map_protocol_.clear();
}


void cluster::out_put_sys()
{
	stat_sys_ = sys_info_parser_.get_sys_state();
	if(NULL == stat_sys_)
	{
		LOG_ERR("get system info failed");
		exit(1);
	}
	circle_sys_.push_back(*stat_sys_);
}

void cluster::out_put_total_flow_stat()
{
	stat_flow_ = stat_parser_.get_total_flow();
	if(NULL == stat_sys_)
	{
		LOG_ERR("get flow stat info failed");
		exit(1);
	}
	circle_flow_stat_.push_back(*stat_flow_);
	stat_parser_.stat_reset();
}

