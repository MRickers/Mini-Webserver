#ifndef _SOCKET_REQUEST_HANDLER_HPP
#define _SOCKET_REQUEST_HANDLER_HPP

#include <unordered_map>
#include <vector>
#include <logging_manager.h>
#include <EventDispatcher.h>
#include <socket.h>
#include <thread_pool.h>

namespace webserver {
    class SocketRequestDispatcher : public EventDispatcher {
        private:
            static const inline int MAX_CLIENTS = 12;
            std::unordered_map<EventType, EventHandler*> m_handler;
            SOCK m_socket_fds[MAX_CLIENTS];
            socket_common::TcpSocket m_master_socket;
            fd_set m_read_fds;
            fd_set m_write_fds;
            Logger m_logger;
            thread_pool::ThreadPool m_tp;
            
            void handleEvents();
        public:
            SocketRequestDispatcher();
            void RegisterHandler(EventHandler* handler, EventType et) override;
            void RemoveHandler(EventHandler* handler, EventType et) override;
            void HandleEvents() override;
    };
}












#endif // _SOCKET_REQUEST_HANDLER_HPP