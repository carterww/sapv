#include "http/curl-http-client.h"

#include <cstring>
#include <iostream>
#include <memory>
#include <stdexcept>

#include <curl/curl.h>

#include "http/http-client.h"
#include "http/request-headers.h"
#include "utils/string.h"

namespace sapv {
namespace curl {

CurlHttpClient::CurlHttpClient() : HttpClient() {
  CURLcode code;
  if ((code = curl_global_init(CURL_GLOBAL_ALL)) != CURLE_OK) {
    std::string error = "Failed to globally initialize curl: ";
    error += curl_easy_strerror(code);
    throw std::runtime_error(error);
  }
  this->curl = curl_easy_init();
  if (this->curl == nullptr) {
    throw std::runtime_error("Failed to initialize curl");
  }
}

CurlHttpClient::~CurlHttpClient() {
  curl_global_cleanup();
}

std::shared_ptr<std::string> CurlHttpClient::get(const std::string_view url,
                                                 HttpRequestHeaders& headers) {
  static CurlWriteDataBuffer buffer = {0};
  this->curl = curl_easy_init();
  curl_easy_setopt(this->curl, CURLOPT_URL, url.data());
  curl_easy_setopt(this->curl, CURLOPT_HTTPGET, 1L);

  std::string accepted_encodings = headers.get_header("Accept-Encoding");
  if (!accepted_encodings.empty()) {
    headers.remove_header("Accept-Encoding");
    std::unique_ptr<std::vector<std::string>> encodings =
        utils::string::split(accepted_encodings, ", ");
    // Set encodings. Can I do multiple encodings?
    for (int i = 0; i < encodings->size(); i++) {
      curl_easy_setopt(this->curl, CURLOPT_ACCEPT_ENCODING,
                       encodings->at(i).data());
    }
  }
  for (int i = 0; i < headers.size(); i++) {
    curl_easy_setopt(this->curl, CURLOPT_HTTPHEADER, headers[i].data());
  }

  curl_easy_setopt(this->curl, CURLOPT_WRITEFUNCTION, curl_write_callback);
  std::shared_ptr<std::string> data = std::make_shared<std::string>();
  curl_easy_setopt(this->curl, CURLOPT_WRITEDATA, (void*)&data);

  CURLcode opcode = curl_easy_perform(this->curl);
  if (opcode != CURLE_OK) {
    std::string error = "Failed to perform curl request: ";
    error += curl_easy_strerror(opcode);
    throw std::runtime_error(error);
  }

  // Add this back
  headers.add_header("Accept-Encoding", accepted_encodings);
  curl_easy_cleanup(this->curl);
  return data;
}

std::shared_ptr<std::string> CurlHttpClient::post(const std::string_view url,
                                                  const std::string_view data,
                                                  HttpRequestHeaders& headers) {
  return std::make_unique<std::string>("Not implemented");
}

size_t curl_write_callback(char* ptr, size_t size, size_t nmemb, void* data) {
  size_t realsize = size * nmemb;
  std::shared_ptr<std::string>* buffer = (std::shared_ptr<std::string>*)data;
  (*buffer)->append(ptr, realsize);
  (*buffer)->append("\0", 1);


  return realsize;
}

}  // namespace curl
}  // namespace sapv
