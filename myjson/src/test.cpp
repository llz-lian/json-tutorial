#include<string>
#include<limits.h>
#include<iostream>
#include<variant>
#include<vector>
#include<set>
#include<unordered_map>
class JsonNode;
using String = std::string;
using Null = std::monostate;
using JsonArray = std::set<JsonNode>;
using Value = std::variant<Null, bool, double, String,JsonArray>;

struct JsonValue
{
	JsonValue(const Value &&val) :__value(std::move(val)){}
	JsonValue(const Value &&val,bool array_type)
		:__value(std::move(val)),array_type(array_type){}
	Value __value;
	bool array_type = false;
	std::string toString()const
	{
		std::string result;
		std::visit(
			[&](auto && arg)
			{
				using T = std::decay_t<decltype(arg)>;
				if constexpr (std::is_same_v<T, double>)
				{
					result += to_string(arg);
				}
				else if constexpr (std::is_same_v<T, String>)
				{
					result.push_back('\"');
					result += arg;
					result.push_back('\"');
				}
				else if constexpr (std::is_same_v<T, Null>)
				{
					result += "null";
				}
				else if constexpr (std::is_same_v<T, bool>)
				{
					result += arg ? "true" : "false";
				}
				else if constexpr (std::is_same_v<T, JsonArray>)
				{
					if (this->array_type)
					{
						result += '[';
						for (auto && v : arg)
						{
							result += v.toString() + ",";
						}
						result.pop_back();
						result += ']';
					}
					else
					{
						result += '{';
						for (auto && v : arg)
						{
							result += v.toString() + ",";
						}
						result.pop_back();
						result += '}';
					}
				}
			}, __value);
		return result;
	}
};

class JsonNode
{
public:
	explicit JsonNode(const std::string & key, const JsonValue&val)
		:__key(std::move(key)), __value(std::move(val)) {}
	std::string toString()const
	{
		return __key.size()==0? __value.toString() :"\""+__key+"\"" + " : " +  __value.toString();
	}
	bool operator<(const JsonNode & b)const
	{
		return this->__key < b.__key;
	}
private:
	std::string __key;
	JsonValue __value;
};
class Json 
{
public:
	std::string toString()const
	{
		std::string result;
		result += "{\n";
		for (auto & node : jsons)
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
	void addNode(const JsonNode & jn)
	{
		jsons.insert(jn);
	}
private:
	std::set<JsonNode> jsons;
};
int main()
{
	/*
		// a node
		"a":[
			"a_sub_num":1.0,
			"a_sub_bool":false,
			"a_sub_null":null,
			"a_sub_string":"a string",
			"a_sub_array":["array_string",true,{"a_sub_sub_num":100}]
			]
	*/
	JsonNode node("a", JsonValue{
		JsonArray{
			JsonNode{"a_sub_num",JsonValue{1.0}} ,
			JsonNode{"a_sub_bool",JsonValue{false}},
			JsonNode{"a_sub_null",JsonValue{std::monostate{}}},
			JsonNode{"a_sub_string",JsonValue{String("a string")}},
			JsonNode{"a_sub_array", JsonValue{
				JsonArray{
					JsonNode{"",JsonValue{1.0}} ,
					JsonNode{"",JsonValue{true}} ,
					JsonNode{"",JsonValue{JsonArray{JsonNode{"array_string",JsonValue{100.0}}},false}}
				},true
			}}
		},false });
	// json node: {"key",value}
	// json type: JsonNode{"",JsonValue{JsonArray{node},false}}
	// json array type: JsonArray{nodes}
	std::cout << node.toString()<<std::endl;
	/*
		// a json
		{
			"title": "Design Patterns",
			"subtitle": "Elements of Reusable Object-Oriented Software",
			"author": [
			    "Erich Gamma",
			    "Richard Helm",
			    "Ralph Johnson",
			    "John Vlissides"
			],
			"year": 2009,
			"weight": 1.8,
			"hardcover": true,
			"publisher": {
			    "Company": "Pearson Education",
			    "Country": "India"
			},
			"website": null
		}
	*/
	JsonNode title("title", JsonValue{ String("Design Patterns") });
	JsonNode subtitle("subtitle", JsonValue{ String("Elements of Reusable Object-Oriented Software") });
	JsonNode author("author", JsonValue{
			JsonArray{
				JsonNode{"",JsonValue{String("Erich Gamma")}},
				JsonNode{"",JsonValue{String("Richard Helm")}},
				JsonNode{"",JsonValue{String("Ralph Johnson")}},
				JsonNode{"",JsonValue{String("John Vlissides")}}
			},true
		}
	);
	JsonNode year("year", JsonValue{ 2009.0 });
	JsonNode weight("weight", JsonValue{ 1.8 });
	JsonNode hardcover("hardcover", JsonValue{ true });
	JsonNode publisher("publisher", JsonValue{
			JsonArray{
				JsonNode{"Company",JsonValue{String("Pearson Education")}},
				JsonNode{"Country",JsonValue{String("India")}}
			},
			false
		}
	);
	JsonNode website("website", JsonValue{ std::monostate{} });
	Json js;
	js.addNode(title);
	js.addNode(subtitle);
	js.addNode(author);
	js.addNode(year);
	js.addNode(weight);
	js.addNode(hardcover);
	js.addNode(publisher);
	js.addNode(website);
	std::cout << js.toString() << std::endl;
	return 0;
}