#include "Demultiplexer.h"
#include <algorithm>
#include <functional>

namespace multiplexer {

    Demultiplexer::Demultiplexer(Logger logger) :
    m_logger(logger),
    m_read_ready(),
    m_write_ready(),
    m_except_ready() {

    }

    const int Demultiplexer::Select(std::vector<int>& read_fds, std::vector<int>& write_fds, std::vector<int>& except_fds, uint16_t timeout_ms) {
        fd_set read_set, write_set, except_set;
        FD_ZERO(&read_set);
        FD_ZERO(&write_set);
        FD_ZERO(&except_set);

        m_logger->Debug("Hi from select");
        for(const auto fd : read_fds) {
            m_logger->Debug("FD="+std::to_string(fd));
            FD_SET(fd, &read_set);
        }
        for(const auto fd : write_fds) {
            FD_SET(fd, &write_set);
        }
        for(const auto fd : except_fds) {
            FD_SET(fd, &except_set);
        }
        // find max fd
        int max_read = 0;
        if(read_fds.empty() == false) {
            max_read = *std::max_element(read_fds.begin(), read_fds.end());
        }
        int max_write = 0;
        if(write_fds.empty() == false) {
            max_write = *std::max_element(write_fds.begin(), write_fds.end());
        }
        int max_except = 0;
        if(except_fds.empty() == false) {
            max_except = *std::max_element(except_fds.begin(), except_fds.end());
        }

        int max_fd = std::max(max_read, std::max(max_write, max_except));
        timeval timeout = {0x00};

        timeout.tv_sec = timeout_ms % 1000;
        timeout.tv_usec = timeout_ms * 1000;

        int activity = select(max_fd+1, &read_set, &write_set, &except_set, &timeout);

        std::copy_if(
            read_fds.begin(),
            read_fds.end(),
            std::back_inserter(m_read_ready), std::bind(&IsFdSet, std::placeholders::_1, read_set));
        
        read_fds.erase(std::remove_if(read_fds.begin(), read_fds.end(),
                        std::bind(&IsFdSet, std::placeholders::_1, read_set)), read_fds.end());

        m_logger->Debug("Ready size="+std::to_string(m_read_ready.size()));
        m_logger->Debug("Read size="+std::to_string(read_fds.size()));
        return activity;
    }

    const std::vector<int> Demultiplexer::GetReadFds() {
        const auto read_fds = m_read_ready;
        m_read_ready.clear();
        return read_fds;
    }

    const size_t Demultiplexer::FdReadCount() const {
        return m_read_ready.size();
    }

    bool IsFdSet(int fd, const fd_set& set) {
        if(FD_ISSET(fd, &set)) {
            return true;
        }
        return false;
    }
}
