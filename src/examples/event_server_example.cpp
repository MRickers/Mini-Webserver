#include "logging_manager.h"
#include <SocketRequestDispatcher.h>

int main() {
    Logger logger = LogManager::GetLogger("Main");
    webserver::SocketRequestDispatcher reactor{};
    logger->Debug("event server example");
    reactor.HandleEvents();
    logger->Debug("Done");
    return 0;
}