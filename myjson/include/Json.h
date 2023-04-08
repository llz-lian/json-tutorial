#pragma once
#include"Node.h"
class Json;
class CanJson
{
public:
	virtual Json toJson() = 0;
};
class Json
{
public:
	Json():jsons(std::make_shared<JsonArray>()){};
	~Json() {};
	Json(const Json & js):jsons(js.jsons) {};
	Json(const Json && js) :jsons(js.jsons) {};
	Json(const CanJson & cls) {};
	void addNode(const JsonNode & jn)
	{
		jsons->insert(jn);
	}
	void parse(const std::string_view & s) {};
	void read(const std::string & file_path) {};
	void dump() {};
	auto operator [](const std::string & s)
	{
		auto find_ret = jsons->find(JsonNode(s));
		if (find_ret == jsons->end())
			throw std::runtime_error("wrong [] key is: " + s);
		return *find_ret;
	}
	std::string toString()const
	{
		std::string result;
		result += "{\n";
		for (auto & node : *jsons)
		{
			result.push_back('\t');
			result += node.toString();
			result.push_back(',');
			result.push_back('\n');
		}
		result.pop_back();
		result.pop_back();
		result += "\n}\n";
		return result;
	}
private:
	std::shared_ptr<JsonArray> jsons;
};