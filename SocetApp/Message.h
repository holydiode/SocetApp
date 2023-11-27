#pragma once
#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;


enum MessageCode { 
    Regestry = 0, 
    Send = 1,
    Unattainable = 2,
};


class Message {
public:
    int author_id;
    int destination_id;
    MessageCode code;
    std::string msg;

    Message() {
    }

    Message(int author_id, int destination_id, MessageCode code, std::string msg) {
        this->author_id = author_id;
        this->destination_id = destination_id;
        this->code = code;
        this->msg = msg;
    }

    std::string to_str() {
        json data;
        data["author_id"] = author_id;
        data["destination_id"] = destination_id;
        data["code"] = code;
        data["msg"] = msg;
        return data.dump();
    }

    void from_str(std::string msg) {
        json data = json::parse(msg);
        this->author_id = data["author_id"];
        this->destination_id = data["destination_id"];
        this->code = data["code"];
        this->msg = data["msg"];
    }
};