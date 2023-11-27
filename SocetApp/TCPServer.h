#include <thread>
#include "Message.h"
#include "TCPClient.h"
#include <vector>
#define MAX_MSG_LEN 100
#define MAX_CONNECTION 100


class TCPServer {
private:
    Socet* socet = nullptr;
    TCPClient** clients;
    std::vector<int>* clients_ids;
    std::string* IP;
    int port;

public:
    TCPServer(std::string* ip, int port) {
        this->IP = ip;
        this->port = port;
        this->socet = new Socet();
        this->clients = new TCPClient * [MAX_CONNECTION]();
        this->clients_ids = new std::vector<int>();
    }

    int init() {
        WSADATA wsa;
        int status = WSAStartup(MAKEWORD(2, 2), &wsa); //инициализация Winsock 2.2
        status = this->socet->init(this->IP, this->port);
        status = this->socet->bind();
        status = listen(this->socet->sokcet, MAX_CONNECTION); //Устанавливаем сокет в режим прослушифания
        return 0;
    }

    void close() {
        this->socet->close();
        WSACleanup(); //очищаем ресурсы вделенные для Winsock
    }

    TCPClient* accept_conection() {
        Socet* client = this->socet->accept();
        TCPClient* tcp_client = new TCPClient(-1, client);
        Message* msg = get_message(tcp_client);
        tcp_client->client_id = msg->author_id;
        this->clients[msg->author_id] = tcp_client;
        clients_ids->push_back(msg->author_id);
        return this->clients[msg->author_id];
    }

    void send_message(Message* msg) {
        std::string msg_txt = msg->to_str();
        if (id_in_connections(msg->destination_id)) {
            send(this->clients[msg->destination_id]->socet->sokcet, msg_txt.c_str(), msg_txt.size(), 0);
            return;
        }
        msg->destination_id = msg->author_id;
        msg->author_id = 0;
        msg->code = Unattainable;
        send(this->clients[msg->destination_id]->socet->sokcet, msg_txt.c_str(), msg_txt.size(), 0);
    }

    Message* get_message(TCPClient* client) {
        char* buffer = new char[MAX_MSG_LEN]();
        recv(client->socet->sokcet, buffer, MAX_MSG_LEN, 0);// 0 - без флагов
        Message* msg = new Message();
        msg->from_str(std::string(buffer));
        return msg;
    }

    Message* get_message() {
        char* buffer = new char[MAX_MSG_LEN]();
        int status = recv(this->socet->sokcet, buffer, MAX_MSG_LEN, 0);// 0 - без флагов
        if (status == SOCKET_ERROR)
        {
            std::cout << "ERROR: " << WSAGetLastError() << std::endl;
        }
        Message* msg = new Message();
        msg->from_str(std::string(buffer));
        return msg;
    }

private:
    bool id_in_connections(int id) {
        return std::find(this->clients_ids->begin(), this->clients_ids->end(), id) != this->clients_ids->end();
    }

};
