#pragma once
 namespace watching {

struct InputIterTag {};
struct ForwardIterTag : public InputIterTag {};
struct RandomAccessIterTag : public ForwardIterTag {};

template <typename IterCategory, typename ValueType, typename Pointer = ValueType*,
          typename Reference = ValueType&>
class Iterator {
  using iterator_category = IterCategory;
  using value_type = ValueType;
  using pointer = Pointer;
  using reference = Reference;
};

template <typename T>
struct iterator_traits {
using iterator_category = typename T::iterator_category;
};

template <typename T>
struct iterator_traits<T*> {
  using iterator_category = RandomAccessIterTag;
};

template <typename T>
struct iterator_traits<const T*> {
  using iterator_category = RandomAccessIterTag;
};

template<typename T>
inline typename iterator_traits<T>::iterator_category iterator_category(T) {
  return iterator_traits<T>::iterator_category();
}

template<typename T>
T __advance(T pos, size_t n, RandomAccessIterTag) {
  return pos + n;
}

template <typename T>
T __advance(T pos, size_t n, ForwardIterTag) {
  while (--n) {
    pos++;
  }
  
  return pos;
}

// value_type is the same
template<typename T>
T advance(T pos, size_t n) {
  return __advance(pos, n, iterator_category(T()));

  //return __advance(pos, n, iterator_traits<T>::iterator_category());
}
} 

 template <typename T, size_t N>
 constexpr size_t array_size(T(&arr)[N]) {
  for (size_t i = 0; i < N; i++) {
     *arr = '5';
  }
  return N;
}