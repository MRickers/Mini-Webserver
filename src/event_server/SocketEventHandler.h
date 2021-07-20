#ifndef _SOCKET_EVENT_Handler_HPP
#define _SOCKET_EVENT_Handler_HPP

#include "logging_manager.h"
#include "EventHandlerInterface.h"
#include "socket.h"

namespace webserver {
    class SocketAcceptHandler : public EventHandler {
    private:
        Logger  m_logger;
    public:
        SocketAcceptHandler(Logger logger=LogManager::GetLogger("SocketAcceptHandler"));
        virtual int HandleEvent(int fd) override;

    };

    class SocketReadHandler : public EventHandler {
    private:
        Logger  m_logger;
    public:
        SocketReadHandler(Logger logger=LogManager::GetLogger("SocketReadHandler"));
        virtual int HandleEvent(int fd) override;

    };
}






#endif // _SOCKET_EVENT_HANDLER_HPP