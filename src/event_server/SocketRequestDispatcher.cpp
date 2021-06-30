#include "SocketRequestDispatcher.h"
#include "SocketEventHandler.h"
#include "Demultiplexer.h"
#include <chrono>
#include <thread>

namespace webserver {
    SocketRequestDispatcher::SocketRequestDispatcher() : 
    m_handler(),
    m_master_socket("localhost", 5000),
    m_read_fds(),
    m_logger(LogManager::GetLogger("Dispatcher")),
    m_tp(LogManager::GetLogger("Threadpool")) {
        
    }

    void SocketRequestDispatcher::RegisterHandler(EventHandler* handler, EventType et) {
        m_handler.emplace(et, handler);
    }

    void SocketRequestDispatcher::RemoveHandler(EventHandler* handler, EventType et) {

    }

    void SocketRequestDispatcher::HandleEvents() {
        multiplexer::Demultiplexer mltplx;

        m_master_socket.Bind();
        m_master_socket.Listen();
        m_master_socket.SetSocketReuse();

        m_read_fds.push_back(m_master_socket.GetHandle());
        m_logger->Debug("Master ===== socket="+std::to_string(m_master_socket.GetHandle()));
        m_logger->Debug(STREAM("Vector="<<m_read_fds.at(0)));
        m_logger->Debug("Handling events");
        while(true) {
            int fd = mltplx.Select(m_read_fds, std::vector<int>{}, std::vector<int>{}, 200);
            m_logger->Debug("fd="+std::to_string(fd));
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            if(fd == 0) {
                continue;
            }
            handleEvents(fd);
        }
    }

    void SocketRequestDispatcher::handleEvents(int fd) {
        m_logger->Debug("Master socket="+std::to_string(m_master_socket.GetHandle()));
        if(fd == m_master_socket.GetHandle()) {
            m_logger->Debug("New connection incoming");
            auto new_sock = m_master_socket.Accept();
            m_read_fds.push_back(new_sock.GetHandle());
        }
    }
}