#pragma once
#include<string>
#include<set>
#include<variant>
#include<iostream>
#include"Node.h"
enum JSONTYPE:int { NULL_TYPE, BOOL_TYPE, NUMBER_TYPE,STRING_TYPE, ARRAY_TYPE, JSON_TYPE ,NO_TYPE = -1};
class JsonNode;
using String = std::string;
using Null = std::monostate;
using JsonArray = std::multiset <JsonNode>;
using Value = std::variant<Null, bool, double, String, JsonArray>;

struct JsonValue
{
	JsonValue() {};
	JsonValue(const Value &&val) :__value(std::move(val)) {}
	JsonValue(const Value &&val, bool array_type)
		:__value(std::move(val)), array_type(array_type) {}
	Value __value = std::monostate{};
	bool array_type = false;
	JSONTYPE type = JSONTYPE::NO_TYPE;
	JSONTYPE getType()
	{
		if (type != JSONTYPE::NO_TYPE)
			return type;
		std::visit(
			[&](auto && arg)
			{
				using T = std::decay_t<decltype(arg)>;
				if constexpr (std::is_same_v<T, double>)
				{
					this->type = JSONTYPE::NUMBER_TYPE;
				}
				else if constexpr (std::is_same_v<T, String>)
				{
					this->type = JSONTYPE::STRING_TYPE;
				}
				else if constexpr (std::is_same_v<T, Null>)
				{
					this->type = JSONTYPE::NULL_TYPE;
				}
				else if constexpr (std::is_same_v<T, bool>)
				{
					this->type = JSONTYPE::BOOL_TYPE;
				}
				else if constexpr (std::is_same_v<T, JsonArray>)
				{
					this->type = this->array_type ? JSONTYPE::ARRAY_TYPE : JSONTYPE::JSON_TYPE;
				}
			}, __value);
		return type;
	}
	std::string toString()const
	{
		std::string result;
		std::visit(
			[&](auto && arg)
			{
				using T = std::decay_t<decltype(arg)>;
				if constexpr (std::is_same_v<T, double>)
				{
					result += std::to_string(arg);
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