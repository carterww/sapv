#include "portfolio/manager.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <string>

#include "portfolio/data.h"
#include "portfolio/read.h"
#include "stock/data.h"
#include "stock/fetcher.h"

namespace sapv {
namespace portfolio {

namespace fetcher = sapv::stock::fetcher;

PortfolioManager::PortfolioManager() {
  stock_fetcher = nullptr;
  portfolio_file = "";
}

PortfolioManager::~PortfolioManager() {}

void PortfolioManager::read_portfolio(const std::string portfolio_file,
                                      Broker broker) {
  portfolio_data = read(portfolio_file, broker);
  this->portfolio_file = portfolio_file;
  current_stock_index = portfolio_data->stocks.size() ? 0 : -1;
}

void PortfolioManager::write_portfolio(const std::string portfolio_file) {
  std::cerr << "Not implemented yet. May never implement honestly."
            << std::endl;
}

void PortfolioManager::set_stock_fetcher(
    std::unique_ptr<fetcher::StockFetcher> stock_fetcher) {
  if (!stock_fetcher) {
    std::cerr << "Stock fetcher is nullptr. Not setting." << std::endl;
    return;
  }
  // Give ownership
  this->stock_fetcher = std::move(stock_fetcher);
}

const PortfolioData& PortfolioManager::get_portfolio() const {
  return *portfolio_data;
}

std::optional<std::reference_wrapper<const stock::StockData>>
PortfolioManager::get_next_stock_to_update() const {
  if (current_stock_index == -1) {
    std::cerr << "No stocks in portfolio." << std::endl;
    return std::nullopt;
  }
  return std::cref(portfolio_data->stocks[current_stock_index]);
}

std::optional<std::reference_wrapper<const stock::StockData>>
PortfolioManager::get_stock(const std::string ticker) const {
  int index = portfolio_data->find_stock(ticker);
  if (index == -1) {
    std::cerr << "Stock not found in portfolio." << std::endl;
    return std::nullopt;
  }
  return std::cref(portfolio_data->stocks[index]);
}

void PortfolioManager::update_next_stock() {
  if (current_stock_index == -1) {
    std::cerr << "No stocks in portfolio." << std::endl;
    return;
  }
  update_stock(portfolio_data->stocks[current_stock_index].ticker);
  ++current_stock_index;
}

void PortfolioManager::update_stock(const std::string ticker) {
  if (stock_fetcher == nullptr) {
    std::cerr << "Stock fetcher is nullptr. Not updating." << std::endl;
    return;
  }
  int index = portfolio_data->find_stock(ticker);
  if (index == -1) {
    std::cerr << "Stock not found in portfolio." << std::endl;
    return;
  }
  std::unique_ptr<stock::FetchedStockData> data = stock_fetcher->fetch(ticker);
  if (data == nullptr) {
    std::cerr << "Stock fetcher returned nullptr. Not updating." << std::endl;
    return;
  }
  portfolio_data->stocks[index].open_price = data->open_price;
  portfolio_data->stocks[index].current_price = data->current_price;
  portfolio_data->stocks[index].previous_close_price =
      data->previous_close_price;
  portfolio_data->stocks[index].daily_change_absolute =
      data->daily_change_absolute;
}

}  // namespace portfolio
}  // namespace sapv
