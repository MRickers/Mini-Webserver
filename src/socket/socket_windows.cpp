#include "socket.h"
#include <stdexcept>
#include <iostream>
#include "logging_manager.h"

Logger logging = LogManager::GetLogger("Socket");


namespace socket_common {
    TcpSocket::TcpSocket(unsigned int port) : TcpSocket{"", port} {

    }

    TcpSocket::TcpSocket(int sock, const std::string& host, unsigned int port) :
    _sock(sock),
    _host(host),
    _port(port) {

    }

    TcpSocket::TcpSocket(const std::string& host, unsigned int port) :
    _host(host),
    _port(port) {
        WSADATA wsadata;

        if(WSAStartup(WINSOCK_VERSION, &wsadata) != NO_ERROR) {
            logging->Error(STREAM("Initializing sockets failed: "<< WSAGetLastError()));
            CleanUp();
            throw SocketException(std::string{"initializing sockets failed: "+std::to_string(WSAGetLastError())}, WSAGetLastError());
        }
    }

    void TcpSocket::CleanUp() {
        WSACleanup();
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
            throw SocketException(std::string{"could not connect: "+std::to_string(WSAGetLastError())}, WSAGetLastError());
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
        hints.ai_protocol = IPPROTO_TCP;          /* Any protocol */
        
        s = getaddrinfo(_host.c_str(), std::to_string(_port).c_str(), &hints, &result);
        if (s != 0) {
            throw SocketException(std::string{"host not found: "+std::to_string(WSAGetLastError())}, WSAGetLastError());
        }

        return result;
    }

    void TcpSocket::Close() {
        int ret = closesocket(_sock);
        if(ret < 0) {
            throw SocketException(std::string{"close socket failed: "+std::to_string(WSAGetLastError())}, WSAGetLastError());
        }
    }

    int TcpSocket::Receive(std::vector<char>& buffer, int len) const {
        int ret = recv(_sock, &buffer[0], len, 0);
        if(ret == 0) {
            logging->Debug("peer shutdown");
        }else if(ret < 0) {
            throw SocketException(std::string{"recv failed: "+std::to_string(WSAGetLastError())}, WSAGetLastError());
        }
        return ret;
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
            throw SocketException{"could not bind", -1};
        }
    }

    void TcpSocket::Listen() {
        int ret = listen(_sock, 0);
        if(ret == -1) {
            throw SocketException{"listen failed", -2};
        }
    }

    TcpSocket TcpSocket::Accept() {
        struct sockaddr_in peer_addr;
        int peer_len;

        peer_len = sizeof(peer_addr);
        int sock = accept(_sock, (struct sockaddr*)& peer_addr, &peer_len);

        logging->Debug(STREAM("Connected to client with address: "<<inet_ntoa(peer_addr.sin_addr)));
        
        return TcpSocket(sock, Host(), Port());
    }

    const std::string TcpSocket::Host() const {
        return _host;
    }

    const unsigned int TcpSocket::Port() const {
        return _port;
    }

    // https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-setsockopt
    void TcpSocket::SetSocketReuse() {
        int ret = 0;
        const char y = 1;
        if ((ret = setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(y))) != 0) {
            throw SocketException(std::string{"Set socket reuse failed: "+ret}, ret);
        }
    }

    void TcpSocket::Debug(bool flag) {
        if(flag) {
           logging->LoggingLevel(logger::Loglevel::L_DEBUG);
        }else {
            logging->LoggingLevel(logger::Loglevel::L_ERROR);
        }
    }

    SOCK TcpSocket::GetHandle() const {
        return _sock;
    }

}