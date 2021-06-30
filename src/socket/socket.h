#ifndef _SOCKET_HPP
#define _SOCKET_HPP
#include <stdexcept>
#include <string>

#if __linux__
#include "socket_linux.h"
#elif _WIN32
#include "socket_windows.h"
#endif

namespace socket_common {
    class SocketException : public std::exception {
    public:
        SocketException(const std::string& message, int socket_error) : 
        _message(message),
        _socket_error(socket_error){}

        virtual const char* what() const override {
            return _message.c_str();
        }

        const int SocketError() const {
            return _socket_error;
        }

    private:
        std::string _message;
        int _socket_error;
    };
}


#endif // _SOCKET_HPP