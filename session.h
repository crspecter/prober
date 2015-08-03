#ifndef __SESSION_H__
#define __SESSION_H__

#include <unordered_map>
#include <cstdint>
#include <cstring>
#include "MurmurHash2.h"
#include "nic_stat.h"

#pragma pack(1)
class session_key
{
public:
	session_key() { memset(this, 0, sizeof(*this)); }

	bool operator==(const session_key& sk) const;
	
public:	

	uint8_t  ip_version;
	uint32_t src_ip;
	uint32_t dst_ip;
	
	uint32_t ip_pair;
	
	uint8_t  protocol;
	
};

class protocol_key
{
public:
	protocol_key() { memset(this, 0, sizeof(*this)); }

	bool operator==(const protocol_key& sk) const;
	
public:	
	//tcp udp时key为 port ^ ip.protocol 其他 ip.protocol ^ 0
	uint16_t key_port_protocol;
};
#pragma pack()

class session_hash {
public:
	size_t operator()(const session_key& sk) const;
};

class protocol_hash {
public:
	size_t operator()(const protocol_key &pk) const;
};





 





typedef std::unordered_map<session_key, _nic_stat_user, session_hash> session_key_map;
typedef std::unordered_map<session_key, _nic_stat_user, session_hash>::iterator session_key_map_it;
typedef std::unordered_map<protocol_key, _nic_stat_protocol, protocol_hash> protocol_key_map;
typedef std::unordered_map<protocol_key, _nic_stat_protocol, protocol_hash>::iterator protocol_key_map_it;


#endif
