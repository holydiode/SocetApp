#pragma once
#include <winsock.h>
#include <string>
#include <iostream>

class Socet {
private:
    sockaddr_in* addr;
    int addr_lenght;
    sockaddr* raw_addr;
public:
    SOCKET sokcet;

    Socet() {
        addr = new sockaddr_in();
        addr_lenght = sizeof(sockaddr_in);
        raw_addr = reinterpret_cast<struct sockaddr*>(addr);
        memset(addr, 0, addr_lenght);
    }

    Socet(SOCKET sokcet, sockaddr_in* addr) {
        this->sokcet = sokcet;
        this->addr = addr;
        addr_lenght = sizeof(sockaddr_in);
        raw_addr = reinterpret_cast<struct sockaddr*>(addr);
    }

    int init(std::string* adress, int port) {
        this->sokcet = socket(AF_INET, SOCK_STREAM, 0); //Создаем новый сокет для tcp/udp потоковой передачи
        if (this->sokcet == SOCKET_ERROR)
            return this->sokcet;

        addr->sin_family = AF_INET; // Использование tcp/udp
        addr->sin_addr.S_un.S_addr = inet_addr(adress->c_str());
        addr->sin_port = htons(port); //устанавливаем порт соединения
        raw_addr = reinterpret_cast<struct sockaddr*>(addr); //преобразуем адресса (небезопасно)
        return 0;
    }

    int bind() {
        int status = ::bind(this->sokcet, raw_addr, addr_lenght);
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
        return new Socet(soc, (sockaddr_in*)addr);
    }
};