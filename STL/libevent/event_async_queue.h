#pragma once
#include <event2/event.h>

#include <atomic>
#include <functional>
#include <mutex>
#include <queue>

class EventAsyncQueue {
 public:
  using Task = std::function<void(void)>;

  EventAsyncQueue(event_base* base);
  ~EventAsyncQueue();

  void PushTask(Task task);
  void OnEvent(evutil_socket_t fd, int16_t flags);

 private:
  static void EventCallBack(evutil_socket_t fd, short flags, void* arg);
  void ProcessRemainingTasks();

 private:
  std::atomic_bool closed_ = {true};

  event* ev_ = nullptr;
  std::mutex mtx_;
  std::queue<Task> task_queue_;

};
