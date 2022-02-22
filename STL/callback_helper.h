#pragma once

#include <functional>
#include <memory>
#include <set>
#include "src/util/thread/BaseWorker.h"

template <typename T>
class CallBackHelper {
  using CallBackFunction = std::function<void(std::shared_ptr<T> callback)>;

 public:
  CallBackHelper(WorkerType worker) : worker_(worker) {}

  void Register(std::shared_ptr<T> callback) {
    worker_->SyncCall([=] {
      callbacks_.insert(callback);
      return 0;
    });
  }

  void Deregister(std::shared_ptr<T> callback) {
    worker_->SyncCall([=] {
      callbacks_.erase(callback);
      return 0;
    });
  }

  void Send(CallBackFunction&& func) {
    worker_->SyncCall([=] {
      for (const auto& callback : callbacks_) {
        func(callback);
      }
    });
  }

  void Post(CallBackFunction&& func) {
    worker_->AsyncCall([=] {
      for (const auto& callback : callbacks_) {
        func(callback);
      }
    });
  }

 private:
  WorkerType worker_;
  std::set<std::shared_ptr<T>> callbacks_;
};