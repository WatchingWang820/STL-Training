#pragma once
#include <atomic>
#include "deleter.h"

namespace watching {
template <typename T>
class RefCountedPtr {
 public:
  explicit RefCountedPtr(T* ptr) : ptr_(ptr) {}

  void Incref() { ++uses_; }
  void Decref() {
    --uses_;
    if (uses_ == 0) {
      destroy();
    }
  }
  void Inwcref() { ++weaks_; }
  void Dewcref() {
    --weaks_;
    if (weaks_ == 0) {
      delete this;
    }
  }

  virtual void destroy() { delete ptr_; }

  bool Expire() { return uses_ == 0; }
  T* GetPtr() { return ptr_; }
  RefCountedPtr(RefCountedPtr* rhs) = delete;

 private:
  T* ptr_;
  std::atomic<uint64_t> uses_ = {1};
  std::atomic<uint64_t> weaks_ = {1};
};

template <typename T>
class RefCountedObject : public RefCountedPtr<T> {
 public:
  template <typename... Args>
  RefCountedObject(Args... args) : RefCountedPtr<T>(&obj) {
    new (&obj) T(std::forward<Args>(args)...);
  }

  void destroy() override { obj.~T(); }

 private:
  T obj;
};

template <typename T>
class shared_ptr {
 public:
  shared_ptr(){};
  explicit shared_ptr(T* ptr) : ptr_(ptr) {
    ref_ptr_ = new RefCountedPtr<T>(ptr);
  }
  void SetRefCountedPtr(RefCountedPtr<T>* ref_ptr) {
    ref_ptr_ = ref_ptr;
    ptr_ = ref_ptr_->GetPtr();
  }

  template <typename U>
  shared_ptr<T>(shared_ptr<U> rhs) {
    this->ref_ptr_ = rhs.ref_ptr_;
    this->ptr_ = rhs.ptr_;
    IncrefAll();
  }

  template <typename U>
  shared_ptr<T> operator=(shared_ptr<U> rhs) {
    return shared_ptr<T>(rhs);
  }

  shared_ptr<T> operator=(shared_ptr<T> rhs) { return shared_ptr<T>(rhs); }

  T* operator->() { return ptr_; }

  void IncrefAll() {
    ref_ptr_->Incref();
    ref_ptr_->Inwcref();
  }

  void Destroy() {
    ref_ptr_->Decref();
    ref_ptr_->Dewcref();
  }

  ~shared_ptr() { Destroy(); }

 private:
  T* ptr_ = nullptr;
  RefCountedPtr<T>* ref_ptr_ = nullptr;
};

template <typename T, typename... Args>
shared_ptr<T> make_shared(Args... args) {
  auto ref_ptr = new RefCountedObject<T>(std::forward<Args>(args)...);
  shared_ptr<T> ptr;
  ptr.SetRefCountedPtr(ref_ptr);
  ptr.IncrefAll(); 
  return ptr;
}
}  // namespace watching