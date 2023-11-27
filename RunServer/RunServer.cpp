#include <iostream>
#include <iostream>
#include <windows.h>
#include <iostream>
#include <string>
#include "../SocetApp/Message.h"
#include "../SocetApp/Socet.h"
#include "../SocetApp/TCPClient.h"
#include "../SocetApp/TCPServer.h"

void handle_client(TCPServer* server, TCPClient* client) {
    while (true)
    {
        Message* msg = server->get_message(client);
        if (msg->code == Send) {
            server->send_message(msg);
        }
        delete msg;
    }
}

void run_server() {
    std::string server_ip = "192.168.0.107";
    int server_port = 60666;

    TCPServer* server = new TCPServer(&server_ip, server_port);

    server->init();

    while (true) {
        TCPClient* client = server->accept_conection();
        std::thread thread = std::thread(handle_client, server, client);
        thread.detach();
    }

    server->close();
}




int main()
{
    std::cout << "Hello World! I'm Server\n";
    run_server();
}

