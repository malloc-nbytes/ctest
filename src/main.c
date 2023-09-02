#define CTEST_IMPL
#include "./include/ctest.h"

int func1() {
  return 1;
}

int func2() {
  return 1;
}

int func3() {
  return 1;
}

int func4() {
  return 1;
}

int main(void) {
  START_CTEST;

  ASSERT_TRUE(func1() == func2());
  ASSERT_TRUE(func3() == func4());
  ASSERT_EQ(func1(), func2());

  END_CTEST;
}
