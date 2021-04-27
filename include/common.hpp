//
// Created by lamp on 4/27/21.
//

#ifndef KV_STORAGE_COMMON_HPP
#define KV_STORAGE_COMMON_HPP

#include <sstream>
#include <rocksdb/db.h>
#include <memory>
#include <queue>
#include <unordered_map>
#include <log_setup.hpp>
#include <boost/shared_ptr.hpp>
#include <random>
#include <set>
#include <unordered_map>
#include <picosha2.h>
#include <string>
#include <thread>

using descriptors_t = std::vector<rocksdb::ColumnFamilyDescriptor>;
using handles_t = std::vector<rocksdb::ColumnFamilyHandle*>;
using string_pair = std::pair<std::string, std::string>;

#define STATUS(status) {if (!(status).ok()) { std::cerr << __FUNCTION__ << "(" <<  __LINE__ << "): " << (status).ToString() << '\n'; return;}}

struct kv_comparator {
  bool operator()(const string_pair& lhs, const string_pair& rhs) const {
    return atoi(lhs.first.data()) < atoi(rhs.first.data());
  }
};

using pair_set = std::set<string_pair, kv_comparator>;
using column_families_queue = std::queue<std::pair<std::string, pair_set>>;
using column_families_map = std::unordered_map<std::string, pair_set>;

#endif  // KV_STORAGE_COMMON_HPP
