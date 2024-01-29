#include <pthread.h>

#include <iostream>
#include <string>

#include "portfolio/manager.h"
#include "stock/fetcher.h"
#include "workers/update-stocks.h"

// BIG TODO POSSIBLY: RACE CONDITION FOR DISPLAY READING INFO AND
// FETCH THREAD UPDATING INFO. WILL THIS EVER MATTER? PROBABLY NOT.

int main(int argc, char** argv) {
  // Create our stock fetcher (YahooFinance)
  sapv::stock::fetcher::YahooFinanceStockFetcher fetcher(
      "../yahoo-finance-req.json");

  // Create our portfolio manager
  sapv::portfolio::PortfolioManager manager;
  manager.read_portfolio("../portfolios/fidelity.csv",
                         sapv::portfolio::FIDELITY);
  // Add our stock fetcher to the portfolio manager
  manager.set_stock_fetcher(
      std::make_unique<sapv::stock::fetcher::YahooFinanceStockFetcher>(
          "../yahoo-finance-req.json"));

  pthread_t fetch_thread, display_thread;

  // Start the worker thread
  int rc = pthread_create(&fetch_thread, NULL,
                          sapv::worker::fetch_thread_worker, (void*)&manager);
  if (rc) {
    std::cerr << "Error: unable to create thread, " << rc << std::endl;
    return 1;
  }

  void* status;
  // Indefinite
  pthread_join(fetch_thread, &status);

  if (status) {
    std::cerr << "Error: thread returned, " << status << std::endl;
    return 1;
  }

  return 0;
}
