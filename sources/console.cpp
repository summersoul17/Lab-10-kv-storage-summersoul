//
// Created by lamp on 05.02.2021.
//

#include <console.hpp>
#include <iostream>

po::options_description console::_desc("Allowed Options");
logger_t logger::_lg;

void console::setup() {
  _desc.add_options()
      ("help,h", "help message")
      ("input,i", po::value<std::string>(), "<path/to/input/storage.db> - NECESSARY")
      ("log-level,ll", po::value<std::string>(), "\"info\"|\"warning\"|\"error\" \ndefault: \"error\"")
      ("thread-count,tc", po::value<int>(), "default: count of logical core")
      ("output,o", po::value<std::string>(), "<path/to/output/storage.db>\ndefault: <path/to/input/dbcs-storage.db>")
  ;
  logger::initiate();
}

void console::start(const po::variables_map& vm) {
  if (vm.count("help")) {
    std::stringstream ss;

    ss << "This program is used to generate key - hash database from the file"
              << _desc << "\nCOPYRIGHT 2021 LAMP\n";

    logger::log_data<log_level::info>(ss.str());

    std::cout << ss.str();
    exit(0);
  }

  if (!vm.count("input")) {
    std::stringstream ss;
    ss << "No input database entered\n";

    logger::log_data<log_level::warning>(ss.str());

    std::cout << ss.str();
    exit(0);
  }

  storage st;
  // PRODUCER_CONSUMER

}