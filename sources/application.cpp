//
// Created by lamp on 4/21/21.
//
#include <application.hpp>

application::application(): _impl(new impl()){
  _impl->_desc.add_options()
      ("help,h", "Shows help message")
      ("pretty-mode,p", "Sets pretty output mode, friendly to people")
      ("mode,m", po::value<std::string>(), "Sets mode. |db_gen|hash_gen|")
      ("input,i", po::value<std::string>(), "Sets input db. Necessary attribute")
      ("log-level,l", po::value<std::string>(), "Sets logging level. Default: trace")
      ("thread-count,t", po::value<int>(), "Sets threads count. Default: count of logical core")
      ("output,o", po::value<std::string>(), "Sets output db. Default: input db");
}

po::variables_map application::parse_command_line(int argc, char **argv) {
  po::variables_map vm;
  po::parsed_options parsed = po::command_line_parser(argc, argv)
      .options(_impl->_desc)
      .allow_unregistered()
      .run();
  po::store(parsed, vm);
  po::notify(vm);
  return vm;
}
bool application::set_settings(const po::variables_map& vm) {
  try{
    if (vm.count("help")){
      BOOST_LOG_TRIVIAL(info) << "Usage:\n" << _impl->_desc;
      return false;
    }

    if (vm.count("mode")){
      _impl->_mode = vm.at("mode").as<std::string>();
    }

    if (vm.count("input") && _impl->_mode == hash_gen){
      if (vm.count("input")){
        _impl->_input_db = vm.at("input").as<std::string>();
      } else {
        throw std::runtime_error{"Couldn't find input database name"};
      }
    }
    if (vm.count("output")){
      _impl->_output_db = vm.at("output").as<std::string>();
    } else {
      _impl->_output_db = "output.database";
    }

    std::string log_level = "trace";

    if (vm.count("log-level")){
      log_level = vm.at("log-level").as<std::string>();
    }
    log_setup::init(log_level);

    if (vm.count("thread-count")){
      _impl->_threads_count = vm.at("thread-count").as<int>();
    }

    return true;
  } catch (const std::exception& e){
    BOOST_LOG_TRIVIAL(fatal) << "Command line parsing error: " << e.what()
        << "\nUsage:\n" << _impl->_desc;
    return false;
  }
}

void application::generate_db() {
  BOOST_LOG_TRIVIAL(info) << "Setting up new random db: " << _impl->_output_db;
  _impl->set_database(_impl->_output_db);
  _impl->_database->generate();


  _impl->_database->print_database(std::cout);
}

void application::hash_db() {
  BOOST_LOG_TRIVIAL(info) << "Starting hashing db: " << _impl->_input_db;
  _impl->set_database(_impl->_input_db);

  column_families_queue queue{};
  _impl->_database->get_column_families(queue);
  BOOST_LOG_TRIVIAL(info) << "Hashing db";
  column_families_map hashed_map{};
  {
    hasher h(hashed_map, queue, _impl->_threads_count);
    h.start_hashing();
  }

  BOOST_LOG_TRIVIAL(info) << "Writing data to db: " << _impl->_output_db;
  _impl->set_database(_impl->_output_db);

  _impl->_database->set_column_families(hashed_map);

  _impl->_database->print_database(std::cout);
}
