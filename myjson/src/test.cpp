#include"Json.h"
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
			    "Country": "India",
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
			},
			true
		}
	);
	JsonNode year("year", JsonValue{ 2009.0 });
	JsonNode weight("weight", JsonValue{ 1.8 });
	JsonNode hardcover("hardcover", JsonValue{ true });
	JsonNode publisher("publisher", JsonValue{
			JsonArray{
				JsonNode{"Company",JsonValue{String("Pearson Education")}},
				JsonNode{"Country",JsonValue{String("India")}},
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
	try
	{
		auto && company_ret = js["publisher"]["Company"];
		std::cout << company_ret << std::endl;

		auto && year_ret = js["year"];
		std::cout << year_ret << std::endl;

		auto && author_ret = js["author"];
		std::cout << author_ret << std::endl;

		auto && error_ret = js["abc"];
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what();
		exit(1);
	}
	// auto error_ret = sub_ret["a"];
	return 0;
}