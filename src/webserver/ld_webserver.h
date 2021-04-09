#ifndef _LONELYDRIVER_WEBSERVER
#define _LONELYDRIVER_WEBSERVER
#include <memory>
#include "logger_interface.h"
#include "socket.h"
#include "thread_pool.h"
#include "ld_router.h"

namespace ld_webserver {
    class Webserver {
    private:
        std::unique_ptr<socket_common::TcpSocket> m_sock;
        std::unique_ptr<thread_pool::ThreadPool> m_thread_pool;
        bool m_running;
        Router m_router;

        void serverThread(const socket_common::TcpSocket& sock, Logger logging);
    public:
        Webserver(const Webserver&) = delete;
        Webserver operator=(const Webserver&) = delete;

        Webserver(int port);
        void Register(const std::string& name, HTTP_METHOD method, callback func);
        void StartServer();
    };
}




#endif // _LONELYDRIVER_WEBSERVER