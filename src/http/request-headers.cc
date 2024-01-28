#include "http/request-headers.h"

#include <cstdint>
#include <fstream>
#include <string>

#include <nlohmann/detail/value_t.hpp>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

namespace sapv {

HttpRequestHeaders::HttpRequestHeaders(
    const std::string_view path_to_headers_file) {
  using json = nlohmann::json;

  typedef nlohmann::detail::value_t value_t;
  std::ifstream headers_file(path_to_headers_file.data());
  json data = json::parse(headers_file);
  std::string value_as_string;

  for (auto& [key, value] : data.items()) {
    // Get string of value
    switch (value.type()) {
      case value_t::null:
      case value_t::object:
      case value_t::array:
      case value_t::binary:
      case value_t::discarded:
        // All these types can't really be headers (at least ones I will use)
        break;
      case value_t::number_float:
        value_as_string = std::to_string(value.get<double>());
        break;
      case value_t::number_integer:
        value_as_string = std::to_string(value.get<int64_t>());
        break;
      case value_t::number_unsigned:
        value_as_string = std::to_string(value.get<uint64_t>());
        break;
      case value_t::boolean:
        value_as_string = std::to_string(value.get<bool>());
        break;
      case value_t::string:
        value_as_string = value.get<std::string>();
        break;
    }
    if (!value_as_string.empty()) {
      headers.push_back(this->make_header(key, value_as_string));
    }
  }
}

HttpRequestHeaders::~HttpRequestHeaders() {}

std::string HttpRequestHeaders::get_header(
    const std::string_view header) const {
  std::string result;
  int index = this->find_header(header);
  if (index != -1) {
    auto pos = headers[index].find(':');
    result = headers[index].substr(pos + 2);  // 2 because of ": "
  }
  return result;
}

void HttpRequestHeaders::add_header(const std::string_view header,
                                    const std::string_view value) {
  headers.push_back(this->make_header(header, value));
}

bool HttpRequestHeaders::update_header(const std::string_view header,
                                       const std::string_view value) {
  int index = this->find_header(header);
  if (index != -1) {
    headers[index] = this->make_header(header, value);
    return true;
  }
  return false;
}

std::string HttpRequestHeaders::remove_header(const std::string_view header) {
  std::string result;
  int index = this->find_header(header);
  if (index != -1) {
    auto pos = headers[index].find(':');
    result = headers[index].substr(pos + 2);
    headers.erase(headers.begin() + index);
  }
  return result;
}

std::string HttpRequestHeaders::operator[](int index) const {
  return headers[index];
}

size_t HttpRequestHeaders::size() const {
  return headers.size();
}

std::string HttpRequestHeaders::make_header(
    const std::string_view header, const std::string_view value) const {
  return std::string(header) + ": " + std::string(value);
}

int HttpRequestHeaders::find_header(const std::string_view header) const {
  for (int i = 0; i < headers.size(); i++) {
    auto pos = headers[i].find(':');
    if (pos == std::string::npos)
      continue;

    std::string key = headers[i].substr(0, pos);
    if (key == header)
      return i;
  }
  return -1;
}

}  // namespace sapv
