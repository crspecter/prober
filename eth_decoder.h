#ifndef __ETH_DECODER_H__
#define __ETH_DECODER_H__
#include <cstdint>
#include <cstdlib>
#include <arpa/inet.h>
typedef enum _tagEETHPLTYPE
{
	EETHPLTYPE_IP	= 0x0800,	// ip type
	EETHPLTYPE_MPLS	= 0x8847,	// MPLS type
	EETHPLYTPE_VLAN = 0x8100,	// 802.1Q Virtual LAN
}EETHPLTYPE, *LPEETHPLTYPE;

#pragma pack(1)
typedef struct eth_header
{
	uint8_t dst_mac[6];
	uint8_t src_mac[6];
	uint16_t pl_type;
};

#pragma pack()
class eth_decoder
{
public:
	void Init()
	{
		payload_data_offset = NULL;
		payload_data_len = 0;
	}

	bool Decode(uint8_t* pData, uint32_t uiLen);
private:
	uint32_t GetMPLSLen(uint8_t* pPkt, uint32_t uiLen);
public:	
	uint8_t *payload_data_offset;
	uint32_t payload_data_len;
	
};

#endif
