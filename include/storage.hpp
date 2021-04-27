//
// Created by lamp on 05.02.2021.
//

#ifndef TEMPLATE_DATABASE_HPP
#define TEMPLATE_DATABASE_HPP

#include <common.hpp>

class storage {
 public:
  storage() = delete;
  storage(const storage&) = delete;
  storage operator=(const storage&) = delete;
  explicit storage(std::string db_name);
  void destroy() { _status = _database->Close(); STATUS(_status); }
  ~storage() { destroy(); };

 public:
  void generate();
  [[nodiscard]] std::string name() const { return _name; }


  void get_column_families(column_families_queue&);
  void set_column_families(column_families_map&);

 private:
  void set_database(descriptors_t& families, handles_t& handles);
  void get_database(descriptors_t& families, handles_t& handles);
  void fill_column_family(handles_t& handle);

 public:
  template <typename ostream_>
  void print_database(ostream_& os){
    using namespace rocksdb;

    descriptors_t families;
    handles_t handles;

    std::vector<std::string> families_names{};
    _status =
        rocksdb::DB::ListColumnFamilies(DBOptions(), _name, &families_names);
    STATUS(_status);
    BOOST_LOG_TRIVIAL(debug) << "PRINT: Set " << families_names.size()
                             << " families";
    for (auto& name : families_names) {
      BOOST_LOG_TRIVIAL(debug) << "PRINT: Setting up family " << name;
      families.emplace_back(name, ColumnFamilyOptions());
    }

    get_database(families, handles);

    os << "Printing database: " << _name << '\n';
    for (auto& handle : handles) {
      print_column_family(os, handle);
    }

    destroy_handles(handles);
  }
  template <typename ostream_>
  void print_column_family(ostream_& os,
                           rocksdb::ColumnFamilyHandle* handle){
    os << "\nColumn family: " << handle->GetName() << '\n';
    auto it = _database->NewIterator(rocksdb::ReadOptions(), handle);
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
      os << it->key().ToString() << ": " << it->value().ToString() << '\n';
    }
    delete it;
  }

  inline void destroy_handles(handles_t& handles) {
    for (auto handle : handles) {
      _status = _database->DestroyColumnFamilyHandle(handle);
    }
    BOOST_LOG_TRIVIAL(debug) << __FUNCTION__
                             << " handles destroyed";
    STATUS(_status);
  }

 protected:
  mutable rocksdb::DB* _database;
  mutable rocksdb::Status _status;
  const std::string _name;
};

#endif  // TEMPLATE_DATABASE_HPP
