#ifndef _SOCKET_WINDOWS_HPP
#define _SOCKET_WINDOWS_HPP

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>

namespace socket_common {

    class TcpSocket {
    private:
        SOCKET _sock;
        std::string _host;
        unsigned int _port;
        bool _init;

        struct addrinfo* getHostInfo() const;

    public:
        TcpSocket(int sock, const std::string& host, unsigned int port);
        TcpSocket(const std::string& host, unsigned int port);

        void Connect();
        void Close();
        int Send(const std::string& message) const;
        std::vector<char> Receive(int len) const;
        void Bind();
        void Listen();
        TcpSocket Accept();
        void CleanUp();

        const std::string Host() const;
        const unsigned int Port() const;
    };

}


#endif // _SOCKET_WINDOWS_HPP