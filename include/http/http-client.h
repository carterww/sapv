#ifndef SAPV_HTTP_CLIENT_H
#define SAPV_HTTP_CLIENT_H

#include "http/request-headers.h"
#include <memory>
#include <vector>
#include <string>

namespace sapv {

class HttpClient {
public:
    HttpClient();
    virtual ~HttpClient() = 0;

    virtual std::unique_ptr<std::string> get(const std::string_view url, HttpRequestHeaders &headers) = 0;
    virtual std::unique_ptr<std::string> post(const std::string_view url, const std::string_view data,
            HttpRequestHeaders &headers) = 0;
};

} // namespace sapv

#endif // SAPV_HTTP_CLIENT_H
