#include "udp_decoder.h"

bool udp_decoder::Decode(uint8_t* pData, uint32_t uiDataLen)
{
	udp_init();
    // 参数合法性校验
    if ((NULL == pData) || (uiDataLen < UDP_HEADER_LEN))
    {
        return false;
    }
    m_usSrcPort = ntohs(*reinterpret_cast<const uint16_t*>(pData));
	m_usDstPort = ntohs(*reinterpret_cast<const uint16_t*>(pData + 2));

	
	// 填充承载
	if (uiDataLen > UDP_HEADER_LEN)
	{
		data_offset = pData+UDP_HEADER_LEN;
		data_len = uiDataLen-UDP_HEADER_LEN;
	}

 

	return true;
}

