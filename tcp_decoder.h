#ifndef __TCP_DECODER_H__
#define __TCP_DECODER_H__

#include <cstdint>
#include <cstdlib>
#include <arpa/inet.h>

class tcp_decoder 
{

public:
	void init()
	{
		 data_offset = NULL;
		 data_len = 0;
		
		 sport = 0;
		 dport = 0;

		 tcp_head_len = 0;
		 seq_num = 0;
		 ack_num = 0 ;
		 window = 0;
		 flag = 0;
	}
	bool Decoder(uint8_t* pData, uint32_t uiDataLen);

public:
	
	uint8_t  *data_offset;
	uint16_t  data_len;
	
	uint16_t sport;
	uint16_t dport;

	uint8_t  tcp_head_len;
	uint32_t seq_num;
	uint32_t ack_num;
	uint16_t window;
	uint8_t  flag;
	
	
	
};
#endif