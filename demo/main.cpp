#include <console.hpp>
#include <iostream>

int main(int argc, char** argv) {
  console::setup();
  po::variables_map vm;
  po::parsed_options options = po::command_line_parser(argc, argv)
                                   .options(console::get_desc())
                                   .allow_unregistered().run();
  po::store(options, vm);
  po::notify(vm);

  console::start(vm);
}