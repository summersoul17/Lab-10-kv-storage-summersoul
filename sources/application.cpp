//
// Created by lamp on 4/21/21.
//
#include <application.hpp>

application::application() {
  _desc.add_options()
      ("help,h", "Shows help message")
      ("mode,m", po::value<std::string>(), "Sets mode. |db_gen|hash_gen|")
      ("input,i", po::value<std::string>(), "Sets input db. Necessary attribute")
      ("log-level,l", po::value<std::string>(), "Sets logging level. Default: trace")
      ("thread-count,t", po::value<int>(), "Sets threads count. Default: count of logical core")
      ("output,o", po::value<std::string>(), "Sets output db. Default: input db");
}
po::variables_map application::parse_command_line(int argc, char **argv) {
  po::variables_map vm;
  po::parsed_options parsed = po::command_line_parser(argc, argv)
      .options(_desc)
      .allow_unregistered()
      .run();
  po::store(parsed, vm);
  po::notify(vm);
  return vm;
}
bool application::set_settings(const po::variables_map& vm) {
  try{
    if (vm.count("help")){
      BOOST_LOG_TRIVIAL(trace) << "Usage:\n" << _desc;
      return false;
    }

    if (vm.count("mode")){
      _mode = vm.at("mode").as<std::string>();
    }

    if (!vm.count("input")){
      throw std::runtime_error{"Not found input DB"};
    }
    _database.reset(new storage(vm.at("input").as<std::string>()));
    BOOST_LOG_TRIVIAL(trace) << "Opening db: " << vm.at("input").as<std::string>();

    std::string log_level = "trace";

    if (vm.count("log-level")){
      log_level = vm.at("log-level").as<std::string>();
    }
    log_setup::init(log_level);

    if (vm.count("thread-count")){
      _threads_count = vm.at("thread-count").as<int>();
    }

    if (vm.count("output")){
      _output_db = vm.at("output").as<std::string>();
    } else {
      _output_db = vm.at("input").as<std::string>();
    }

    return true;
  } catch (const std::exception& e){
    BOOST_LOG_TRIVIAL(fatal) << "Command line parsing error: " << e.what()
        << "\nUsage:\n" << _desc;
    return false;
  }
}

void application::generate_random_values(size_t size){
  for (size_t i = 0; i < size; ++i) {
    std::string key{ "key-" + std::to_string(i+1)};
    std::string value { "value-" + std::to_string(rand()%100)};
    _database->set(key, value);
    BOOST_LOG_TRIVIAL(debug) << "Added " << key << ":" << value << " to db";
  }
  BOOST_LOG_TRIVIAL(trace) << "Database generated: " <<
}

void application::hash_database() {
  auto values = _database->get_values();

  _database.reset(new storage(_output_db));

  for (; !values.empty() ;){
    auto tmp = std::move(values.front());
    _database->set(tmp.first, hasher::encode_data(tmp.first, tmp.second));
    values.pop();
  }

  BOOST_LOG_TRIVIAL(debug) << "Database hashed:\n" << _database->to_string();
  BOOST_LOG_TRIVIAL(trace) << "Hashing done: " << _output_db;
}
