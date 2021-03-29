#include "logging_manager.h"
#include "socket.h"

int main() {
    bool running = true;
    const auto logger = LogManager::GetLogger("Main");
    
    socket_common::TcpSocket tcp_socket("localhost", 5000);

    tcp_socket.Bind();
    tcp_socket.Listen();
    logger->Debug("Listening on "+tcp_socket.Host()+" with port "+std::to_string(tcp_socket.Port()));
    while (running)
    {
        auto new_sock = tcp_socket.Accept();
        while(running) {
            const auto buf = new_sock.Receive(0x0200);
            if(buf.size() == 0) {
                logger->Debug("Client shutdown");
                break;
            }
            const auto s = std::string(buf.begin(), buf.end());
            logger->Debug(STREAM("Received: "<<s));
            if(s.find("quit") != std::string::npos) {
                logger->Debug("Cya");
                running = false;
            }
            new_sock.Send(s);
        }
        new_sock.Close();
        logger->Debug("Waiting for new connection");
    }
    tcp_socket.CleanUp();
    

    return 0;
}