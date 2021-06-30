#ifndef _EVENT_SERVER_HPP
#define _EVENT_SERVER_HPP

#include "logging_manager.h"
#include "EventHandlerInterface.h"


namespace webserver {
    class Reactor {
    private:
        Logger m_logger;
    public:
        void RegisterHandler(const EventHandler* handler, EventType event);
        void RemoveHandler(const EventHandler* handler);
        void HandleEvents();
    };  
}








#endif // _EVENT_SERVER_HPP