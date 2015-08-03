#ifndef __UDP_DECODER_H__
#define __UDP_DECODER_H__

#include <cstdint>
#include <cstdlib>
#include <arpa/inet.h>


#define UDP_HEADER_LEN      8




class udp_decoder
{
public:
	/*
	 *	构造函数
	 */
	udp_decoder() 
	{
		 data_offset = NULL;
		 data_len = 0;
		 m_usSrcPort = 0;
		 m_usDstPort = 0;
	}
	/*
	 *	析构函数
	 */
	virtual ~udp_decoder(){}
private:
	void udp_init()
		{
		  data_offset = NULL;
		  data_len = 0;
		 m_usSrcPort = 0;
		 m_usDstPort = 0;
		}

public:

	/*
	 *	解码函数
	 */
	bool Decode(uint8_t* pData, uint32_t uiLen);
	
public:
	uint8_t  *data_offset;
	uint16_t  data_len;
	uint16_t m_usSrcPort;
	uint16_t m_usDstPort;
};


#endif
