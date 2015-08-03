#include "tcp_decoder.h"


bool tcp_decoder::Decoder(uint8_t* pData, uint32_t uiDataLen)
{
	init();
	if(NULL == pData || uiDataLen < 20)
		 return false;
	

	sport = ntohs(*reinterpret_cast<const uint16_t*>(pData));
	dport = ntohs(*reinterpret_cast<const uint16_t*>(pData + 2));
	seq_num = ntohl(*reinterpret_cast<const uint32_t*>(pData + 4));
	ack_num = ntohl(*reinterpret_cast<const uint32_t*>(pData + 8));
	tcp_head_len = (pData[12] >> 4) << 2;
	window = ntohs(*reinterpret_cast<const uint16_t*>(pData + 14));

	data_len = uiDataLen - tcp_head_len;
	data_offset = pData + tcp_head_len;
	return true;
}