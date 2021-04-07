#include "ld_webserver.h"
#include "ld_timer.h"

namespace ld_webserver {
    Webserver::Webserver(int port) :
    _sock(std::make_unique<socket_common::TcpSocket>(port)),
    _thread_pool(std::make_unique<thread_pool::ThreadPool>()),
    _running(false) {

    }

    void Webserver::serverThread(socket_common::TcpSocket sock, Logger logging) {
        std::vector<char> buffer;

        buffer.resize(0x0200);

        const auto len = sock.Receive(buffer, buffer.size());
        logging->Debug("Received: "+std::string{buffer.begin(), buffer.end()});
        logging->Debug("Root url and invoke function");
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
    }

    void Webserver::StartServer() {
        Logger logging = LogManager::GetLogger("Webserver");

        logging->Debug(STREAM("Creating listener on port"<<_sock->Port()));

        try {
            _sock->Bind();
            _sock->Listen();

            _running = true;
            while(_running) {
                auto new_sock = _sock->Accept();
                _thread_pool->Push([this, new_sock, logging] {
                    ld_utils::Timer timer{std::chrono::high_resolution_clock::now()};
                    this->serverThread(new_sock, logging);
                    logging->Debug(STREAM("Response time: "<<timer.Stop()<<" ms"));
                });
            }
        }catch(const socket_common::SocketException& e) {
            logging->Error(STREAM(e.what()<<" with code "<<e.SocketError()));
        }

    }
}