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

void remove(std::string& str, const std::string_view chars) {
  for (int i = 0; i < str.size(); ++i) {
    for (int j = 0; j < chars.size(); ++j) {
      if (str[i] == chars[j]) {
        str.erase(i, 1);
      }
    }
  }
}

void to_upper(std::string& str) {
  for (int i = 0; i < str.size(); ++i) {
    str[i] = toupper(str[i]);
  }
}

}  // namespace string
}  // namespace utils
}  // namespace sapv
