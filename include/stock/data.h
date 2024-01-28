#ifndef SAPV_STOCK_DATA_H
#define SAPV_STOCK_DATA_H

#include <cstdint>
#include <optional>
#include <string>

namespace sapv {
namespace stock {

// Percent change/per share is calculated instead of being stored
// TODO: Create a builder for this class
class StockData {
 public:
  std::string ticker;
  std::string name;
  float current_price;
  float daily_change_absolute;
  float daily_change_percent();
  float open_price;
  float previous_close_price;

  // Optional because depends on user supplied data
  std::optional<float> share_count;
  std::optional<float> cost_basis_total;
  std::optional<float> cost_basis_per_share();
  std::optional<float> total_gain_loss_absolute;
  std::optional<float> total_gain_loss_percent();
  std::optional<float> daily_gain_loss_absolute;
  std::optional<float> daily_gain_loss_percent();
  std::optional<float> yearly_gain_loss_absolute;
  std::optional<float> yearly_gain_loss_percent();
};

}  // namespace stock
}  // namespace sapv

#endif  //SAPV_STOCK_DATA_H
