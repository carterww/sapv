#ifndef SAPV_PORTFOLIO_READ_H
#define SAPV_PORTFOLIO_READ_H

#include <memory>
#include <string>

#include "portfolio/data.h"

namespace sapv {
namespace portfolio {

enum Broker {
  FIDELITY
};

std::unique_ptr<PortfolioData> read(const std::string_view path, Broker broker);

}  // namespace portfolio
}  // namespace sapv

#endif  //SAPV_PORTFOLIO_READ_H
