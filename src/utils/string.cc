#include "utils/string.h"

#include <memory>
#include <string>

namespace sapv {
namespace utils {
namespace string {

std::unique_ptr<std::vector<std::string>> split(std::string str,
                                                const std::string_view delim) {
  std::unique_ptr<std::vector<std::string>> result =
      std::make_unique<std::vector<std::string>>();
  while (true) {
    size_t index = str.find(delim);
    if (index == std::string::npos) {
      result->push_back(std::string(str));
      break;
    }
    result->push_back(std::string(str.substr(0, index)));
    str = str.substr(index + delim.size());
  }
  return result;
}

}  // namespace string
}  // namespace utils
}  // namespace sapv
