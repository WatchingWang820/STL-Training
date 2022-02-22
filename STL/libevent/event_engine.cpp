#include "event_engine.h"
#include "event2/thread.h"

EventTimer::EventTimer(event_base* base, timer_callback_type&& f, uint64_t ms,
                       bool persist) {
  int16_t flag = EV_READ;
  if (persist) flag |= EV_PERSIST;
  timer_event_ = ::event_new(base, -1, flag, TimerCallback, this);

  timeval tm;
  tm.tv_sec = decltype(tm.tv_sec)(ms / 1000);
  tm.tv_usec = decltype(tm.tv_usec)((ms % 1000) * 1000);
  evtimer_add(timer_event_, &tm);
}

void EventTimer::TimerCallback(evutil_socket_t fd, int16_t event,
                               void* context) {
  auto event_timer = reinterpret_cast<EventTimer*>(context);
  event_timer->func_();
}

EventTimer::~EventTimer() {
  if (timer_event_) {
    evtimer_del(timer_event_);
    ::event_free(timer_event_);
  }
}

EventEngine::EventEngine(int priorities) {
#if defined(_WIN32)
  WSADATA WSAData;
  WSAStartup(0x101, &WSAData);
  ::evthread_use_windows_threads();
#else
  if (signal(SIGHUP, SIG_IGN) == SIG_ERR) {
    log(LOG_ERROR, "ignore SIGHUP failed.");
  }
  ::evthread_use_pthreads();
#endif

  base_ = event_base_new();
  event_base_priority_init(base_, priorities);
}

EventAsyncQueue* EventEngine::CreateAsyncQueue() {
  return new EventAsyncQueue(base_);
}

EventTimer* EventEngine::CreateTimer(EventTimer::timer_callback_type&& f,
                                     uint64_t ms, bool persist) {
  return new EventTimer(base_, std::move(f), ms, persist);
}

void EventEngine::Run() { event_base_loop(base_, 0); }
void EventEngine::Stop() { event_base_loopbreak(base_); }
