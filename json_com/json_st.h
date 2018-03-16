#pragma once
#ifndef JSON_ST_H_H
#define JSON_ST_H_H 

#include <vector>
#include <map>
using std::vector; 
using std::map; 

enum JSON_VALUE_TYPE
{
	type_uint8,
	type_int8,
	type_uint16,
	type_int16,
	type_uint32,
	type_int32,
	type_uint64,
	type_int64,
	type_str,
	type_arr,
	type_obj
}

struct json_type
{
	int type; 
	int sub_type; 
}

struct json_value
{
private:
	map<const char *, struct json_type> keys_and_type; 
	vector<struct json_value*> sub_objs; 
public:
	json_value() = default; 
	json_value():sub_obj(NULL) {}
	void addMember(const char *key, struct json_type type, struct json_value * sub_obj = NULL)
	{
		keys_and_type.push_back(make_pair(key, type)); 
		if (sub_obj)
			sub_objs.push_back(sub_obj); 
	}
};

#endif  // JSON_ST_H_H
