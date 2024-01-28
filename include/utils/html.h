#ifndef SAPV_UTILS_HTML_H_
#define SAPV_UTILS_HTML_H_

#include <optional>
#include <string>
#include <unordered_map>

#include <lexbor/dom/collection.h>
#include <lexbor/html/interface.h>

namespace sapv {
namespace utils {
namespace html {

class HtmlDocumentParser {
 public:
  HtmlDocumentParser() = default;
  ~HtmlDocumentParser();

  int parse(const char* html, const size_t html_size,
            size_t collection_size = 128);

  int element_by_attribute(const std::string& attr_name,
                           const std::string& attr_value,
                           std::string new_element_name,
                           std::string wrapper_element_name = "");

  std::optional<std::string> element_get_attribute(
      const std::string& element_name, const std::string& attr_name);

  std::optional<std::string> element_get_inner_html(
      const std::string& element_name);

 private:
  lxb_html_document_t* doc;
  lxb_dom_collection_t* collection;

  // Key: element name (user defined) -> Value: index in collection
  std::unordered_map<std::string, int> saved_elements;

  const lxb_char_t* tmp_str_data;
  size_t tmp_str_len;

  lxb_dom_element_t* element_get(const std::string& element_name) const;
  bool is_initialzed() const;
};

}  // namespace html
}  // namespace utils
}  // namespace sapv

#endif  // SAPV_UTILS_HTML_H_
