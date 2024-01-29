#include "config/globalconf.h"
#include "workers/update-stocks.h"

#include <unistd.h>

#include <iostream>

#include "portfolio/manager.h"

namespace sapv {
namespace worker {

static const config::GlobalConfig* conf;

// This thread should run indefinitely, updating the stocks in the portfolio
// If it returns, something went wrong.
void* fetch_thread_worker(void* portfolio_manager) {

  portfolio::PortfolioManager* manager =
      (portfolio::PortfolioManager*)portfolio_manager;

  conf = config::GlobalConfig::get_instance();

  while (true) {
    auto stock = manager->get_next_stock_to_update();
    if (!stock) {
      std::cerr << "No stocks in portfolio." << std::endl;
      return (void*)1;
    }
    manager->update_next_stock();
    auto data = stock.value().get();

    std::cout << "Current Price: " << data.current_price << std::endl;
    std::cout << "Daily Gain/Loss: " << data.daily_change_absolute << std::endl;
    std::cout << "Open Price: " << data.open_price << std::endl;
    std::cout << "Previous close: " << data.previous_close_price << std::endl
              << std::endl;

    // Should sleep for global conf time
    std::cout << "Sleeping for " << conf->time_between_requests()
              << " seconds..." << std::endl;
    sleep(conf->time_between_requests());
  }

  return nullptr;
}

}  // namespace worker
}  // namespace sapv
