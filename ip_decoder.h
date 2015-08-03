#ifndef __IP_DECODER_H__
#define __IP_DECODER_H__
#include <cstdint>
#include <cstdlib>
#include <arpa/inet.h>

#pragma pack(1)

// IP头结构定义
typedef struct _SIPHEADER
{
	uint8_t     ucVerHl;            // version & head length
	uint8_t   	ucTOS;          	// Type of Service
	uint16_t  	usPacketLen;    	// Packet Length
	uint16_t  	usSequence;     	// identificatino
	uint16_t    usFragOff;          // :1 Flags:Reserved bit :1 Don't fragment :1 More fragment :13 Fragment offset field
	uint8_t   	ucTTL;          	// Time Life
	uint8_t   	ucPT;           	// Protocoltype
	uint16_t  	usCheckSum;     	// Check Sum
	uint32_t    unSrc;          	// Source IP
	uint32_t    unDst;          	// Destinate IP
}SIPHEADER, *LPSIPHEADER;

#pragma pack()

class ip_decoder
{
public:
	void Init()
	{
		version = 0;
		payload_data_offset = NULL;
		payload_data_len = 0;
		protocol = 0;
		src_ip = 0;
		dst_ip = 0;
	}
	bool Decode(uint8_t* pCode, uint32_t uiLen);

public:
	uint8_t  version;
	uint8_t  protocol;
	uint8_t *payload_data_offset;
	uint32_t payload_data_len;
	uint32_t src_ip;
	uint32_t dst_ip;

};

#endif