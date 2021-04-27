//
// Created by lamp on 4/21/21.
//

#ifndef KV_STORAGE_APPLICATION_HPP
#define KV_STORAGE_APPLICATION_HPP

#include <hashser.hpp>
#include <common.hpp>
#include <log_setup.hpp>
#include <storage.hpp>
#include <thread>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

const static std::string db_gen = "db_gen";
const static std::string hash_gen = "hash_gen";

class application {
 public:
  application();

  po::variables_map parse_command_line(int argc, char** argv);
  bool set_settings(const po::variables_map& vm);
  [[nodiscard]] std::string mode() const { return _impl->_mode; }

 public:
  void generate_db();
  void hash_db();

 private:

  struct impl {
    impl() = default;
    impl(const impl&) = delete;
    ~impl() { delete _database; }
    void set_database(const std::string& name) {
      delete _database;
      _database = new storage(name);
    }
    size_t _threads_count = std::thread::hardware_concurrency();
    std::string _mode = db_gen;
    std::string _output_db;
    std::string _input_db;
    storage* _database{};
    po::options_description _desc{"Allowed options"};
  };
  std::unique_ptr<impl> _impl;
};

#endif  // KV_STORAGE_APPLICATION_HPP
