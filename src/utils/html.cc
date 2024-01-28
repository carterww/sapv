#include "utils/html.h"

#include <iostream>
#include <optional>

#include <lexbor/dom/collection.h>
#include <lexbor/dom/interfaces/element.h>
#include <lexbor/html/interface.h>
#include <lexbor/html/interfaces/document.h>
#include <lexbor/html/serialize.h>

namespace sapv {
namespace utils {
namespace html {
/*
class HtmlDocumentParser {
 public:
  HtmlDocumentParser() = default;
  ~HtmlDocumentParser();

  int parse(const std::string& html, size_t collection_size = 128);

  int element_by_attribute(const std::string& attr_name,
                           const std::string& attr_value,
                           std::string new_element_name,
                           std::string wrapper_element_name = "");

  std::string element_get_attribute(const std::string& element_name,
                                    const std::string& attr_name);

  std::string element_get_inner_html(const std::string& element_name);
 private:
  lxb_html_document_t* doc;
  lxb_dom_collection_t* collection;

  // Key: element name (user defined) -> Value: index in collection
  std::unordered_map<std::string, int> saved_elements;

  const lxb_char_t* tmp_str_data;
  size_t tmp_str_len;
};
 */

HtmlDocumentParser::~HtmlDocumentParser() {
  if (doc != nullptr) {
    lxb_html_document_destroy(doc);
  }
  if (collection != nullptr) {
    lxb_dom_collection_destroy(collection, true);
  }
}

int HtmlDocumentParser::parse(const char* html, const size_t html_size,
                              size_t collection_size) {
  lxb_status_t status;

  doc = lxb_html_document_create();
  if (doc == nullptr) {
    std::cerr << "Failed to create HTML document" << std::endl;
    return -1;
  }

  lxb_char_t* html_cstr = (lxb_char_t*)html;
  status = lxb_html_document_parse(doc, html_cstr, html_size);
  if (status != LXB_STATUS_OK) {
    std::cerr << "Failed to parse HTML document" << std::endl;
    return -1;
  }

  collection = lxb_dom_collection_make(&doc->dom_document, collection_size);
  if (collection == nullptr) {
    std::cerr << "Failed to create DOM collection" << std::endl;
    return -1;
  }

  return 0;
}

int HtmlDocumentParser::element_by_attribute(const std::string& attr_name,
                                             const std::string& attr_value,
                                             std::string new_element_name,
                                             std::string wrapper_element_name) {
  if (doc == nullptr || collection == nullptr) {
    std::cerr
        << "HtmlDocumentParser::parse must be called prior to this function"
        << std::endl;
    return -1;
  }
  lxb_status_t status;
  lxb_dom_element_t* element = nullptr;

  // If wrapper, use saved_elements to get the element
  if (!wrapper_element_name.empty() &&
      saved_elements.find(wrapper_element_name) != saved_elements.end()) {
    int index = saved_elements.at(wrapper_element_name);
    element = lxb_dom_collection_element(collection, index);
  }
  // Use body as default
  if (element == nullptr) {
    element = lxb_dom_interface_element(doc->body);
  }
  lxb_char_t* attr_name_cstr = (lxb_char_t*)attr_name.c_str();
  lxb_char_t* attr_value_cstr = (lxb_char_t*)attr_value.c_str();
  size_t prev_collection_length = lxb_dom_collection_length(collection);

  status = lxb_dom_elements_by_attr(element, collection, attr_name_cstr,
                                          attr_name.size(), attr_value_cstr,
                                          attr_value.size(), false);
  if (status != LXB_STATUS_OK) {
    std::cerr << "Failed to get elements by attribute" << std::endl;
    return -1;
  }
  if (lxb_dom_collection_length(collection) == prev_collection_length) {
    std::cerr << "No elements found" << std::endl;
    return -1;
  }

  // Add new element to saved_elements
  if (!new_element_name.empty()) {
    int index = lxb_dom_collection_length(collection) - 1;
    saved_elements[new_element_name] = index;
  }
  return 0;
}

std::optional<std::string> HtmlDocumentParser::element_get_attribute(
    const std::string& element_name, const std::string& attr_name) {
  lxb_dom_element_t* element = element_get(element_name);
  if (element == nullptr) {
    std::cerr << "Element not found" << std::endl;
    return std::nullopt;
  }
  tmp_str_data = lxb_dom_element_get_attribute(
      element, (lxb_char_t*)attr_name.c_str(), attr_name.size(), &tmp_str_len);
  if (tmp_str_data == nullptr || tmp_str_len == 0) {
    std::cerr << "Attribute not found" << std::endl;
    return std::nullopt;
  }

  return std::string((char*)tmp_str_data, tmp_str_len);
}

std::optional<std::string> HtmlDocumentParser::element_get_inner_html(
    const std::string& element_name) {
  lxb_dom_element_t* element = element_get(element_name);
  if (element == nullptr) {
    std::cerr << "Element not found" << std::endl;
    return std::nullopt;
  }
  lxb_dom_node_t* node = lxb_dom_interface_node(element);
  if (node == nullptr) {
    std::cerr << "Node not found" << std::endl;
    return std::nullopt;
  }
  lexbor_str_t str = {0};
  lxb_html_serialize_deep_str(node, &str);
  if (str.data == nullptr || str.length == 0) {
    std::cerr << "Failed to serialize node" << std::endl;
    return std::nullopt;
  }
  return std::string((char*)str.data, str.length);
}

lxb_dom_element_t* HtmlDocumentParser::element_get(
    const std::string& element_name) const {
  if (!is_initialzed()) {
    return nullptr;
  }
  if (saved_elements.find(element_name) == saved_elements.end()) {
    std::cerr << "Element not found" << std::endl;
    return nullptr;
  }
  int index = saved_elements.at(element_name);
  lxb_dom_element_t* element = lxb_dom_collection_element(collection, index);
  return element;
}

bool HtmlDocumentParser::is_initialzed() const {
  bool is_initialized = doc != nullptr && collection != nullptr;
  if (!is_initialized) {
    std::cerr
        << "HtmlDocumentParser::parse must be called prior to this function"
        << std::endl;
  }
  return is_initialized;
}

}  // namespace html
}  // namespace utils
}  // namespace sapv
