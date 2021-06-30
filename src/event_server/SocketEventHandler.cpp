#include "SocketEventHandler.h"

namespace webserver {
    SocketHandler::SocketHandler(const socket_common::TcpSocket socket, Logger logger) : 
    m_logger(logger),
    m_socket(socket),
    EventHandler() {

    }

    void SocketHandler::HandleEvent() {
        auto buf = std::vector<char>{};
        buf.resize(0x0200);
        m_logger->Debug("Handle Event");
        try {
            const auto len = m_socket.Receive(buf, 0x0200);
            if(len == 0) {
                m_logger->Debug("Client shut down");
            }else {
                buf.resize(len);
                const auto s = std::string(buf.begin(), buf.end());
                m_logger->Debug(STREAM("Received: "<<s));
            }
        }catch(const socket_common::SocketException& e) {
            m_logger->Error(STREAM("Receive failed: "<<e.what()));
        }
    }
}