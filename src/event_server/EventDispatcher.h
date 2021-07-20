#ifndef _EVENT_DISPATCHER_BASE_HPP
#define _EVENT_DISPATCHER_BASE_HPP
#include "EventHandlerInterface.h"

namespace webserver
{
    class EventDispatcher {
    public:
        virtual ~EventDispatcher() {}
        virtual void RegisterHandler(EventHandlerPtr handler, EventType et) = 0;
        virtual void RemoveHandler(EventType et) = 0;
        virtual void HandleEvents() = 0;
    };
} // namespace webserver











#endif // _EVENT_DISPATCHER_BASE_HPP