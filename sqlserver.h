#ifndef __SQL_SERVER_H__
#define __SQL_SERVER_H__

#include "circle_buffer.h"
#include "config.h"
#include <stdexcept>
#include <pthread.h>
#include "nic_stat.h"
#include </usr/local/freetds/include/sqlfront.h>
#include <string>
#include "sys_info_struct.h"
#include <unordered_map>





class sql_server_operator
{
typedef std::unordered_map<int, std::string>::const_iterator const_it;
public:

	sql_server_operator();

	
    ~sql_server_operator()
   	{
		
		dbclose(dbprocess);
    }
	
	void start_up();

	void _Run();

	void init(circle_buffer<_nic_stat_user>* cir_user,
		   circle_buffer<_nic_stat_protocol>* cir_proto, 
		   circle_buffer<_nic_stat_sys>* cir_sys,
		   circle_buffer<_nic_flow_stat>* cir_flow);

	void print_statistics(const std::string dev);



private:
    pthread_t tid;
private:
	std::string user_name;
	std::string password;
	std::string dbname;
	std::string server_host;

	LOGINREC *loginrec;
	DBPROCESS *dbprocess;
	
	circle_buffer<_nic_stat_user>*  circle_user_;
	circle_buffer<_nic_stat_protocol>*  circle_protocol_;
	circle_buffer<_nic_stat_sys>* circle_sys_;
	circle_buffer<_nic_flow_stat>* circle_flow_;


	Config_parser config_;


	uint64_t user_count;
	uint64_t protocol_count;
	uint64_t sys_count;
	uint64_t flow_count;


	
};







#endif
