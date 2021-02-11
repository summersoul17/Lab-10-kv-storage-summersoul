//
// Created by lamp on 05.02.2021.
//

#ifndef KV_STORAGE_HASHSER_HPP
#define KV_STORAGE_HASHSER_HPP
#include <picosha2.h>
#include <string>

using string = std::string;

string encode_data(const string &key, const string &value) {
  return picosha2::hash256_hex_string(string(key + value));
}

#endif  // KV_STORAGE_HASHSER_HPP
