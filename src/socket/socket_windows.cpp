#include "socket_windows.h"

namespace socket_common {
    TcpSocket::TcpSocket(int sock, const std::string& host, unsigned int port) :
    _sock(sock),
    _host(host),
    _port(port) {

    }

    TcpSocket::TcpSocket(const std::string& host, unsigned int port) :
    _host(host),
    _port(port) {

    }

    void TcpSocket::Initialize() {
        WSADATA wsaData;
        int result = WSAStartUp(MAKEWORD(2,2), &wsaData);
        if(result != 0) {
            throw std::runtime_error("WSAStartup failed with error: "+std::to_string(result);
        }
    }

    void TcpSocket::CleanUp() {
        WSACleanUp();
    }

    void TcpSocket::Connect() {
        struct addrinfo *rp;
        const auto result = getHostInfo();

        for (rp = result; rp != NULL; rp = rp->ai_next) {
            _sock = socket(rp->ai_family, rp->ai_socktype,
                        rp->ai_protocol);
            if (_sock == -1)
                continue;

            if (connect(_sock, rp->ai_addr, rp->ai_addrlen) != -1)
                break;                  /* Success */

            Close();
        }
        freeaddrinfo(result);           /* No longer needed */

        if (rp == NULL) {               /* No address succeeded */
            throw std::runtime_error("could not connect");
        }
    }

    struct addrinfo* TcpSocket::getHostInfo() const {
        struct addrinfo hints;
        struct addrinfo *result;
        int s;

        std::memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;    /* Allow IPv4 or IPv6 */
        hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
        hints.ai_flags = 0;
        hints.ai_protocol = 0;          /* Any protocol */
        
        s = getaddrinfo(_host.c_str(), std::to_string(_port).c_str(), &hints, &result);
        if (s != 0) {
            throw std::runtime_error("host not found");
        }

        return result;
    }

    void TcpSocket::Close() {
        int ret = closesocket(_sock);
        if(ret < 0) {
            throw std::runtime_error("close socket failed");
        }
    }

    std::vector<unsigned char> TcpSocket::Receive(int len) const {
        std::vector<unsigned char> buffer;
        buffer.resize(len);
        int ret = recv(_sock, &buffer[0], len, 0);
        if(ret == 0) {
            buffer.resize(0);
        }else if(ret < 0) {
            throw std::runtime_error("recv failed: "+std::to_string(WSAGetLastError()));
        }
        return buffer;
    }

    int TcpSocket::Send(const std::string& message) const {
        int ret = send(_sock, message.data(), message.size(), 0);
        return ret;
    }

    void TcpSocket::Bind() {
        struct addrinfo *rp;
        const auto result = getHostInfo();

        for (rp = result; rp != NULL; rp = rp->ai_next) {
            _sock = socket(rp->ai_family, rp->ai_socktype,
                        rp->ai_protocol);
            if (_sock == -1)
                continue;

            if (bind(_sock, rp->ai_addr, rp->ai_addrlen) == 0)
                break;                  /* Success */

            Close();
        }
        freeaddrinfo(result);           /* No longer needed */

        if (rp == NULL) {               /* No address succeeded */
            throw std::runtime_error("could not bind");
        }
    }

    void TcpSocket::Listen() {
        int ret = listen(_sock, 50);
        if(ret == -1) {
            throw std::runtime_error("listen failed");
        }
    }

    TcpSocket TcpSocket::Accept() {
        struct sockaddr peer_addr;
        socklen_t peer_len;

        peer_len = sizeof(peer_addr);
        int sock = accept(_sock, (struct sockaddr*)& peer_addr, &peer_len);
        
        return TcpSocket(sock, Host(), Port());
    }

    const std::string TcpSocket::Host() const {
        return _host;
    }

    const unsigned int TcpSocket::Port() const {
        return _port;
    }
}