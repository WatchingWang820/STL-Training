#pragma once

namespace watching {
struct true_type {};
struct false_type{};
template <typename T>
struct value_type {
  using has_trivial_des = false_type;
  using is_const = false_type;
  using is_reference = false_type;
  using is_raw_pointer = false_type;
};

template <>
struct value_type<char> {
  using has_trivial_des = true_type;
};

template <typename T>
struct value_type<const T> {
  using is_const = true_type;
};

template <typename T>
struct value_type<T*> {
  using is_raw_pointer = true_type;
};

template <typename T>
struct value_type<T&> {
  using is_reference = true_type;
};

template <typename T>
void destroy(T* pointer) {
  pointer->~T();
}

template <typename ForwardIterator>
void __destroy(ForwardIterator start, ForwardIterator end, false_type) {
  for (; start < end; ++start) {
    destroy(&(*start));
  }
}

template <typename ForwardIterator>
void __destroy(ForwardIterator start, ForwardIterator end, true_type) {
  printf("trivial destroy");
}

template <typename T>
void reset(T& value, true_type) {
  printf("can't reset");
}

template <typename T>
void reset(T& value, false_type) {
  value = 0;
}
}  // namespace watching