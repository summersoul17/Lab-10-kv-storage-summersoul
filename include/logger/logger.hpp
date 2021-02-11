//
// Created by lamp on 10.02.2021.
//

#ifndef KV_STORAGE_LOGGER_HPP
#define KV_STORAGE_LOGGER_HPP

#include <boost/thread.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
typedef src::severity_logger<logging::trivial::severity_level> logger_t;

enum log_level {
  error, info, warning
};

class logger {
  static logger_t _lg;
 public:
  template <log_level level>
  static void log_data(const std::string& data){

    switch (level) {
      case log_level::error: {
        BOOST_LOG_SEV(_lg, boost::log::trivial::error) << data;
        break;
      }
      case log_level::warning: {
        BOOST_LOG_SEV(_lg, boost::log::trivial::warning) << data;
        break;
      }
      case log_level::info: {
        BOOST_LOG_SEV(_lg, boost::log::trivial::info) << data;
        break;
      }
    }
  }
  static void initiate() {
    logging::add_file_log(
        keywords::file_name = "logs/log_%5N.log",
        keywords::rotation_size = 10 * 1024 * 1024,
        keywords::time_based_rotation =
            sinks::file::rotation_at_time_point(0, 0, 0),
        keywords::format = "[%TimeStamp%][%Severity%][%ThreadID%]: %Message%");

    srand(time(nullptr));
  }
};

#endif  // KV_STORAGE_LOGGER_HPP
