#include "logging_manager.h"
#include <SocketRequestDispatcher.h>
#include <SocketEventHandler.h>
#include <memory>

int main() {
    Logger logger = LogManager::GetLogger("Main");
    webserver::SocketRequestDispatcher reactor{};

    EventHandlerPtr socket_accept_handler = std::make_shared<webserver::SocketAcceptHandler>();
    EventHandlerPtr socket_read_handler = std::make_shared<webserver::SocketReadHandler>();
    reactor.RegisterHandler(socket_accept_handler, EventType::Accept);
    reactor.RegisterHandler(socket_read_handler, EventType::Read);

    reactor.HandleEvents();
    logger->Debug("Done");
    return 0;
}