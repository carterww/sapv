#include "portfolio/fidelity.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "portfolio/data.h"
#include "stock/data.h"
#include "utils/string.h"

namespace {

namespace string = sapv::utils::string;
namespace fidelity = sapv::portfolio::fidelity;

void read_record(fidelity::FidelityDataRecord& record, std::string& line) {
  std::unique_ptr<std::vector<std::string>> fields = string::split(line, ",");
  // SPAXX** is cash, we don't care about that
  if (fields->size() != 16 || fields->at(2) == "SPAXX**") {
    return;
  }
  // Ok this is terrible, I'll refactor this later
  record.account_number = fields->at(0);
  record.account_name = fields->at(1);
  record.symbol = fields->at(2);
  record.description = fields->at(3);

  // Remove $ and % from the float fields
  for (int i = 4; i < 15; ++i) {
    string::remove(fields->at(i), "$%");
  }

  record.quantity = std::stof(fields->at(4));
  record.last_price = std::stof(fields->at(5));
  record.last_price_change = std::stof(fields->at(6));
  record.current_value = std::stof(fields->at(7));
  record.todays_gain_loss_dollar = std::stof(fields->at(8));
  record.todays_gain_loss_percent = std::stof(fields->at(9));
  record.total_gain_loss_dollar = std::stof(fields->at(10));
  record.total_gain_loss_percent = std::stof(fields->at(11));
  record.percent_of_account = std::stof(fields->at(12));
  record.cost_basis_total = std::stof(fields->at(13));
  record.average_cost_basis = std::stof(fields->at(14));

  record.type = fields->at(15);
}

void move_records_to_portfolio(
    std::vector<fidelity::FidelityDataRecord>& records,
    sapv::portfolio::PortfolioData* portfolio) {
  for (fidelity::FidelityDataRecord& record : records) {
    sapv::stock::StockData stock;
    stock.ticker = std::move(record.symbol);
    stock.name = std::move(record.description);

    // This stuff will be updated on fetch
    stock.current_price = record.last_price;
    stock.total_gain_loss_absolute = record.total_gain_loss_dollar;

    stock.share_count = record.quantity;
    stock.cost_basis_total = record.cost_basis_total;

    // Stuff we don't have, but will be fetched
    stock.daily_change_absolute = 0.0f;
    stock.daily_gain_loss_absolute = 0.0f;
    stock.open_price = 0.0f;
    stock.previous_close_price = 0.0f;
    portfolio->add_stock(stock);
  }
}

}  // namespace

namespace sapv {
namespace portfolio {
namespace fidelity {

std::unique_ptr<PortfolioData> read(const std::string_view path) {
  std::ifstream file(path.data());
  if (!file.is_open()) {
    std::cerr << "Could not open file: " << path << std::endl;
    throw std::runtime_error("Could not open file");
  }
  std::string line;
  std::getline(file, line);  // Skip header

  std::vector<FidelityDataRecord> records;
  PortfolioData* portfolio = new PortfolioData();
  while (file.good()) {
    std::getline(file, line);

    FidelityDataRecord record;
    read_record(record, line);
    records.push_back(record);
  }
  move_records_to_portfolio(records, portfolio);
  return std::unique_ptr<PortfolioData>(portfolio);
}

}  // namespace fidelity
}  // namespace portfolio
}  // namespace sapv
