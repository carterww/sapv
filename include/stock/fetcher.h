#ifndef SAPV_STOCK_FETCHER_H_
#define SAPV_STOCK_FETCHER_H_

#include <memory>
#include <string>

#include "http/request-headers.h"
#include "http/curl-http-client.h"
#include "stock/data.h"

namespace sapv {
namespace stock {
namespace fetcher {

class StockFetcher {
 public:
  StockFetcher() = default;
  virtual ~StockFetcher() = default;

  virtual std::unique_ptr<FetchedStockData> fetch(
      const std::string& ticker) = 0;
};

class YahooFinanceStockFetcher : public StockFetcher {
 public:
  YahooFinanceStockFetcher(const std::string_view path_to_headers);
  ~YahooFinanceStockFetcher();

  std::unique_ptr<FetchedStockData> fetch(
      const std::string& ticker) override;

 protected:
  HttpRequestHeaders headers;
  curl::CurlHttpClient client;

  std::unique_ptr<FetchedStockData> parse_response(
      std::unique_ptr<std::string> response);
};

}  // namespace fetcher
}  // namespace stock
}  // namespace sapv

#endif  // SAPV_STOCK_FETCHER_H_
