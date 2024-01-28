#ifndef SAPV_PORTFOLIO_FIDELITY_H
#define SAPV_PORTFOLIO_FIDELITY_H

#include "portfolio/read.h"

#include <memory>
#include <string>

#include "portfolio/data.h"

namespace sapv {
namespace portfolio {
namespace fidelity {

struct FidelityDataRecord {
  // In this order
  std::string account_number;
  std::string account_name;
  std::string symbol;
  std::string description;
  float quantity;
  float last_price;
  float last_price_change;
  float current_value;
  float todays_gain_loss_dollar;
  float todays_gain_loss_percent;
  float total_gain_loss_dollar;
  float total_gain_loss_percent;
  float percent_of_account;
  float cost_basis_total;
  float average_cost_basis;
  std::string type;
};

std::unique_ptr<PortfolioData> read(const std::string_view path);

}  // namespace fidelity
}  // namespace portfolio
}  // namespace sapv

#endif  //SAPV_PORTFOLIO_FIDELITY_H
