#ifndef _LONELYDRIVER_WEBSERVER
#define _LONELYDRIVER_WEBSERVER
#include <memory>
#include "logger_interface.h"
#include "socket.h"
#include "thread_pool.h"

namespace ld_webserver {
    class Webserver {
    private:
        std::unique_ptr<socket_common::TcpSocket> _sock;
        std::unique_ptr<thread_pool::ThreadPool> _thread_pool;
        bool _running;

        void serverThread(socket_common::TcpSocket sock, Logger logging);
    public:
        Webserver(const Webserver&) = delete;
        Webserver operator=(const Webserver&) = delete;

        Webserver(int port);

        void StartServer();
    };
}




#endif // _LONELYDRIVER_WEBSERVER