#ifndef SAPV_UTILS_STRING_H
#define SAPV_UTILS_STRING_H

#include <memory>
#include <string>
#include <vector>

namespace sapv {
namespace utils {
namespace string {

std::unique_ptr<std::vector<std::string>> split(std::string str,
                                                const std::string_view delim);

void remove(std::string& str, const std::string_view chars);

void to_upper(std::string& str);

}  // namespace string
}  // namespace utils
}  // namespace sapv

#endif  // SAPV_UTILS_STRING_H
