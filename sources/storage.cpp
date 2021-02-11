//
// Created by lamp on 05.02.2021.
//

#include <storage.hpp>

storage::storage() {

  _db = nullptr;

  rocksdb::Options options;

  options.create_if_missing = true;

  rocksdb::Status status = rocksdb::DB::Open(options, "data", &(_db));

  if(!status.ok()) {
    throw std::runtime_error { "DB::Open Failed!"};
  }
}

storage::~storage() {
  if (_db) {
    delete _db;
  }
}
std::string storage::get(const std::string& key, bool* found) {
  std::string value;
  rocksdb::Status status = _db->Get(rocksdb::ReadOptions(), key, &value);
  *found = status.ok();
  return value;
}

void storage::set(const std::string& key, const std::string& value) {
  rocksdb::Status status = _db->Put(rocksdb::WriteOptions(), key, value);
  if(!status.ok()) {
    throw std::runtime_error {"DB::set failed!"};
  }
}

void storage::del(const std::string& key, bool* found) {
  std::string value;
  rocksdb::Status status = _db->Get(rocksdb::ReadOptions(), key, &value);
  *found = status.ok();

  if (!*found) {
    return;
  }
  status = _db->Delete(rocksdb::WriteOptions(), key);

  *found = status.ok();
}

std::string storage::to_string() const {
  rocksdb::Iterator* it = _db->NewIterator(rocksdb::ReadOptions());
  std::stringstream ss;
  for (it->SeekToFirst(); it->Valid(); it->Next()) {
    ss << it->key().ToString() << ": " << it->value().ToString() << "\n";
  }
  assert(it->status().ok());
  delete it;
  return ss.str();
}

std::queue<std::pair<std::string, std::string>> storage::operator()(){
  rocksdb::Iterator* it = _db->NewIterator(rocksdb::ReadOptions());
  std::queue<std::pair<std::string, std::string>> queue;
  for (it->SeekToFirst(); it->Valid(); it->Next()) {
    queue.push(std::make_pair<std::string, std::string>(
        it->key().ToString(), it->value().ToString())
               );
  }
  assert(it->status().ok());
  delete it;
  return queue;
}

std::string storage::operator[](const std::string& key) {
  std::string value;
  rocksdb::Status status = _db->Get(rocksdb::ReadOptions(), key, &value);
  if (status.ok()) {
    return value;
  } else {
    return "INVALID KEY";
  }
}