#pragma once
#include"JsonValues.h"
inline std::ostream & operator <<(std::ostream & o, const JsonNode & jn);
class JsonNode
{
public:
	friend inline std::ostream & operator <<(std::ostream & o, const JsonNode & jn);
	explicit JsonNode(const std::string & key)
		:__key(std::move(key)) {}
	explicit JsonNode(const std::string & key, const JsonValue&val)
		:__key(std::move(key)), __value(std::move(val)) {}
	std::string toString()const
	{
		return __key.size() == 0 ? __value.toString() : "\"" + __key + "\"" + " : " + __value.toString();
	}
	bool operator<(const JsonNode & b)const
	{
		return this->__key < b.__key;
	}
	auto operator [](const std::string & s)
	{
		JSONTYPE value_type = __value.getType();
		if (!(value_type == JSONTYPE::JSON_TYPE))
			throw std::runtime_error("wrong [], value type is: " + value_type);
		// value must be a set
		auto & json_nodes = std::get<JsonArray>(__value.__value);
		auto find_ret = json_nodes.find(JsonNode(s));
		if (find_ret == json_nodes.end())
			throw std::runtime_error("worng [] key is: " + s);
		return *find_ret;// a jsonvalue not a node
	}
	std::string & getKey()
	{
		return __key;
	}

private:
	std::string __key;
	JsonValue __value;
};

inline std::ostream & operator <<(std::ostream & o, const JsonNode & jn)
{
	o << jn.__value.toString();
	return o;
}