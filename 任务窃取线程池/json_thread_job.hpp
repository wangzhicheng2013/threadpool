#pragma once
#include <string>
#include <json/json.h>
class json_thread_job final : public thread_job {
public:
    static std::string execute(const std::string &str) override {
        test_serialize(str);
        test_deserialize(str);
        return "hello simple thread pool";
    }
    static void test_serialize(std::string &str) {
        Json::Value root;
        Json::Value data;
        root["action"] = "run";
        data["number"] = 1;
        root["data"] = data;

        Json::StreamWriterBuilder builder;
        str = Json::writeString(builder,root);
        std::cout << str << std::endl;
    }
    static void test_deserialize(const std::string &str) {
        Json::CharReaderBuilder builder;
        std::unique_ptr<Json::CharReader>reader(builder.newCharReader());

        JSONCPP_STRING err;
        Json::Value root;
        Json::Value data;

        if (std::string::npos == str.find("{")) {   // jsoncpp对没有"{"会产生异常 parse不会出错 但root["action"].asString()这样代码会出错
            std::cout << "json invalid." << std::endl;
            return;
        }
        if (!reader->parse(str.c_str(), str.c_str() + str.size(), &root, &err)) {
            std::cout << err << std::endl;
            return;
        }

        std::string x = root["action"].asString();
        data = root["data"];
        int y = data["number"].asInt();
        std::string res = x;
        for (int i = 0;i < 1000;i++) {
            res += std::to_string(y);
        }
        std::cout << res << std::endl;
    }
};