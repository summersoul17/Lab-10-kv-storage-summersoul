//
// Created by lamp on 4/21/21.
//

#include <hashser.hpp>

std::string hasher::encode_data(const std::string &key, const std::string &value)  {
  return picosha2::hash256_hex_string(std::string(key + value));
}