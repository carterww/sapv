#ifndef SAPV_CURL_HTTP_CLIENT_H
#define SAPV_CURL_HTTP_CLIENT_H

#include <curl/curl.h>
#include <memory>

#include "http/http-client.h"
#include "http/request-headers.h"

namespace sapv {
namespace curl {

class CurlHttpClient : public HttpClient {
 public:
  CurlHttpClient();
  ~CurlHttpClient();

  std::unique_ptr<std::string> get(const std::string_view url,
                                   HttpRequestHeaders& headers) override;
  std::unique_ptr<std::string> post(const std::string_view url,
                                    const std::string_view data,
                                    HttpRequestHeaders& headers) override;

 private:
  CURL* curl;
};

struct CurlWriteDataBuffer {
  char* data;
  size_t size;
};

size_t curl_write_callback(char* ptr, size_t size, size_t nmemb, void* buff);

}  // namespace curl
}  // namespace sapv

#endif  // SAPV_CURL_HTTP_CLIENT_H
