#include "logging_manager.h"
#include "socket.h"

int main() {
    const auto logger = LogManager::GetLogger("Main");
    logger->Debug("Hello World");
    socket_common::TcpSocket tcp_socket("localhost", 5000);

    tcp_socket.Bind();
    tcp_socket.Listen();

    while (true)
    {
        auto new_sock = tcp_socket.Accept();
        while(true) {
            const auto buf = new_sock.Receive(0x0200);
            if(buf.size() == 0) {
                logger->Debug("Client shutdown");
                break;
            }
            const auto s = std::string(buf.begin(), buf.end());
            logger->Debug(STREAM("Received: "<<s));
            new_sock.Send(s);
        }
        new_sock.Close();
        logger->Debug("Waiting for new connection");
    }
    

    return 0;
}