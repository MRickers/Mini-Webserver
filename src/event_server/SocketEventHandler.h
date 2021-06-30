#ifndef _SOCKET_EVENT_Handler_HPP
#define _SOCKET_EVENT_Handler_HPP

#include "logging_manager.h"
#include "EventHandlerInterface.h"
#include "socket.h"

namespace webserver {
    class SocketHandler : public EventHandler {
    private:
        Logger  m_logger;
        socket_common::TcpSocket m_socket;
    public:
        SocketHandler(const socket_common::TcpSocket socket, Logger logger=LogManager::GetLogger("SocketHandler"));
        virtual void HandleEvent() override;

    };
}






#endif // _SOCKET_EVENT_HANDLER_HPP