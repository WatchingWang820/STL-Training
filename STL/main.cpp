#include <iostream>
#include <thread>
#include <vector>
#include "allocator.h"
#include "shared_ptr.h"
#include "template_traits.h"
#include "unique_ptr.h"
#include "virtual_map.h"
#include "iterator.h"

#include "iterator_traits.h"
#include "type_traits.h"
#pragma pack(4)
using namespace std;

class Test {
public:
	~Test() {
		printf("test");
	}
};


int main() {
  //{
  //  auto ptr = watching::make_shared<Test>(3, 6);
  //  watching::shared_ptr<Test> ptr2(ptr);
  //  ptr2->b_ = 3;
  //}
   char a[3] = {0};
  Test b[3];
  watching::__destroy(a, a + 3,
                      watching::value_type<char>::has_trivial_des());
  watching::__destroy(b, b + 3, watching::value_type<Test>::has_trivial_des());

  int c = 3;
  watching::reset(c, watching::value_type<decltype(c)>::is_const());

  const int d = 5;
  watching::reset(d, watching::value_type<decltype(d)>::is_const());

  int& e = c;

   watching::reset(d, watching::value_type<decltype(e)>::is_reference());
  int* f = (int*)3;

  watching::advance(f, 3);

  auto size = array_size(a);
  return 0;
}