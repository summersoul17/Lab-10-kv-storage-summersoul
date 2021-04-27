//
// Created by lamp on 4/21/21.
//

#include <log_setup.hpp>

boost::log::trivial::severity_level recognize_log_level(const std::string& level){
  if (level == "debug"){
    return boost::log::trivial::debug;
  } else if (level == "warning"){
    return boost::log::trivial::warning;
  } else if (level == "error") {
    return boost::log::trivial::error;
  } else {
    return boost::log::trivial::trace;
  }
}

void log_setup::init(const std::string& log_level){

  boost::log::add_common_attributes();

  boost::log::core::get()->set_filter(
      boost::log::trivial::severity >= recognize_log_level(log_level)
      );

  boost::log::add_console_log(
      std::cout, boost::log::keywords::format =
          "[%TimeStamp%][%Severity%][%ThreadID%]: %Message%");
  boost::log::add_file_log(
      boost::log::keywords::file_name = "log_%N.log",
      boost::log::keywords::rotation_size = 10 * 1024 * 1024,
      boost::log::keywords::time_based_rotation =
          boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
      boost::log::keywords::format =
          "[%TimeStamp%][%Severity%][%ThreadID%]: %Message%");

  std::srand(time(nullptr));
}
