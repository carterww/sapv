#include "portfolio/fidelity.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "portfolio/data.h"
#include "utils/string.h"

namespace {

namespace string = sapv::utils::string;
namespace fidelity = sapv::portfolio::fidelity;

std::unique_ptr<fidelity::FidelityDataRecord> read_record(std::string& line) {

  std::unique_ptr<std::vector<std::string>> fields = string::split(line, ",");
  if (fields->size() != 16) {
    return std::unique_ptr<fidelity::FidelityDataRecord>(nullptr);
  }
  std::unique_ptr<fidelity::FidelityDataRecord> record =
      std::make_unique<fidelity::FidelityDataRecord>();
  // Ok this is terrible, I'll refactor this later
  record->account_number = fields->at(0);
  record->account_name = fields->at(1);
  record->symbol = fields->at(2);
  if (record->symbol == "SPAXX**") {
    return std::unique_ptr<fidelity::FidelityDataRecord>(nullptr);
  }
  record->description = fields->at(3);

  record->quantity = std::stof(fields->at(4));
  record->last_price = std::stof(fields->at(5).substr(1));
  record->last_price_change = std::stof(fields->at(6).replace(1, 1, ""));
  record->current_value = std::stof(fields->at(7).substr(1));
  record->todays_gain_loss_dollar = std::stof(fields->at(8).replace(1, 1, ""));
  record->todays_gain_loss_percent =
      std::stof(fields->at(9).substr(0, fields->at(9).size() - 1));
  record->total_gain_loss_dollar = std::stof(fields->at(10).replace(1, 1, ""));
  record->total_gain_loss_percent =
      std::stof(fields->at(11).substr(0, fields->at(11).size() - 1));
  record->percent_of_account =
      std::stof(fields->at(12).substr(0, fields->at(12).size() - 1));
  record->cost_basis_total = std::stof(fields->at(13).substr(1));
  record->average_cost_basis = std::stof(fields->at(14).substr(1));

  record->type = fields->at(15);

  return record;
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

  std::unique_ptr<PortfolioData> portfolio = std::make_unique<PortfolioData>();
  while (file.good()) {
    std::getline(file, line);
    std::unique_ptr<FidelityDataRecord> record = read_record(line);

    if (record == nullptr) {
      continue;
    }
  }
  // TODO: Convert records to stocks
  return portfolio;
}

}  // namespace fidelity
}  // namespace portfolio
}  // namespace sapv
