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
            std::unordered_map<EventType, EventHandlerPtr> m_handler;
            socket_common::TcpSocket m_master_socket;
            std::vector<int> m_read_fds;
            std::vector<int> m_write_fds;
            std::vector<int> m_except_fds;
            Logger m_logger;
            thread_pool::ThreadPool m_tp;
            
            void handleEvents();
        public:
            SocketRequestDispatcher();
            void RegisterHandler(EventHandlerPtr handler, EventType et) override;
            void RemoveHandler(EventType et) override;
            void HandleEvents() override;
    };
}












#endif // _SOCKET_REQUEST_HANDLER_HPP