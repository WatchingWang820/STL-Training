#include "waitable_event.h"

ManualResetEvent::ManualResetEvent(bool init_state) : signal_(init_state) {}

void ManualResetEvent::Set() {
  std::lock_guard<std::mutex> _(mtx_);
  signal_ = true;
  cv_.notify_all();
}

int ManualResetEvent::Wait(int wait_ms) {
  int ret = -1;

  std::unique_lock<std::mutex> _(mtx_);
  if (wait_ms < 0) {
    cv_.wait(_, [=] { return signal_; });
    ret = 0;
  } else {
    bool succeed = cv_.wait_for(_, std::chrono::milliseconds(wait_ms),
                                [=] { return signal_; });
    ret = succeed ? 0 : -1;
  }
  return ret;
}

void ManualResetEvent::Reset() {
  std::lock_guard<std::mutex> _(mtx_);
  signal_ = false;
}

void AutoResetEvent::Set() {
  std::lock_guard<std::mutex> _(mtx_);
  signal_ = true;
  cv_.notify_all();
}

int AutoResetEvent::Wait(int wait_ms) {
  int ret = -1;

  std::unique_lock<std::mutex> _(mtx_);
  if (wait_ms < 0) {
    cv_.wait(_, [=] { return signal_; });
    ret = 0;
  } else {
    bool succeed = cv_.wait_for(_, std::chrono::milliseconds(wait_ms),
                                [=] { return signal_; });
    ret = succeed ? 0 : -1;
  }

  if (ret == 0) {
    std::lock_guard<std::mutex> _(mtx_);
    signal_ = false;
  }

  return ret;
}
