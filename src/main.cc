#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "portfolio/read.h"
#include "stock/fetcher.h"

int main(int argc, char** argv) {
  sapv::stock::fetcher::YahooFinanceStockFetcher fetcher("../yahoo-finance-req.json");

  std::unique_ptr<sapv::portfolio::PortfolioData> portfolio =
      sapv::portfolio::read(
          "../portfolios/fidelity.csv",
          sapv::portfolio::FIDELITY);

  for (const sapv::stock::StockData& stock : portfolio->get_stocks()) {
    std::unique_ptr<sapv::stock::FetchedStockData> data =
      fetcher.fetch(stock.ticker);

    std::cout << "Current Price: " << data->current_price << std::endl;
    std::cout << "Daily Gain/Loss: " << data->daily_change_absolute << std::endl;
    std::cout << "Open Price: " << data->open_price << std::endl;
    std::cout << "Previous close: " << data->previous_close_price << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(15));
  }

  return 0;
}
