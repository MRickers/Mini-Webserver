#include "ld_router.h"
#include <stdexcept>

namespace ld_webserver {
    void Router::Register(const std::string& name, HTTP_METHOD methode, const callback& func) {
        switch(methode) {
            case HTTP_METHOD::GET:
                m_get_routes.emplace(name, func);
                break;
            case HTTP_METHOD::POST:
                m_post_routes.emplace(name, func);
                break;
            case HTTP_METHOD::DEL:
                m_delete_routes.emplace(name, func);
                break;
            default:
                throw std::runtime_error{"invalid http method"};
        }
    }

    callback Router::Get(const std::string& name, HTTP_METHOD method) const {
        switch(method) {
            case HTTP_METHOD::GET:
                return m_get_routes.at(name);
            case HTTP_METHOD::POST:
                return m_post_routes.at(name);
            case HTTP_METHOD::DEL:
                return m_delete_routes.at(name);
        }
    }
}