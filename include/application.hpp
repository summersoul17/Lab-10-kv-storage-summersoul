//
// Created by lamp on 4/21/21.
//

#ifndef KV_STORAGE_APPLICATION_HPP
#define KV_STORAGE_APPLICATION_HPP

#include <log_setup.hpp>
#include <storage.hpp>

#include <thread>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

// TODO: do norm
const static std::string db_gen = "db_gen";
const static std::string hash_gen = "hash_gen";

class application {
 public:
  application();

  po::variables_map parse_command_line(int argc, char** argv);
  bool set_settings(const po::variables_map& vm);
  void generate_random_values(size_t size);
  void hash_database();

  std::string mode() const { return _mode; }
 private:
  size_t _threads_count = std::thread::hardware_concurrency();
  std::string _mode = db_gen;
  std::string _input_db;
  std::string _output_db;
  std::unique_ptr<storage> _database;
  po::options_description _desc{"Allowed options"};

};

#endif  // KV_STORAGE_APPLICATION_HPP
