#pragma once
#include "Socet.h"
#include <string>
#include "Message.h"
#define MAX_MSG_LEN 100



class TCPClient {
public:
    int client_id;
    Socet* socet = nullptr;

    TCPClient() { }

    TCPClient(int id) {
        client_id = id;
        this->socet = new Socet();
    }

    TCPClient(int id, Socet* socet) {
        client_id = id;
        this->socet = socet;
    }

    void init() {
        WSADATA wsa;
        WSAStartup(MAKEWORD(2, 2), &wsa);

    }

    void close() {
        this->socet->close();
        WSACleanup(); //очищаем ресурсы вделенные для Winsock
    }

    void connect_to_server(std::string* ip, int port) {
        this->socet->init(ip, port);
        this->socet->connect();
        send_message(new Message(this->client_id, 0, MessageCode::Regestry, ""));
    }

    void send_message(Message* msg) {
        std::string msg_txt = msg->to_str();
        send(this->socet->sokcet, msg_txt.c_str(), msg_txt.size(), 0);// 0 - без флагов
    }

    Message* get_message() {
        char* buffer = new char[MAX_MSG_LEN]();
        recv(this->socet->sokcet, buffer, MAX_MSG_LEN, 0);// 0 - без флагов
        Message* msg = new Message();
        msg->from_str(std::string(buffer));
        return msg;
    }
};