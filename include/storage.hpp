//
// Created by lamp on 05.02.2021.
//

#ifndef TEMPLATE_DATABASE_HPP
#define TEMPLATE_DATABASE_HPP

#include <hashser.hpp>
#include <sstream>
#include <rocksdb/db.h>
#include <memory>
#include <queue>

class storage {
 public:
  storage(const storage&) = delete;
  storage operator=(const storage&) = delete;
  storage(const std::string& db_name);
  ~storage() = default;
  std::string get(const std::string& key,
                  bool* found);
  void set(const std::string& key,
           const std::string& value);
  void del(const std::string& key,
           bool* found);
  [[nodiscard]] std::string to_string() const;
  std::string operator[](const std::string& key);
  std::queue<std::pair<std::string, std::string>> get_values();
 private:
  std::unique_ptr<rocksdb::DB> _db;
};

#endif  // TEMPLATE_DATABASE_HPP
