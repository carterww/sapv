#ifndef SAPV_HTTP_CLIENT_H
#define SAPV_HTTP_CLIENT_H

#include <memory>
#include <string>
#include <vector>

#include "http/request-headers.h"

namespace sapv {

class HttpClient {
 public:
  HttpClient() = default;

  virtual std::shared_ptr<std::string> get(const std::string_view url,
                                           HttpRequestHeaders& headers) = 0;
  virtual std::shared_ptr<std::string> post(const std::string_view url,
                                            const std::string_view data,
                                            HttpRequestHeaders& headers) = 0;
};

}  // namespace sapv

#endif  // SAPV_HTTP_CLIENT_H
