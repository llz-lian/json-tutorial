#include<variant>
enum JSONTYPE:int
{ 
    NULL, BOOL , NUMBER, STRING, ARRAY, JSONNODE 
};

struct JsonType
{
    int json_type;
};

class JsonNode:protected JsonType
{
public:
    using JsonType::json_type;
    std::variant<bool,string> value;
};
