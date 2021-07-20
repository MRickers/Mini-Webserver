#include "SocketRequestDispatcher.h"
#include "SocketEventHandler.h"
#include "Demultiplexer.h"
#include <chrono>
#include <thread>
#include <errno.h>
#include <string.h>

namespace webserver {
    SocketRequestDispatcher::SocketRequestDispatcher() : 
    m_handler(),
    m_master_socket("localhost", 5000),
    m_read_fds(),
    m_write_fds(),
    m_except_fds(),
    m_logger(LogManager::GetLogger("Dispatcher")),
    m_tp(LogManager::GetLogger("Threadpool")) {
        socket_common::TcpSocket::Init();
    }

    void SocketRequestDispatcher::RegisterHandler(EventHandlerPtr handler, EventType et) {
        m_handler.emplace(et, handler);
    }

    void SocketRequestDispatcher::RemoveHandler(EventType et) {
        if(m_handler.erase(et)) {
            m_logger->Debug("Handler "+std::to_string(static_cast<int>(et))+" removed");
        }
    }

    void SocketRequestDispatcher::HandleEvents() {
        m_master_socket.Bind();
        m_master_socket.Listen();
        m_master_socket.SetSocketReuse();

        m_read_fds.push_back(m_master_socket.GetHandle());

        m_logger->Debug("Listening on localhost port "+std::to_string(m_master_socket.Port()));

        while(true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            handleEvents();
        }
    }

    void SocketRequestDispatcher::handleEvents() {
        multiplexer::Demultiplexer multiplexer;
        const auto activity = multiplexer.Select(m_read_fds, m_write_fds, m_except_fds, 50);
        m_logger->Debug("Master socket="+std::to_string(m_master_socket.GetHandle()));
        m_logger->Debug("Activity="+std::to_string(activity));
        switch(activity) {
            case -1:
                m_logger->Error("Select error: "+std::to_string(WSAGetLastError()));
                break;
            case 0:
                m_logger->Debug("Select timeout");
                break;
            default:
                for(auto fd : multiplexer.GetReadFds()) {
                    if(fd == m_master_socket.GetHandle()) {
                        m_logger->Debug("Accept");
                        const auto handler = m_handler.at(EventType::Accept);

                        m_tp.Push([handler, fd, &read_fds=this->m_read_fds]{
                            int new_fd = handler->HandleEvent(fd);
                            if(new_fd > 0) {
                                read_fds.push_back(new_fd);
                            }
                        });
                        m_read_fds.push_back(fd);
                    }else {
                        try {
                            m_logger->Debug("Read");
                            const auto handler = m_handler.at(EventType::Read);

                            m_tp.Push([handler, fd, &read_fds=this->m_read_fds]{
                                int new_fd = handler->HandleEvent(fd);
                                if(new_fd > 0) {
                                    read_fds.push_back(new_fd);
                                }
                            });

                        } catch(const std::out_of_range& e) {
                            m_logger->Warn("No Read handler found");
                        }
                    }
                }

                for(const auto fd : m_write_fds) {

                }

                for(const auto fd : m_except_fds) {
                    
                }
        }
    }
}