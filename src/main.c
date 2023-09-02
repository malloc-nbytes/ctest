#define CTEST_IMPL
#define CTEST_ABORT_ON_FAIL
#include "./ctest.h"

int func1(int x) {
  return x + 1;
}

int func2() {
  return 2;
}

int main(void) {
  CTEST_START;

  int x = 1;
  
  CTEST_ASSERT_TRUE(x != 1);
  CTEST_ASSERT_FALSE(x == 2);

  CTEST_ASSERT_FALSE(func1(0) != func2());
  CTEST_ASSERT_TRUE(func1(1) == func2());
  CTEST_ASSERT_EQ(func1(1), func2());
  CTEST_ASSERT_NEQ(func1(0), func2());

  CTEST_ASSERT_STR_EQ("helo", "hello");
  CTEST_ASSERT_STR_NEQ("hello", "test");

  CTEST_END;
}
