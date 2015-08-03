#include "config.h"
#include "log.h"
#include <iostream>   
#include <algorithm>   
#include <functional>
#include <fstream> 
#include <stdlib.h>

Config_parser::Config_parser()
{
	get_process_name();
}



void Config_parser::parse_file()
{
	std::ifstream fs;
	//type_it it = parse_ret.end();
	fs.open(config_file_name);

	if(!fs.is_open())
	{
		LOG_ERR("config file:%s is not exist", config_file_name.c_str());
		exit(1);
	}

	std::string line;
	while(!fs.eof())
	{
		std::getline(fs, line);
		if(is_space_line(line) || is_comment(line))
		{
			continue;
		}
		else if (is_segment(line))
		{
			MAP_LOCATION map_loc;
			std::string segment;
			segment = get_segment(line);
			parse_ret[segment] = map_loc;
		}

	}
	fs.clear();
	fs.seekg(0, std::ios::beg);
	std::string seg, key, value;	
	
	while(!fs.eof())
	{
		std::getline(fs, line);
		if(is_space_line(line) || is_comment(line) )
		{
			continue;
		}

		if(is_segment(line))
		{
			seg = get_segment(line);
			continue;
		}
		

		if(!get_key_and_value(line, key, value))
			continue;

		(parse_ret[seg])[key] = value;
		
	}
}


std::string Config_parser::get_string(const std::string &seg, const std::string &key, std::string def)
{
	std::string ret;
	do
	{
		seg_it sg_it;
		value_it va_it;
		if((sg_it = parse_ret.find(seg)) == parse_ret.end())
		{
			LOG_ERR("Parse config file no segment: %s", seg.c_str());
			ret = def;
			break;
		}
		if((va_it = sg_it->second.find(key)) == sg_it->second.end())
		{
			LOG_ERR("Parse config file no key: %s", key.c_str());
			ret = def;
			break;
		}

		ret = va_it->second;

	}while(0);
	return ret;
}

uint32_t Config_parser::get_int(const std::string &seg, const std::string &key, uint32_t def)
{
	uint32_t ret;
	do
	{
		seg_it sg_it;
		value_it va_it;
		if((sg_it = parse_ret.find(seg)) == parse_ret.end())
		{
			LOG_ERR("Parse config file no segment: %s", seg.c_str());
			ret = def;
			break;
		}
		if((va_it = sg_it->second.find(key)) == sg_it->second.end())
		{
			LOG_ERR("Parse config file no key: %s", key.c_str());
			ret = def;
			break;
		}

		ret = atol(va_it->second.c_str());

	}while(0);
	return ret;
}

bool Config_parser::is_space_line(std::string line)
{
	line.erase(0, line.find_first_not_of(" \t\f\v\n\r"));
	return line.length() == 0?true:false;
}

void Config_parser::trim_space(std::string &dest_str)
{
    dest_str.erase(0, dest_str.find_first_not_of(" \t\f\v\n\r"));  
     
    dest_str.erase(dest_str.find_last_not_of(" \t\f\v\n\r")+1);  
}


bool Config_parser::is_segment(std::string line)
{
	trim_space(line);
	if((start_with(line, "[")) && (end_with(line, "]")) ) return true;
	else return false;
}

bool Config_parser::is_comment(std::string line)
{
	trim_space(line);
	return start_with(line, "#");
}



bool Config_parser::get_key_and_value(std::string & line, std::string & key, std::string & value)
{
    size_t pos = line.find('=');//找到每行的“=”号位置，之前是key之后是value   
    if(pos==std::string::npos) return false;  
    key = line.substr(0,pos);//取=号之前  
    trim_space(key);
    value = line.substr(pos+1);//取=号之后   
    trim_space(value);

	if(key.length() == 0) return false;
	
    return true;  
}



std::string Config_parser::get_segment( std::string&  line)
{
    line.erase(std::remove_if(line.begin(),line.end(),   
        std::bind2nd(std::equal_to<char>(), '[')),   
        line.end());
	
    line.erase(std::remove_if(line.begin(),line.end(),   
        std::bind2nd(std::equal_to<char>(), ']')),   
        line.end());	
	return line;
}




bool Config_parser::start_with(const std::string& dst_str, const std::string& targ)
{
	return dst_str.compare(0, targ.size(), targ) == 0;
}
bool Config_parser::end_with(const std::string& dst_str, const std::string& targ)
{
	return dst_str.compare(dst_str.size() - targ.size(), targ.size(), targ) == 0 ;
}


void Config_parser::get_process_name()
{
    char buf[1024] = { 0 };
	char name[64] = { 0 };
    int n;
 
    n = readlink("/proc/self/exe" , buf , sizeof(buf));

    buf[n] = '\0';
    for(n=n-1;n>0;n--)
    {
       if(buf[n] == '/')
       break;

    }    
    strncpy(name, buf + n + 1 , strlen(buf+n) - 1);
	config_file_name ="./";
	config_file_name += name;
	config_file_name += ".conf";
}
