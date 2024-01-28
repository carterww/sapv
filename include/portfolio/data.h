#ifndef SAPV_PORTFOLIO_DATA_H
#define SAPV_PORTFOLIO_DATA_H

#include <memory>
#include <optional>
#include <vector>

#include "stock/data.h"

namespace sapv {
namespace portfolio {

// Everything is calculated from list of stocks
class PortfolioData {
 public:
  PortfolioData() = default;
  PortfolioData(const PortfolioData& other) = default; // TODO: make this later
  PortfolioData(PortfolioData&& other) = default; // TODO: make this later

  std::optional<float> total_current_value();
  std::optional<float> total_cost_basis();
  std::optional<float> total_gain_loss_absolute();
  std::optional<float> total_gain_loss_percent();
  std::optional<float> daily_gain_loss_absolute();
  std::optional<float> daily_gain_loss_percent();

  void add_stock(const sapv::stock::StockData& stock); // Copy
  void add_stock(sapv::stock::StockData&& stock);

  int find_stock(const std::string& ticker);

 private:
  std::vector<sapv::stock::StockData> stocks;
};

}  // namespace portfolio
}  // namespace sapv

#endif  //SAPV_PORTFOLIO_DATA_H
