#include <vector>
#include "allocator.h"
#include "template_traits.h"

int main() {
  int a = 3;
  std::vector<int, watching::allocator<int>> b;

  return 0;
}