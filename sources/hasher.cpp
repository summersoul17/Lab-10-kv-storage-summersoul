//
// Created by lamp on 4/21/21.
//

#include <hashser.hpp>

std::string hasher::encode_data(const std::string &key, const std::string &value) {
  return picosha2::hash256_hex_string(std::string(key + value));
}

void hasher::start_hashing(){
  for (size_t i = 0; i < _threads_count; ++i) {
    _threads.emplace_back(std::thread(&hasher::hash_func, this));
  }
}

void hasher::hash_func(){
  while (!_queue.empty()) {
    _queue_mutex.lock();
    if (_queue.empty()) {
      _queue_mutex.unlock();
      continue;
    }
    auto family = _queue.front();
    _queue.pop();
    _queue_mutex.unlock();
    pair_set set{};
    for (auto& values : family.second) {
      set.emplace(values.first,
                  hasher::encode_data(values.first, values.second));
    }

    _map_mutex.lock();
    _map.emplace(family.first, std::move(set));
    _map_mutex.unlock();
  }
}