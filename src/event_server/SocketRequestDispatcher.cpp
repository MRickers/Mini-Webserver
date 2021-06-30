#include "SocketRequestDispatcher.h"
#include "SocketEventHandler.h"

namespace webserver {
    SocketRequestDispatcher::SocketRequestDispatcher() : 
    m_handler(),
    m_socket_fds(),
    m_master_socket("localhost", 5000),
    m_read_fds(),
    m_write_fds(),
    m_logger(LogManager::GetLogger("Dispatcher")),
    m_tp(LogManager::GetLogger("Threadpool")) {
        for(int i=0;i<MAX_CLIENTS;i++) {
            m_socket_fds[i] = INVALID_SOCKET;
        }
        FD_ZERO(&m_read_fds);
        FD_ZERO(&m_write_fds);
        
        m_logger->Debug("Dispatcher ready");
        try {
            m_master_socket.Bind();
            m_master_socket.Listen();
            m_master_socket.SetSocketReuse();
        }catch(const socket_common::SocketException& e) {
            m_logger->Error(e.what());
        }
        FD_SET(m_master_socket.GetHandle(), &m_read_fds);
    }

    void SocketRequestDispatcher::RegisterHandler(EventHandler* handler, EventType et) {
        m_handler.emplace(et, handler);
    }

    void SocketRequestDispatcher::RemoveHandler(EventHandler* handler, EventType et) {

    }

    void SocketRequestDispatcher::HandleEvents() {
        m_logger->Debug("Handling events");
        while(true) {
            handleEvents();
        }
    }

    void SocketRequestDispatcher::handleEvents() {
        int max_fds = INVALID_SOCKET;
        max_fds = m_master_socket.GetHandle();

        int activity = select(max_fds+1, &m_read_fds, &m_write_fds, NULL, NULL);
        if(activity > 0) {
            if(FD_ISSET(m_master_socket.GetHandle(), &m_read_fds)) {
                m_logger->Debug("Master socket incoming message");
                auto new_sock = m_master_socket.Accept();
                m_logger->Debug("Accept called");
                std::unique_ptr<webserver::EventHandler> handler = std::make_unique<webserver::SocketHandler>(new_sock);
                m_tp.Push([handler = std::move(handler)]{
                    handler->HandleEvent();
                });
            } else {
                m_logger->Debug("Nothing on master socket");
            }
        } 
    }
}