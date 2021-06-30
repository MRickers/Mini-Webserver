#include "Demultiplexer.h"
#include <algorithm>

namespace multiplexer {

    Demultiplexer::Demultiplexer(Logger logger) :
    m_logger(logger) {

    }

    int Demultiplexer::Select(std::vector<int>& read_fds, std::vector<int>& write_fds, std::vector<int>& except_fds, uint16_t timeout_ms) {
        fd_set read_set, write_set, except_set;
        FD_ZERO(&read_set);
        FD_ZERO(&write_set);
        FD_ZERO(&except_set);

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

        m_logger->Debug("Calling select");

        int activity = select(max_fd+1, &read_set, &write_set, &except_set, &timeout);

        m_logger->Debug("Found activity on fd "+std::to_string(activity));
        return activity;
    }
}