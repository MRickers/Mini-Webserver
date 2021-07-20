#ifndef _DEMULTIPLEXER_HPP
#define _DEMULTIPLEXER_HPP
#include "logging_manager.h"
#include "socket.h"
#include "EventHandlerInterface.h"
#include <vector>

namespace multiplexer {

    bool IsFdSet(int fd, const fd_set& set);

    class Demultiplexer {
    private:
        Logger m_logger;
        std::vector<int> m_read_ready;
        std::vector<int> m_write_ready;
        std::vector<int> m_except_ready;

    public:
        Demultiplexer(Logger logger=LogManager::GetLogger("Mltplx"));

        const int  Select(
            std::vector<int>& m_read_fds,
            std::vector<int>& m_write_fds,
            std::vector<int>& m_except_fds,
            uint16_t timeout_ms);

        const std::vector<int> GetReadFds();
        const size_t FdReadCount() const;
    };
}







#endif //_DEMULTIPLEXER_HPP