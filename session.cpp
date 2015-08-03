#include "session.h"
#include "MurmurHash2.h"

bool session_key::operator==(const session_key& sk) const 
{
	return (ip_version == sk.ip_version) && (protocol == sk.protocol)
		      && (ip_pair == sk.ip_pair);
}


bool protocol_key::operator==(const protocol_key& sk) const
{
	return key_port_protocol == sk.key_port_protocol;
}

size_t session_hash::operator()(const session_key& sk) const {
	return MurmurHash64A(&sk, sizeof(sk), 0xee6b27eb);
}


size_t protocol_hash::operator()(const protocol_key& pk) const {
	return MurmurHash2(&pk, sizeof(pk), 0xee6b27eb);
}