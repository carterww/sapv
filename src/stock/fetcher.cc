#include "stock/fetcher.h"

#include <iostream>
#include <memory>
#include <string>

#include "http/curl-http-client.h"
#include "http/request-headers.h"
#include "stock/data.h"
#include "utils/html.h"
#include "utils/string.h"

static const std::string_view yahoo_finance_base = "https://finance.yahoo.com/";

namespace sapv {
namespace stock {
namespace fetcher {

YahooFinanceStockFetcher::YahooFinanceStockFetcher(
    const std::string_view path_to_headers)
    : headers(path_to_headers), client(){};

YahooFinanceStockFetcher::~YahooFinanceStockFetcher(){};

std::unique_ptr<FetchedStockData> YahooFinanceStockFetcher::fetch(
    const std::string& ticker) {
  std::string upper_ticker = ticker;
  std::string url = yahoo_finance_base.data();
  url += "quote/" + upper_ticker;

  std::cout << "Fetching " << url << std::endl;

  std::unique_ptr<std::string> response = client.get(url, headers);
  return parse_response(std::move(response));
}

// This needs to be broken up into smaller functions
// but need to make sure it works first
std::unique_ptr<FetchedStockData> YahooFinanceStockFetcher::parse_response(
    std::unique_ptr<std::string> response) {
  std::unique_ptr<FetchedStockData> result =
      std::make_unique<FetchedStockData>();

  char* html = response->data();
  size_t html_size = response->size();

  utils::html::HtmlDocumentParser parser;
  parser.parse(html, html_size, 12);

  // Use these to narrow search for data fields
  parser.element_by_attribute("id", "quote-header-info", "quote-header-info");
  parser.element_by_attribute("id", "quote-summary", "quote-summary");

  // Get data elements
  parser.element_by_attribute("data-field", "regularMarketPrice",
                              "current-price", "quote-header-info");
  parser.element_by_attribute("data-field", "regularMarketChange",
                              "daily-change-absolute", "quote-header-info");
  parser.element_by_attribute("data-test", "OPEN-value", "open-price",
                              "quote-summary");
  parser.element_by_attribute("data-test", "PREV_CLOSE-value",
                              "previous-close-price", "quote-summary");

  std::optional<std::string> tmp_str;

  tmp_str = parser.element_get_attribute("current-price", "value");
  result->current_price = std::stof(tmp_str.value_or("0.0"));

  tmp_str = parser.element_get_attribute("daily-change-absolute", "value");
  result->daily_change_absolute = std::stof(tmp_str.value_or("0.0"));

  tmp_str = parser.element_get_inner_html("open-price");
  result->open_price = std::stof(tmp_str.value_or("0.0"));

  tmp_str = parser.element_get_inner_html("previous-close-price");
  result->previous_close_price = std::stof(tmp_str.value_or("0.0"));

  return result;
}

}  // namespace fetcher
}  // namespace stock
}  // namespace sapv
