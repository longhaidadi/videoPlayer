 #include "json/value.h"
 #include "json/json.h"
 #include <string>
void testParserJson(){
	std::string strValue = “{\”key1\”:\”value1\”,\”array\”:[{\"key2\":\"value2\"},{\"key2\":\"value3\"},{\"key2\":\"value4\"}]}”;

    Json::Reader reader;
    Json::Value value;

    if (reader.parse(strValue, value))
    {
		std::string out = value["key1"].asString();
		std::cout << out << std::endl;
		const Json::Value arrayObj = value["array"];
		for (int i=0; i<arrayObj.size(); i++){
			out = arrayObj[i]["key2"].asString();
			std::cout << out;
			if (i != arrayObj.size() – 1 )
				std::cout << std::endl;
			}
    }
}

void writeJson(){
	Json::Value root;
    Json::Value arrayObj;
    Json::Value item;
    for (int i=0; i<10; i++)
    {
		item["key"] = i;
		arrayObj.append(item);
    }

		root["key1"] = “value1″;
		root["key2"] = “value2″;
		root["array"] = arrayObj;
		root.toStyledString();
		std::string out = root.toStyledString();
		std::cout << out << std::endl;
}

int main(){
	testParserJson();
	writeJson();
	return 0;
}
