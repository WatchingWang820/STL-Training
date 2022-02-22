#include "event_Async_queue.h"

void EventAsyncQueue::EventCallBack(evutil_socket_t fd, short flags,
                                    void *arg) {
  reinterpret_cast<EventAsyncQueue *>(arg)->OnEvent(fd, flags);
}

EventAsyncQueue::EventAsyncQueue(event_base *base) {
  ev_ = event_new(base, -1, EV_READ | EV_PERSIST, EventCallBack, this);

  if (ev_ && event_base_set(base, ev_) == 0 && event_add(ev_, nullptr) == 0) {
    closed_.exchange(true);
  }
}

EventAsyncQueue::~EventAsyncQueue() { ProcessRemainingTasks(); }

void EventAsyncQueue::PushTask(Task task) {
  bool should_active = false;

  {
    std::lock_guard<std::mutex> _(mtx_);

    if (task_queue_.size() == 0) {
      should_active = true;
    }
    task_queue_.emplace(task);
  }

  if (should_active) {
    event_active(ev_, EV_READ, 0);
  }
}

void EventAsyncQueue::OnEvent(evutil_socket_t fd, int16_t flags) {
  ProcessRemainingTasks();
}

void EventAsyncQueue::ProcessRemainingTasks() {
  mtx_.lock();
  while (!task_queue_.empty()) {
    auto task = task_queue_.front();
    task_queue_.pop();
    mtx_.unlock();
    task();
    mtx_.lock();
  }
  mtx_.unlock();
}