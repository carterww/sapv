#include "stock/data.h"

namespace sapv {
namespace stock {

// NEED TO FIX == FLOAT COMPARISON

StockData::StockData(const StockData& other) {
  ticker = other.ticker;
  name = other.name;
  current_price = other.current_price;
  daily_change_absolute = other.daily_change_absolute;
  open_price = other.open_price;
  previous_close_price = other.previous_close_price;
  share_count = other.share_count;
  cost_basis_total = other.cost_basis_total;
  total_gain_loss_absolute = other.total_gain_loss_absolute;
  daily_gain_loss_absolute = other.daily_gain_loss_absolute;
}

float StockData::daily_change_percent() {
  if (previous_close_price == 0.0f) {
    return 0.0f;
  }
  return daily_change_absolute / previous_close_price;
}

std::optional<float> StockData::cost_basis_per_share() {
  if (!share_count.has_value() || cost_basis_total.value_or(0.0f) == 0.0f) {
    return std::nullopt;
  }
  return cost_basis_total.value() / share_count.value();
}

std::optional<float> StockData::total_gain_loss_percent() {
  if (!total_gain_loss_absolute.has_value() ||
      cost_basis_total.value_or(0.0f) == 0.0f) {
    return std::nullopt;
  }
  return total_gain_loss_absolute.value() / cost_basis_total.value();
}

std::optional<float> StockData::daily_gain_loss_percent() {
  if (!daily_gain_loss_absolute.has_value() ||
      share_count.value_or(0.0f) == 0.0f || previous_close_price == 0.0f) {
    return std::nullopt;
  }
  return daily_gain_loss_absolute.value() /
         (share_count.value() * previous_close_price);
}

StockData& StockData::operator+=(const StockData&& other) {
  share_count = share_count.value_or(0.0f) + other.share_count.value_or(0.0f);
  cost_basis_total =
      cost_basis_total.value_or(0.0f) + other.cost_basis_total.value_or(0.0f);
  total_gain_loss_absolute = total_gain_loss_absolute.value_or(0.0f) +
                             other.total_gain_loss_absolute.value_or(0.0f);
  daily_gain_loss_absolute = daily_gain_loss_absolute.value_or(0.0f) +
                             other.daily_gain_loss_absolute.value_or(0.0f);
  return *this;
}

}  // namespace stock
}  // namespace sapv
