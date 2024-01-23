// SocetApp.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <string>
#include <thread>
#include "TCPServer.h"



void handle_getting_message(TCPClient* client) {
    while (true) {
        Message* msg = client->get_message();
        std::cout << msg->to_str() << std::endl;
    }
}

void handle_sending_message(TCPClient* client) {
    while (true) {
        std::string text;
        int addr;
        std::cin >> addr;
        std::cin >> text;
        Message* msg = new Message(client->client_id, addr, Send, text);
        client->send_message(msg);
    }
}

void run_client(int id) {
    std::string server_ip = "192.168.0.107";
    int server_port = 60666;

    TCPClient* client = new TCPClient(id);
    client->init();
    client->connect_to_server(&server_ip, server_port);
    std::thread sendThread = std::thread(handle_getting_message, client);
    std::thread receiveThread = std::thread(handle_sending_message, client);

    sendThread.join();
    receiveThread.join();

    client->close();
}


int main(int argc, char* argv[])
{
    std::cout << "Hello World! I'm Client\n";
    int ans = atoi(argv[1]);
    run_client(ans);
}