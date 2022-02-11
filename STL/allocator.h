#pragma once

#include <climits>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <new>

using namespace std;

namespace watching {
template <typename T>
inline T* _allocator(ptrdiff_t size, T*) {
  set_new_handler(0);

  T* tmp = ::operator new(sizeof(T) * size);
  if (tmp == nullptr) {
    cerr << "out of memory" << endl;
    return nullptr;
  }
  return tmp;
}

template <typename T>
inline void _deallocate(T* buff) {
  ::operator delete(buff);
}

template <typename T1, typename ...Arg>
inline void _construct(T1* p, const Arg... value) {
  new (p) T1(std::forward<Arg>(value)...);
}

template <typename T>
inline void _destroy(T* ptr) {
  ptr->~T();
}

template <typename T>
class allocator {
 public:
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;
  using difference_type = ptrdiff_t;

  pointer allocate(size_type n, const void* hint = 0) {
    return _allocate(n, (pointer)0);
  }

  void deallocate(pointer p) { _deallocate(p); }

  void construct(pointer p, const_reference value) { _construct(p, value); }

  void destroy(pointer p) { _destroy(p); }

  pointer address(reference x) { return (pointer)&x; }

  const_pointer const_address(reference x) { return (const_pointer)&x; }

  size_type max_size() const { return size_type(UINT_MAX / sizeof(T)); }
};
}
