#include <application.hpp>
#include <iostream>

int main(int argc, char** argv) {
  application app{};
  if (!app.set_settings(app.parse_command_line(argc, argv))){
    return 1;
  } else {
    if (app.mode() == db_gen){
      BOOST_LOG_TRIVIAL(trace) << "Starting database generation";
      app.generate_db();
      return 0;
    } else if (app.mode() == hash_gen){
      BOOST_LOG_TRIVIAL(trace) << "Starting database hashing";
      app.hash_db();
      return 0;
    } else {
      BOOST_LOG_TRIVIAL(fatal) << "Unknown mode: " << app.mode();
      return 1;
    }
  }
}
