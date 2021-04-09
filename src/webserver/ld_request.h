#ifndef _LD_HTTP_REQUEST_H
#define _LD_HTTP_REQUEST_H
#include <vector>
#include <string>
#include "ld_http_utils.h"

namespace ld_webserver {
    
    class HttpRequest {
    private:
        HTTP_METHOD m_method;
        std::string m_host;
        std::string m_version;
        std::string m_mime_type;
        std::size_t m_payload_length;
        std::string m_payload;

    public:
        HttpRequest(const std::string& request);
    };
}


#endif // _LD_HTTP_REQUEST_H