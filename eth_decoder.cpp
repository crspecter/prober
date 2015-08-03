#include "eth_decoder.h"


#define ETHHEADER_LEN	sizeof(eth_header)

bool eth_decoder::Decode(uint8_t* pData, uint32_t uiLen)
{
	
	if ((NULL == pData) || (uiLen < ETHHEADER_LEN))
	{
		return false;
	}
	Init();
	//int offset = 0;
	
	eth_header *p_eth_header = (eth_header*)pData;
	uint16_t pltype = ntohs(p_eth_header->pl_type);
	if(EETHPLTYPE_MPLS == pltype)
	{
		uint32_t mpls_len = GetMPLSLen((uint8_t*)(pData + ETHHEADER_LEN),
										uiLen - ETHHEADER_LEN);
		payload_data_len = uiLen - mpls_len - ETHHEADER_LEN;
		if( 0 != payload_data_len)
			payload_data_offset = pData + ETHHEADER_LEN + mpls_len;
		
	}
	//vlan packet have multi layers 
	else if(EETHPLYTPE_VLAN == pltype)
	{
		payload_data_len = uiLen - ETHHEADER_LEN - 4 ;
		if (0 != payload_data_len)
		{
			payload_data_offset = pData + ETHHEADER_LEN + 4;
		}	
		else
			return false;
	}
	//nomal ip packet
	else if(EETHPLTYPE_IP == pltype)
	{
		payload_data_len = uiLen - ETHHEADER_LEN;
		if (0 != payload_data_len)
		{
			payload_data_offset = pData + ETHHEADER_LEN ;
		}	
		else 
		    return false;
		
	}
	else
	{
		return false;
	}
	return true;
}



uint32_t eth_decoder::GetMPLSLen(uint8_t* pPkt, uint32_t uiLen)
{
//	assert(pPkt);
	// 1 2 3 4 5 6 7 8|1 2 3 4 5 6 7 8|1 2 3 4 5 6 7 8|1 2 3 4 5 6 7 8
	//+		Label					          + Exp +S+   	TTL		  + 
	// S：1bit，MPLS支持多重标签。值为1时表示为最底层标签。
#define S_MASK	0x01
	uint8_t uiS = (pPkt[2] & S_MASK);
	uint32_t uiRetLen = 4;
	while ((0 == uiS) && (uiRetLen < uiLen))
	{
		uiS = (pPkt[uiRetLen+2] & S_MASK);
		uiRetLen += 4;
	}
	
	return uiRetLen;
}