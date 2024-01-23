#define WIN32_LEAN_AND_MEAN

#pragma once

#undef UNICODE
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <afunix.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#pragma comment(lib, "ws2_32")

#define SERVER_PATH   "../server.sock"

class Socet {
private:
    SOCKADDR_UN* addr;
    int addr_lenght;
    sockaddr* raw_addr;
public:
    SOCKET sokcet;

    Socet() {
        addr = new SOCKADDR_UN();
        addr_lenght = sizeof(sockaddr_in);
        raw_addr = reinterpret_cast<struct sockaddr*>(addr);
        memset(addr, 0, addr_lenght);
    }

    Socet(SOCKET sokcet, SOCKADDR_UN* addr) {
        this->sokcet = sokcet;
        this->addr = addr;
        addr_lenght = sizeof(sockaddr_in);
        raw_addr = reinterpret_cast<struct sockaddr*>(addr);
    }

    int init(std::string* adress, int port) {
        this->sokcet = socket(AF_UNIX, SOCK_STREAM, 0); //Создаем новый сокет для локальной потоковой передачи
        if (this->sokcet == SOCKET_ERROR)
            return this->sokcet;

        addr->sun_family = AF_UNIX; // Использование локальных каналов
        strncpy_s(addr->sun_path, sizeof SERVER_PATH, SERVER_PATH, (sizeof SERVER_PATH) - 1);

        raw_addr = reinterpret_cast<struct sockaddr*>(addr); //преобразуем адресса (небезопасно)
        return 0;
    }

    int bind() {
        int status = std::remove(SERVER_PATH);
        status = ::bind(this->sokcet, raw_addr, addr_lenght);
        //Соеденяем сокет с адресом и портом

        if (status == SOCKET_ERROR)
            return status;
        return 0;
    }

    int connect() {
        auto boopa = reinterpret_cast<struct sockaddr_in*>(raw_addr);
        int status = ::connect(this->sokcet, raw_addr, addr_lenght);
        while (status == SOCKET_ERROR)
        {
            std::cout << "ERROR: " << WSAGetLastError() << std::endl;
            status = ::connect(this->sokcet, raw_addr, addr_lenght);
        }
        return 0;
    }

    void close() {
        closesocket(this->sokcet);
    }

    Socet* accept() {
        sockaddr* addr = new sockaddr();
        SOCKET soc = ::accept(this->sokcet, addr, &addr_lenght);
        return new Socet(soc, (SOCKADDR_UN*)addr);
    }
};