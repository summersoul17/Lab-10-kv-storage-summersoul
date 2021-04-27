//
// Created by lamp on 05.02.2021.
//

#include <storage.hpp>
#include <utility>
#include <iomanip>

storage::storage(std::string  db_name):_name(std::move(db_name)){}


void storage::set_database(descriptors_t & families, handles_t & handles) {
  using namespace rocksdb;

  Options options{};
  options.create_if_missing = true;
  options.create_missing_column_families = true;
  options.error_if_exists = true;
  _status = DB::Open(options, _name, families, &handles, &_database);
  STATUS(_status);
}

void storage::get_database(descriptors_t& families, handles_t & handles) {
  using namespace rocksdb;
  Options options{};
  options.create_missing_column_families = true;

  _status = DB::OpenForReadOnly(options, _name, families, &handles, &_database);
  STATUS(_status);
}

std::string generate_random_string(){
  std::string out{};

  for (size_t i = 0; i < random_string_length; ++i){
    out += (std::to_string(rand()%10));
  }
  return out;
}

void storage::fill_column_family(handles_t & handles) {
  using namespace rocksdb;
  for (auto& handle : handles) {
    WriteBatch batch{};

    size_t length = rand() % max_random_family_length + 1;
    BOOST_LOG_TRIVIAL(debug)
        << "Setting family " << handle->GetName() << " with size " << length;
    for (size_t i = 0; i < length; ++i) {
      std::stringstream key{};
      key << std::setw(key_width) << std::setfill('0') << (i+1);
      _status = batch.Put(handle, key.str(),
                          generate_random_string());
    }
    STATUS(_status);
    _status = _database->Write(WriteOptions(), &batch);
  }
  STATUS(_status);
}

void storage::generate() {
  using namespace rocksdb;

  descriptors_t families;
  handles_t handles;

  families.emplace_back(kDefaultColumnFamilyName, ColumnFamilyOptions());
  size_t families_count = rand() % max_families + 1;
  families.reserve(families_count);
  for (size_t i = 0; i < families_count; ++i) {
    families.emplace_back("column_family_" + std::to_string(i + 1),
                          ColumnFamilyOptions());
  }
  BOOST_LOG_TRIVIAL(trace) << "Set " << families.size() << " families";

  set_database(families, handles);
  fill_column_family(handles);
  destroy_handles(handles);
}

void storage::get_column_families(column_families_queue& queue) {
  using namespace rocksdb;

  descriptors_t families;
  handles_t handles;

  std::vector<std::string> families_names{};
  _status =
      rocksdb::DB::ListColumnFamilies(DBOptions(), _name, &families_names);
  STATUS(_status);

  BOOST_LOG_TRIVIAL(debug) << "GET: Set " << families_names.size()
                           << " families";
  for (auto& name : families_names) {
    families.emplace_back(name, ColumnFamilyOptions());
  }

  get_database(families, handles);

  for (auto& handle : handles) {
    pair_set set{};
    auto it = _database->NewIterator(rocksdb::ReadOptions(), handle);
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
      std::string key = it->key().ToString(), value = it->value().ToString();
      set.emplace(std::move(key), std::move(value));
    }
    BOOST_LOG_TRIVIAL(debug) << "GET: Setting up family " << handle->GetName()
                             << " with values " << set.size();
    queue.emplace(handle->GetName(), std::move(set));
    delete it;
  }
  destroy_handles(handles);
}

rocksdb::ColumnFamilyHandle*& find_column_family(const std::string& name, handles_t& handles) {
  for (auto& handle : handles){
    if (handle->GetName() == name){
      BOOST_LOG_TRIVIAL(debug) << "Column family found";
      return handle;
    }
  }
  BOOST_LOG_TRIVIAL(error) << "Column family not found!";
  throw std::runtime_error{"Column family not found!"};
}

void storage::set_column_families(column_families_map& map) {
  using namespace rocksdb;

  descriptors_t families;
  handles_t handles;

  for (auto& column_fam : map) {
    families.emplace_back(column_fam.first, ColumnFamilyOptions());
  }

  set_database(families, handles);

  for (auto& column_fam : map){
    WriteBatch batch{};
    for (auto& values : column_fam.second){
      _status = batch.Put(find_column_family(column_fam.first, handles), values.first, values.second);
    }
    _database->Write(WriteOptions(), &batch);
  }
  STATUS(_status);

  destroy_handles(handles);
}