#pragma once
#include <mutex>
#include <string>
#include <thread>
#include <vector>

template <typename ThreadType>
class ThreadPool {
 public:
  using ThreadTypeSharedPtr = std::shared_ptr<ThreadType>;

 public:
  ThreadPool() {
    size_t concurrency = std::thread::hardware_concurrency();
    capacity_ = concurrency ? 2 * concurrency + 1 : 2;

    thread_pool_.reserve(capacity_);
  }

  ThreadTypeSharedPtr GetOne(std::string thread_name, bool permanent = false) {
    auto it = std::find_if(thread_pool_.begin(), thread_pool_.end(),
                           [thread_name](ThreadTypeSharedPtr member) {
                             return member->GetThreadName().find(thread_name) !=
                                    std::string::npos;
                           });

    if (it != thread_pool_.end()) {
      return *it;
    }

    if (capacity_ <= thread_pool_.size()) {
      return nullptr;
    }

    thread_pool_.emplace_back(std::move(new ThreadType(thread_name)));
    return GetOne(thread_name);
  }

  void ClearAll() {
    for (auto& thread : thread_pool_) {
      thread->Stop();
    }
  }

 private:
  size_t capacity_ = 2;
  std::vector<ThreadTypeSharedPtr> thread_pool_;
  std::mutex pool_mutex_;
};
