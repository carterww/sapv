#include <iostream>
#include <string>

#include <nlohmann/json.hpp>

#include "http/curl-http-client.h"
#include "http/request-headers.h"
#include "portfolio//read.h"
#include "utils/string.h"

int main(int argc, char** argv) {
  /* Test CURL
  sapv::HttpRequestHeaders headers =
      sapv::HttpRequestHeaders("../yahoo-finance-req.json");

  sapv::curl::CurlHttpClient client;
  std::unique_ptr<std::string> response =
      client.get("https://finance.yahoo.com/quote/AAPL", headers);
  std::cout << "\n\n" << response->data() << std::endl;
  */

  std::unique_ptr<sapv::portfolio::PortfolioData> portfolio =
      sapv::portfolio::read(
          "/home/carter/Downloads/Portfolio_Positions_Jan-27-2024.csv",
          sapv::portfolio::FIDELITY);

  return 0;
}
