#include "portfolio/read.h"
#include "portfolio/fidelity.h"

#include <iostream>
#include <memory>
#include <string>

namespace sapv {
namespace portfolio {

std::unique_ptr<PortfolioData> read(const std::string_view path,
                                    Broker broker) {
  switch (broker) {
    case FIDELITY:
      return fidelity::read(path);
    default:
      std::cerr << "Broker not supported" << std::endl;
      return std::unique_ptr<PortfolioData>(nullptr);
  }
}

}  // namespace portfolio
}  // namespace sapv
