#ifndef __CONFIG_H__
#define __CONFIG_H__
#include <map>
#include <string>
#include <cstring>
#include <cstdint>
class Config_parser
{

typedef std::map<std::string, std::string> MAP_LOCATION;
typedef std::map<std::string, MAP_LOCATION > MAP_TOTAL;
typedef std::map<std::string, std::string>::iterator 	value_it;
typedef std::map<std::string, MAP_LOCATION >::iterator  seg_it;

public:
	Config_parser();
	void parse_file();
	std::string get_string(const std::string &seg, const std::string &key, std::string def = "");
	uint32_t get_int(const std::string &seg, const std::string &key, uint32_t def = 0);

	
private:
	
	void trim_space(std::string &dest_str);
	bool start_with(const std::string& dst_str, const std::string& targ);
	bool end_with(const std::string& dst_str, const std::string& targ);
	void get_process_name();
	bool is_comment(std::string line);
	bool is_segment(std::string line);
	bool is_space_line(std::string line);
	
	std::string get_segment(std::string &line);
	bool get_key_and_value(std::string& line, std::string &key, std::string &value);
	
private:
	std::string config_file_name;
	MAP_TOTAL parse_ret;
};


#endif