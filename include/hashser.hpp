//
// Created by lamp on 05.02.2021.
//

#ifndef KV_STORAGE_HASHSER_HPP
#define KV_STORAGE_HASHSER_HPP
#include <picosha2.h>
#include <string>

namespace hasher {
  std::string encode_data(const std::string &key, const std::string &value);
}
#endif  // KV_STORAGE_HASHSER_HPP
