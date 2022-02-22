#pragma once
#include <condition_variable>
#include <mutex>

class ManualResetEvent {
 public:
  explicit ManualResetEvent(bool init_state = false);
  void Set();
  int Wait(int wait_ms = -1);
  void Reset();

 private:
  std::condition_variable cv_;
  bool signal_;
  std::mutex mtx_;
};

class AutoResetEvent {
 public:
  explicit AutoResetEvent() = default;
  void Set();
  int Wait(int wait_ms = -1);

 private:
  std::condition_variable cv_;
  bool signal_ = false;
  std::mutex mtx_;
};