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
  CTEST_START;

  ASSERT_STR_EQ("hello", "hello");
  ASSERT_STR_NEQ("hello", "test");

  ASSERT_TRUE(func1() == func2());
  ASSERT_TRUE(func3() == func4());
  ASSERT_EQ(func1(), func2());

  CTEST_END;
}
