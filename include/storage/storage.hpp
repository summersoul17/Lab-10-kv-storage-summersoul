//
// Created by lamp on 05.02.2021.
//

#ifndef TEMPLATE_DATABASE_HPP
#define TEMPLATE_DATABASE_HPP

#include <sstream>
#include <rocksdb/db.h>
#include <memory>
#include <queue>
class storage {
 public:
  storage(const storage&) = delete;
  storage operator=(const storage&) = delete;
  storage();
  virtual ~storage();
  std::string get(const std::string& key,
                  bool* found);
  void set(const std::string& key,
           const std::string& value);
  void del(const std::string& key,
           bool* found);
  [[nodiscard]] std::string to_string() const;

  std::string operator[](const std::string& key);
  std::queue<std::pair<std::string, std::string>> operator()();
 private:
  rocksdb::DB* _db;
};

#endif  // TEMPLATE_DATABASE_HPP
