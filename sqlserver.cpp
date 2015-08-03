#include "sqlserver.h"
#include "log.h"






#define Format_ip(str, ip) do\
{\
	snprintf(str, sizeof(str), "%d.%d.%d.%d", (ip & 0xff000000) >> 24, \
	(ip & 0x00ff0000) >> 16, (ip & 0x0000ff00) >> 8, (ip & 0x000000ff));\
}while(0)

template <typename TYPE, void (TYPE::*_Run)()>
void* _thread_t(void *param)
{
	TYPE* This = (TYPE*) param;
	This->_Run();
	return NULL;
}



sql_server_operator::sql_server_operator():user_count(0),
				protocol_count(0),
				sys_count(0),
				flow_count(0)
	
		   
{


}


void sql_server_operator::init(circle_buffer<_nic_stat_user>* cir_user,
		   circle_buffer<_nic_stat_protocol>* cir_proto, 
		   circle_buffer<_nic_stat_sys>* cir_sys,
		   circle_buffer<_nic_flow_stat>* cir_flow)
{
//解析配置文件，读取数据库连接信息
	config_.parse_file();
	user_name = config_.get_string("db", "username", "");
	password  = config_.get_string("db", "password", "");
	dbname	  = config_.get_string("db", "dbname", "");
	server_host = config_.get_string("db", "server_host", "");	
	
//循环缓冲区的指针初始化，使生产者和消费者对应到相同的缓冲区
	circle_user_ = cir_user;
	circle_protocol_ = cir_proto;
	circle_sys_ = cir_sys;
	circle_flow_ = cir_flow;
	


	
}

void sql_server_operator::start_up()
{

	//初始化
   	dbinit();
   //连接数据库
   	loginrec = dblogin();

	DBSETLUSER(loginrec, user_name.c_str());
	DBSETLPWD(loginrec, password.c_str());
	dbprocess = dbopen(loginrec, server_host.c_str());
	if(dbprocess == FAIL)
	{
	 	throw std::runtime_error("sqlserver val_ptr new failed");
	    
	}
	if(dbuse(dbprocess, dbname.c_str()) == FAIL)
	{
	 	throw std::runtime_error("sqlserver val_ptr new failed");
	    
	}

   
	pthread_create(&tid, NULL, _thread_t<sql_server_operator, &sql_server_operator::_Run>, this);
	pthread_detach(tid);
	
}





#define SQL_USER_HEAD "insert into ids_flow_ip(src_ip, dst_ip, event_time, flow) values('%s', '%s', getdate(), %lu)"
#define SQL_PROTOCOL_HEAD "insert into ids_flow_protocol(protocol, event_time, flow) values('%s', getdate(), %lu)"
#define SQL_SYS_HEAD "delete from ids_server_info insert into ids_server_info(cpu, totalmemory, usedmemory, totalharddisk, usedharddisk) values(%.3f, %lu, %lu, %lu, %lu)"
#define SQL_FLOW_STAT_HEAD "insert into ids_flow_mbps(mbps, event_time) values(%.3f, getdate())"


void sql_server_operator::_Run()
{

	char sql_query[1024] = {0};
	_nic_stat_user *user_ptr = new _nic_stat_user;
	_nic_stat_protocol *protocol_ptr = new _nic_stat_protocol;
	_nic_stat_sys *sys_ptr = new _nic_stat_sys;
	_nic_flow_stat *flow_ptr = new _nic_flow_stat;

	
	for(; ;)
	{
		if( circle_user_->pop_front(user_ptr) == true)
		{
			memset(sql_query, 0, 1024);
			
			//snprintf(sql_query, 1024, SQL_USER_HEAD, user_ptr->ip_, user_ptr->time_, user_ptr->current_flow_);
			char src_ip[16] = {0};
			char dst_ip[16] = {0};
			Format_ip(src_ip, user_ptr->src_ip_);
			Format_ip(dst_ip, user_ptr->dst_ip_);
			snprintf(sql_query, 1024, SQL_USER_HEAD, src_ip, dst_ip, user_ptr->current_flow_);
			dbcmd(dbprocess, sql_query);  
		    if(dbsqlexec(dbprocess) == FAIL)
		    {
		   		LOG_ERR("user ip sql_server error!!!");
		    }
			++user_count;
		
		}

		if( circle_protocol_->pop_front(protocol_ptr) == true)
		{
			memset(sql_query, 0, 1024);
			//填写ip层的协议名称
			
			snprintf(sql_query, 1024, SQL_PROTOCOL_HEAD, protocol_ptr->protocol_.c_str(),  protocol_ptr->current_flow_);
			
			dbcmd(dbprocess, sql_query);  			
		    if(dbsqlexec(dbprocess) == FAIL)
		    {
		   		LOG_ERR("protocol sql_server error!!!");
		    }
			++protocol_count;
		}

		if(circle_sys_->pop_front(sys_ptr) == true)
		{
			memset(sql_query, 0, 1024);
			snprintf(sql_query, 1024, SQL_SYS_HEAD, sys_ptr->cpu_info.cpu_occupy_rate,
				sys_ptr->mem_info.mem_total, sys_ptr->mem_info.mem_used,
				sys_ptr->disk_info.disk_total, sys_ptr->disk_info.disk_used);
			
			dbcmd(dbprocess, sql_query);  
			
		    if(dbsqlexec(dbprocess) == FAIL)
		    {
		   		LOG_ERR("sys sql_server error!!!");
		    }
			++sys_count;

		}

		if(circle_flow_->pop_front(flow_ptr) == true)
		{
			memset(sql_query, 0, 1024);
			snprintf(sql_query, 1024, SQL_FLOW_STAT_HEAD, flow_ptr->flow_rate);
			
			dbcmd(dbprocess, sql_query);  
			
		    if(dbsqlexec(dbprocess) == FAIL)
		    {
		   		LOG_ERR("flow count sql_server error!!!");
		    }
			++flow_count;
		}
		
		usleep(10);
	}
}



void sql_server_operator::print_statistics(const std::string dev)
{
	LOG_INFO("Stat Infomation");
	printf("stats: %s \n", dev.c_str());
	printf("	ip_user_result:  %lu\n", user_count);
	printf("	protocol_result: %lu\n", protocol_count);
	printf("	sys_result:    %lu\n", sys_count);	
	printf("	flow_result:	%lu\n", flow_count);
	printf("=====================================================\n", flow_count);
}

