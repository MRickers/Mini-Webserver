#ifndef _LD_ROUTER_HPP
#define _LD_ROUTER_HPP
#include <functional>
#include <unordered_map>
#include <string>
#include "ld_request.h"

typedef std::function<void()> callback;

namespace ld_webserver {

    class Router {
    private:
        std::unordered_map<std::string, callback> m_get_routes;
        std::unordered_map<std::string, callback> m_post_routes;
        std::unordered_map<std::string, callback> m_delete_routes;
    public:
        Router(const Router&) = delete;
        Router operator=(const Router&) = delete;

        Router() = default;

        void Register(const std::string& name, HTTP_METHOD methode, const callback& func);
        callback Get(const std::string& name, HTTP_METHOD method) const;
    };
}



#endif // _LD_ROUTER_HPP