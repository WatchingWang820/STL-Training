#pragma once

#include "libevent/event_engine.h"

#include <atomic>
#include <memory>
#include <string>

class BaseWorker {
 public:
  using SyncTask = std::function<int(void)>;

  explicit BaseWorker(const std::string& thread_name, int priorities = 1);
  void AsyncCall(EventAsyncQueue::Task&& task);
  int SyncCall(SyncTask&& task);
  EventTimer* StartTimer(EventTimer::timer_callback_type&& f, uint64_t ms,
                         bool persist = false);
  void WaitForAll();

  std::string GetThreadName() const { return thread_name_; }
  std::thread::id GetThreadId() const { return thread_->get_id(); }
  void Stop(bool wait_for_all = true);

  BaseWorker() = delete;

 private:
  std::atomic_bool started_{false};

  std::string thread_name_;
  std::unique_ptr<std::thread> thread_;

  std::unique_ptr<EventEngine> event_engine_;
  std::unique_ptr<EventAsyncQueue> async_queue_;
};
