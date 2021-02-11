//
// Created by lamp on 05.02.2021.
//

#ifndef KV_STORAGE_SYNC_QUEUE_HPP
#define KV_STORAGE_SYNC_QUEUE_HPP

#include <queue>
#include <mutex>
template <typename T>
class sync_queue {
 private:
  std::queue<T> queue;
  std::mutex mutex;
 public:
  T pop() {
    std::unique_lock<std::mutex> locker;
    return queue.pop();
  }
  void pop(T& item){
    std::unique_lock<std::mutex> locker;
    item = queue.front();
    queue.pop();
  }
  void push(const T& item){
    std::unique_lock<std::mutex> locker;
    queue.push(item);
  }
  sync_queue() = default;
  sync_queue(const sync_queue&) = delete;
  sync_queue operator=(const sync_queue&) = delete;
};

#endif  // KV_STORAGE_SYNC_QUEUE_HPP
