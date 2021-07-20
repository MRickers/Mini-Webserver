#include "SocketEventHandler.h"

namespace webserver {
    
    SocketAcceptHandler::SocketAcceptHandler(Logger logger) : 
    m_logger(logger),
    EventHandler() {

    }

    int SocketAcceptHandler::HandleEvent(int fd) {
        socket_common::TcpSocket socket{fd};
        m_logger->Debug("Hello from Accept handler");
        auto new_socket =  socket.Accept();
        return new_socket.GetHandle();
    }

    SocketReadHandler::SocketReadHandler(Logger logger) : 
    m_logger(logger),
    EventHandler() {

    }

    int SocketReadHandler::HandleEvent(int fd) {
        auto buf = std::vector<char>{};
        buf.resize(0x0200);
        socket_common::TcpSocket socket{fd};

        m_logger->Debug("Receiving on socket "+std::to_string(socket.GetHandle()));
        try {
            const auto len = socket.Receive(buf, 0x0200);
            if(len == 0) {
                m_logger->Debug("Client shut down");
                socket.Close();
                return len;
            }else {
                buf.resize(len);
                const auto s = std::string(buf.begin(), buf.end());
                m_logger->Debug(STREAM("Received: "<<s));
                return fd;
            }
        }catch(const socket_common::SocketException& e) {
            m_logger->Error(STREAM("Receive failed: "<<e.what()));
            return -1;
        }
    }
}