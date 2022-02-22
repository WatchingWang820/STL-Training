#include "BaseWorker.h"
#include <event2/thread.h>
#include "src/util/lock/waitable_event.h"

BaseWorker::BaseWorker(const std::string& thread_name, int priorities)
    : thread_name_(thread_name) {
  event_engine_ = std::make_unique<EventEngine>();
  async_queue_.reset(event_engine_->CreateAsyncQueue());

  thread_ = std::make_unique<std::thread>([this] {
    started_ = true;
    event_engine_->Run();
    started_ = false;
  });
}

void BaseWorker::AsyncCall(EventAsyncQueue::Task&& task) {
  async_queue_->PushTask(std::move(task));
}

int BaseWorker::SyncCall(SyncTask&& task) {
  if (std::this_thread::get_id() == thread_->get_id()) {
    return task();
  }

  auto manual_event = std::make_shared<ManualResetEvent>();

  int ret;
  EventAsyncQueue::Task sync_task = [&, manual_event] {
    ret = task();
    manual_event->Set();
  };

  async_queue_->PushTask(sync_task);
  manual_event->Wait();

  return ret;
}

EventTimer* BaseWorker::StartTimer(EventTimer::timer_callback_type&& f,
                                   uint64_t ms, bool persist) {
  return event_engine_->CreateTimer(std::move(f), ms, persist);
}

void BaseWorker::WaitForAll() {
  SyncCall([] { return 0; });
}

void BaseWorker::Stop(bool wait_for_all) {
  if (wait_for_all) {
    WaitForAll();
  }

  event_engine_->Stop();
}