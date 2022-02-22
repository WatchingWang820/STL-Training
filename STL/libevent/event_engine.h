#pragma once

#include "event_async_queue.h"

class EventTimer {
 public:
  using timer_callback_type = std::function<void()>;

 public:
  EventTimer(event_base* base, timer_callback_type&& f, uint64_t ms,
             bool persist = false);
  ~EventTimer();
  static void TimerCallback(evutil_socket_t fd, int16_t event, void* context);

 private:
  event* timer_event_ = nullptr;
  timer_callback_type func_;
};

class EventEngine {
 public:
  explicit EventEngine(int priorities = 1);
  EventAsyncQueue* CreateAsyncQueue();
  EventTimer* CreateTimer(EventTimer::timer_callback_type&& f, uint64_t ms,
                          bool persist = false);
  void Run();
  void Stop();

 private:
  event_base* base_ = nullptr;
};