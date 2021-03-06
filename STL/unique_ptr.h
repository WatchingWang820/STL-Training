#pragma once
#include "deleter.h"
namespace watching {

template <typename T, typename deleter = default_deleter<T>>
class unique_ptr {
 public:
  unique_ptr(T* ptr) : ptr_(ptr) { 
      cout << "default"; }
  template <typename RT>
  unique_ptr(RT* ptr) : ptr_(ptr) {
    cout << "default";
  }

  unique_ptr(unique_ptr&& rhs) {
    ptr_ = rhs.Get();
    rhs.ptr_ = nullptr;
	del = rhs.del;
  }

  template <typename RT>
  unique_ptr(unique_ptr<RT>&& rhs) {
    ptr_ = rhs.ptr_;
    rhs.ptr_ = nullptr;
    del = rhs.del;
  }

  T* Get() { return ptr_; }

  ~unique_ptr() { del(ptr_); }

 private:
  template <class, class>
  friend class unique_ptr;

  deleter del;
  T* ptr_ = nullptr;
};

template <typename T>
unique_ptr<T> make_unique(T* ptr) {
  return unique_ptr<T>(ptr);
}

template <typename L, typename R>
unique_ptr<L> static_pointer_cast(unique_ptr<R> ptr) {
	return ptr;
}

}  // namespace watching
