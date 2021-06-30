#ifndef _DEMULTIPLEXER_HPP
#define _DEMULTIPLEXER_HPP
#include "logging_manager.h"
#include "socket.h"
#include "EventHandlerInterface.h"
#include <vector>

namespace multiplexer {

    class Demultiplexer {
    private:
        Logger m_logger;
    public:
        Demultiplexer(Logger logger=LogManager::GetLogger("Mltplx"));

        int Select(
            std::vector<int>& m_read_fds,
            std::vector<int>& m_write_fds,
            std::vector<int>& m_except_fds,
            uint16_t timeout_ms);
    };
}







#endif //_DEMULTIPLEXER_HPP