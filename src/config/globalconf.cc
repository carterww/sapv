#include "config/globalconf.h"

#include <cstdint>
#include <fstream>

#include "nlohmann/json.hpp"

namespace sapv {
namespace config {

GlobalConfig::GlobalConfig(const std::string_view path) {
  using json = nlohmann::json;

  std::ifstream file(path.data());
  if (!file.is_open()) {
    time_between_requests_ = 60;
    display_mode_directories_ = {"./"};
    display_mode_ = "terminal";
    start_web_gui_ = false;
    return;
  }
  json data = json::parse(file);
  
  // TODO: Add error handling?
  time_between_requests_ = data.at("time_between_requests").get<int32_t>();
  display_mode_directories_ = data.at("display_mode_directories").get<std::vector<std::string>>();
  display_mode_ = data.at("display_mode").get<std::string>();
  start_web_gui_ = data.at("start_web_gui").get<bool>();
}

GlobalConfig::~GlobalConfig() {}

int32_t GlobalConfig::time_between_requests() const {
  return time_between_requests_;
}

const std::vector<std::string>& GlobalConfig::display_mode_directories() const {
  return display_mode_directories_;
}

const std::string& GlobalConfig::display_mode() const {
  return display_mode_;
}

bool GlobalConfig::start_web_gui() const {
  return start_web_gui_;
}

const GlobalConfig* GlobalConfig::get_instance(const std::string_view path) {
  // This will be allocated the entire time the program is running
  // after first call
  static GlobalConfig* instance = nullptr;
  if (instance == nullptr) {
    instance = new GlobalConfig(path);
  }
  return instance;
}

}  // namespace config
}  // namespace sapv
