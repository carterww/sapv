#ifndef SAPV_HTTP_REQUEST_HEADERS_H
#define SAPV_HTTP_REQUEST_HEADERS_H

#include <cstddef>
#include <string>
#include <vector>

namespace sapv {

class HttpRequestHeaders {
public:
    HttpRequestHeaders(const std::string_view path_to_headers_file);
    ~HttpRequestHeaders();

    std::string get_header(const std::string_view header) const;
    void add_header(const std::string_view header, const std::string_view value);
    bool update_header(const std::string_view header, const std::string_view value);
    std::string remove_header(const std::string_view header);

    std::string operator [](int index) const;
    size_t size() const;

    int find_header(const std::string_view header) const;

private:
    std::string make_header(const std::string_view header, const std::string_view value) const;

    // List of headers
    std::vector<std::string> headers;
};

} // namespace sapv

#endif // SAPV_HTTP_REQUEST_HEADERS_H
