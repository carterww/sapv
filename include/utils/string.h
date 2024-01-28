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

}  // namespace string
}  // namespace utils
}  // namespace sapv

#endif  // SAPV_UTILS_STRING_H
