#ifndef _SOCKET_LINUX_HPP
#define _SOCKET_LINUX_HPP
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <vector>


namespace socket_common {

    class TcpSocket {
    private:
        int _sock;
        std::string _host;
        unsigned int _port;

        struct addrinfo* getHostInfo() const;

    public:
        TcpSocket(int sock, const std::string& host, unsigned int port);
        TcpSocket(const std::string& host, unsigned int port);

        void Connect();
        void Close();
        int Send(const std::string& message) const;
        std::vector<unsigned char> Receive(int len) const;
        void Bind();
        void Listen();
        TcpSocket Accept();
        void CleanUp() {}
        void Set_Socket_Reuse() {}

        const std::string Host() const;
        const unsigned int Port() const;
    };
} // socket_common


#endif // _SOCKET_LINUX_HPP