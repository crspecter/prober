#include "protocol_xmlparser.h"

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"

#include "l3_protocol_name.h"

using namespace rapidxml;

#define TCP_STR 17
#define UDP_STR 6

_protocol_name _protocol_name_array[] = {
		{1, "ICMP"},
		{2, "IGMP"},
		{3, "GGP"},
		{4, "IP-ENCAP"},
		{5, "ST"},
		{6, "TCP"},
		{8, "EGP"},
		{12, "PUP"},
		{17, "UDP"},
		{20, "HMP"},
		{22, "XNS-IDP"},
		{27, "RDP"},
		{29, "ISO-TP4"},
		{36, "XTP"},
		{37, "DDP"},
		{39, "IDRP-CMTP"},
		{73, "RSPF"},
		{81, "VMTP"},
		{89, "OSPFIGP"},
		{94, "IPIP"},
		{98, "ENCAP"},
};


void protocol_xml_parser::startup()
{
	parse_array_config();
	parse_xml_config();
}

void protocol_xml_parser::parse_array_config()
{
	//step1 从全局数组中读取对应的协议名称
    for (int i = 0 ; i < sizeof(_protocol_name_array) / sizeof(_protocol_name_array[0]); i++)
    {
    	map_[_protocol_name_array[i].pro_number] = _protocol_name_array[i].name;	
	}
}

void protocol_xml_parser::parse_xml_config()
{


	//step2 从xml中读取协议名称
    file<> fdoc("port_name.xml");
    xml_document<>   doc;
    doc.parse<0>(fdoc.data());



	xml_node<>* root = doc.first_node();
	xml_node<> * node = root->first_node("segment");
	while(node != NULL)
	{  
		//解析tcp/udp分类
		xml_attribute<> * attr = node->first_attribute("protocol");  		
		std::string  prot_value = attr->value();
		
		uint16_t uprotocol = atol(prot_value.c_str());
		
		attr = node->first_attribute("port");	
		std::string  port = attr->value();
		uint16_t uport = atol(port.c_str());
		
		attr = node->first_attribute("name");
		std::string  name = attr->value();

		map_[uprotocol ^ uport] = name;
			
        node = node->next_sibling();
	}  

	
}


void protocol_xml_parser::get_key_port(uint16_t &port_key, _nic_stat_session_tuple* tuple)
{
	if(tuple->protocol_ == 6 || tuple->protocol_ == 17)
	{
		map_it it;
		do
		{
			if((it = map_.find(tuple->src_port_ ^ tuple->protocol_)) != map_.end())
			{
				port_key = it->first;
				break;
			}
			else if((it = map_.find(tuple->dst_port_ ^ tuple->protocol_)) != map_.end())
			{
				port_key = it->first;
				break;
			}
			else
				port_key = tuple->protocol_;
		}while(0);

	}
		
	else
		port_key = tuple->protocol_;
}

void protocol_xml_parser::get_protocol_name(uint16_t  protocol_port_key, _nic_stat_protocol* stat_procol)
{

	map_it it;
	
	if((it = map_.find(protocol_port_key)) != map_.end())
	{
		stat_procol->protocol_ = it->second;
	}
	else
		stat_procol->protocol_ = "unknown";

}
