#include "ld_webserver.h"
#include "ld_timer.h"

namespace ld_webserver {
    Webserver::Webserver(int port) :
    m_sock(std::make_unique<socket_common::TcpSocket>(port)),
    m_thread_pool(std::make_unique<thread_pool::ThreadPool>()),
    m_running(false),
    m_router() {

    }

    void Webserver::serverThread(const socket_common::TcpSocket& sock, Logger logging) {
        std::vector<char> buffer;

        buffer.resize(0x0200);

        const auto len = sock.Receive(buffer, buffer.size());
        logging->Debug("Received: "+std::string{buffer.begin(), buffer.end()});
        logging->Debug("Root url and invoke function");
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
        
    }

    void Webserver::StartServer() {
        Logger logging = LogManager::GetLogger("Webserver");

        logging->Debug(STREAM("Creating listener on port"<<m_sock->Port()));

        try {
            m_sock->Bind();
            m_sock->SetSocketReuse();
            m_sock->Listen();

            m_running = true;
            while(m_running) {
                auto new_sock = m_sock->Accept();
                m_thread_pool->Push([this, new_sock, logging] {
                    ld_utils::Timer timer{std::chrono::high_resolution_clock::now()};
                    this->serverThread(new_sock, logging);
                    logging->Debug(STREAM("Response time: "<<timer.Stop()<<" ms"));
                });
            }
        }catch(const socket_common::SocketException& e) {
            logging->Error(STREAM(e.what()<<" with code "<<e.SocketError()));
        }
    }

    void Webserver::Register(const std::string& name, HTTP_METHOD method, callback func) {
        m_router.Register(name, method, func);
    }
}