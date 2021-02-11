//
// Created by lamp on 10.02.2021.
//

#ifndef KV_STORAGE_CONSOLE_HPP
#define KV_STORAGE_CONSOLE_HPP

#include <boost/program_options.hpp>
#include <boost/log/trivial.hpp>
#include <logger.hpp>
#include <storage.hpp>

namespace po = boost::program_options;

class console{
 private:
  static po::options_description _desc;
  static storage _storage;
  void encode();
 public:
  static void setup();
  static void start(const po::variables_map&);
  static po::options_description& get_desc(){ return _desc; }
  };

#endif  // KV_STORAGE_CONSOLE_HPP
