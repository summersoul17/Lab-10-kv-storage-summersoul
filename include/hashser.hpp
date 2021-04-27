//
// Created by lamp on 05.02.2021.
//

#ifndef KV_STORAGE_HASHSER_HPP
#define KV_STORAGE_HASHSER_HPP

#include <common.hpp>

class hasher {
 public:
  hasher(const hasher&) = delete;
  hasher(column_families_map& map, column_families_queue& queue, size_t threads)
      : _threads_count(threads), _queue(queue), _map(map) {
    _threads.reserve(threads);
  }
  ~hasher(){
    for (auto& thread : _threads){
      if (thread.joinable()){
        thread.join();
      }
    }
  }
  static std::string encode_data(const std::string &key, const std::string &value);

  void start_hashing();
 private:
  void hash_func();
  size_t _threads_count;
  std::vector<std::thread> _threads;
  mutable std::mutex _queue_mutex;
  mutable std::mutex _map_mutex;
  column_families_queue& _queue;
  column_families_map& _map;
};

#endif  // KV_STORAGE_HASHSER_HPP
