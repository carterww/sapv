#ifndef SAPV_PORTFOLIO_MANAGER_H
#define SAPV_PORTFOLIO_MANAGER_H

#include <memory>
#include <string>

#include "portfolio/data.h"
#include "portfolio/read.h"
#include "stock/data.h"
#include "stock/fetcher.h"

namespace sapv {
namespace portfolio {

class PortfolioManager {
 public:
  PortfolioManager();
  virtual ~PortfolioManager();

  virtual void read_portfolio(const std::string portfolio_file, Broker broker);
  virtual void write_portfolio(const std::string portfolio_file);

  virtual void set_stock_fetcher(
      std::unique_ptr<stock::fetcher::StockFetcher> stock_fetcher);

  virtual const PortfolioData& get_portfolio() const;

  // Gosh these are ugly. Returning an optional const reference
  virtual std::optional<std::reference_wrapper<const stock::StockData>>
  get_next_stock_to_update() const;
  virtual std::optional<std::reference_wrapper<const stock::StockData>>
  get_stock(const std::string ticker) const;

  virtual void update_next_stock();
  virtual void update_stock(const std::string ticker);

 protected:
  std::unique_ptr<stock::fetcher::StockFetcher> stock_fetcher;
  std::unique_ptr<PortfolioData> portfolio_data;

  std::string portfolio_file;
  int current_stock_index;
};

}  // namespace portfolio
}  // namespace sapv

#endif  //SAPV_PORTFOLIO_MANAGER_H
