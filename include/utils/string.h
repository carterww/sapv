#ifndef SAPV_UTILS_STRING_H
#define SAPV_UTILS_STRING_H

#include <memory>
#include <vector>
#include <string>

namespace sapv {
namespace utils {
namespace string {


std::unique_ptr<std::vector<std::string>> split(const std::string_view str, const std::string_view delimiter);


} // namespace string
} // namespace utils
} // namespace sapv

#endif // SAPV_UTILS_STRING_H
