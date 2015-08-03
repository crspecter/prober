#include "ip_decoder.h"

bool ip_decoder::Decode(uint8_t* pCode, uint32_t uiLen)
{
	if ((NULL == pCode) || (0 == uiLen))
	{
		return false;
	}	
	Init();
	LPSIPHEADER lpIPHeader = (LPSIPHEADER)pCode;
	payload_data_offset= pCode + sizeof(SIPHEADER);
	uint16_t usIPPacketLen=ntohs(lpIPHeader->usPacketLen);

	if (uiLen>usIPPacketLen)
	{
		payload_data_len = usIPPacketLen-sizeof(SIPHEADER);
	}
	else
	{
		payload_data_len = uiLen -  sizeof(SIPHEADER);
	}
	version = (lpIPHeader->ucVerHl) >> 4;
	protocol = lpIPHeader->ucPT;
	dst_ip = ntohl(lpIPHeader->unDst);
	src_ip = ntohl(lpIPHeader->unSrc);
	return true;
}