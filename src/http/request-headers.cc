#include "http/request-headers.h"

#include <fstream>
#include <string>
#include <cstdint>

#include "nlohmann/json.hpp"

typedef nlohmann::json json;

namespace sapv {

HttpRequestHeaders::HttpRequestHeaders(const std::string_view path_to_headers_file) {
    std::ifstream headers_file(path_to_headers_file.data());
    json data = json::parse(headers_file);
    for (auto& [key, value] : data.items()) {
        headers.push_back(this->make_header(key, value.get<std::string>()));
    }
}

std::string HttpRequestHeaders::get_header(const std::string_view header) const {
    std::string result;
    int index = this->find_header(header);
    if (index != -1) {
        auto pos = header.find(':');
        result = headers[index].substr(pos + 2); // 2 because of ": "
    }
    return result;
}

void HttpRequestHeaders::add_header(const std::string_view header, const std::string_view value) {
    headers.push_back(this->make_header(header, value));
}

bool HttpRequestHeaders::update_header(const std::string_view header, const std::string_view value) {
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
        auto pos = header.find(':');
        result = headers[index].substr(pos + 2);
        headers.erase(headers.begin() + index);
    }
    return result;
}

std::string HttpRequestHeaders::operator [](int index) const {
    return headers[index];
}

size_t HttpRequestHeaders::size() const {
    return headers.size();
}

std::string HttpRequestHeaders::make_header(const std::string_view header,
        const std::string_view value) const {
    return std::string(header) + ": " + std::string(value);
}

int HttpRequestHeaders::find_header(const std::string_view header) const {
    for (int i = 0; i < headers.size(); i++) {
        auto pos = header.find(':');
        if (pos == std::string::npos)
            continue;

        std::string key = headers[i].substr(0, pos);
        if (key == header)
            return i;
    }
    return -1;
}


} // namespace sapv
