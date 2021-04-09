#ifndef _LD_HTTP_UTILS_H
#define _LD_HTTP_UTILS_H

namespace ld_webserver {
    enum class HTTP_METHOD {
            GET,
            POST,
            DEL
        };

    enum class HTTP_ERROR_CODES : int {
        OK=200,

        BAD_REQUEST=400,
        NOT_IMPLEMENTED=501,
    };
}



#endif //_LD_HTTP_UTILS_H