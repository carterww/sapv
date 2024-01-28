#include "portfolio/data.h"
#include <iostream>
#include "stock/data.h"

namespace sapv {
namespace portfolio {

std::optional<float> PortfolioData::total_current_value() {
  float total = 0;
  for (const stock::StockData& stock : stocks) {
    if (stock.share_count.has_value()) {
      total += stock.current_price * stock.share_count.value();
    }
  }
  return total == 0 ? std::nullopt : std::optional(total);
}

std::optional<float> PortfolioData::total_cost_basis() {
  float total = 0;
  for (const stock::StockData& stock : stocks) {
    if (stock.cost_basis_total.has_value()) {
      total += stock.cost_basis_total.value();
    }
  }
  return total == 0 ? std::nullopt : std::optional(total);
}

std::optional<float> PortfolioData::total_gain_loss_absolute() {
  float total = 0;
  for (const stock::StockData& stock : stocks) {
    if (stock.total_gain_loss_absolute.has_value()) {
      total += stock.total_gain_loss_absolute.value();
    }
  }
  return total == 0 ? std::nullopt : std::optional(total);
}

std::optional<float> PortfolioData::total_gain_loss_percent() {
  std::optional<float> total_cost_basis = this->total_cost_basis();
  std::optional<float> total_current_value = this->total_current_value();
  if (!total_cost_basis.has_value() || !total_current_value.has_value()) {
    return std::nullopt;
  }
  return (total_current_value.value() - total_cost_basis.value()) /
         total_cost_basis.value();
}

std::optional<float> PortfolioData::daily_gain_loss_absolute() {
  float total = 0;
  for (const stock::StockData& stock : stocks) {
    if (stock.daily_gain_loss_absolute.has_value()) {
      total += stock.daily_gain_loss_absolute.value();
    }
  }
  return total == 0 ? std::nullopt : std::optional(total);
}

std::optional<float> PortfolioData::daily_gain_loss_percent() {
  std::optional<float> daily_gain_loss_abs = this->daily_gain_loss_absolute();
  std::optional<float> total_current_value = this->total_current_value();
  if (!daily_gain_loss_abs.has_value() || !total_current_value.has_value()) {
    return std::nullopt;
  }
  return daily_gain_loss_abs.value() / total_current_value.value();
}

void PortfolioData::add_stock(const sapv::stock::StockData& stock) {
  stock::StockData stock_copy = stock;
  this->add_stock(std::move(stock_copy));
}

void PortfolioData::add_stock(sapv::stock::StockData&& stock) {
  if (stock.ticker.empty()) {
    return;
  }

  int index = this->find_stock(stock.ticker);
  if (index == -1) {
    stocks.push_back(stock);
  } else {
    stocks[index] += std::move(stock);
  }
}

int PortfolioData::find_stock(const std::string& ticker) {
  for (int i = 0; i < stocks.size(); ++i) {
    if (stocks[i].ticker == ticker)
      return i;
  }
  return -1;
}

const std::vector<sapv::stock::StockData>& PortfolioData::get_stocks() const {
  return stocks;
}

}  // namespace portfolio
}  // namespace sapv
