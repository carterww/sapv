#ifndef SAPV_CONFIG_GLOBALCONF_H
#define SAPV_CONFIG_GLOBALCONF_H

#include <cstdint>
#include <string>
#include <vector>

namespace sapv {
namespace config {

class GlobalConfig {
 public:
  GlobalConfig(const std::string_view path);
  ~GlobalConfig();

  int32_t time_between_requests() const;
  const std::vector<std::string>& display_mode_directories() const;
  const std::string& display_mode() const;
  bool start_web_gui() const;

 private:
  // Properties of the config.json file
  int32_t time_between_requests_;
  std::vector<std::string> display_mode_directories_;
  std::string display_mode_;
  bool start_web_gui_;
};

}  // namespace config
}  // namespace sapv

#endif  //SAPV_CONFIG_GLOBALCONF_H
