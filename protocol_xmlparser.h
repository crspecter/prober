#ifndef __PROTOCOL_XMLPARSER_H__
#define __PROTOCOL_XMLPARSER_H__


#include <string>
#include <unordered_map>
#include <cstdint>
#include "nic_stat.h"

class protocol_xml_parser
{
typedef	std::unordered_map<uint16_t, std::string>::iterator map_it;

public:
	void startup();
	void parse_array_config();
	void parse_xml_config();
	void get_protocol_name(uint16_t protocol_port_key, _nic_stat_protocol* stat_procol);

	void get_key_port(uint16_t &port, _nic_stat_session_tuple* tuple);
	
private:

//存放xml配置对应的协议名称
	std::unordered_map<uint16_t, std::string> map_;
	
	

	std::string xml_file;
};


#endif