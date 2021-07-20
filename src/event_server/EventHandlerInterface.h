#ifndef _EVENT_Handler_INTERFACE_HPP
#define _EVENT_Handler_INTERFACE_HPP

#include <memory>
#include "logging_manager.h"

enum class EventType {
    Accept  = 0x01,
    Read    = 0x02,
    Write   = 0x04,
    Close   = 0x08,
};

namespace webserver {
    class EventHandler {
    public:
        virtual ~EventHandler() {}

        virtual int HandleEvent(int fd) = 0;

    };
}

using EventHandlerPtr = std::shared_ptr<webserver::EventHandler>;






#endif // _EVENT_Handler_INTERFACE_HPP