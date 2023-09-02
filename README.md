# ctests

CTests is a small testing library for testing C/C++ code.

# Usage
1. put `#define CTEST_IMPL` at the top of the file that is being tested in
2. `#include "./ctest.h"` right beneath it
3. put `CTEST_START` at the beginning of the main function
4. put `CTEST_END` at the end of the main function
5. *optional* `#define CTEST_ABORT_ON_FAIL` to immediately abort tests on a failure

# Functionality

`CTEST_ASSERT_TRUE(expr)` - asserts that the expression is true

`CTEST_ASSERT_FALSE(expr)` - asserts that the expression is false

`CTEST_ASSERT_EQ(expr, expr)` - asserts that the two expressions are equal

`CTEST_ASSERT_NEQ(expr, expr)` - asserts that the two expressions are not equal

`CTEST_ASSERT_STR_EQ(expr, expr)` - asserts that the two expressions are equal strings

`CTEST_ASSERT_STR_NEQ(expr, expr)` - asserts that the two expressions are not equal strings

# Examples

## Passed Tests

```c
#define CTEST_IMPL
// #define CTEST_ABORT_ON_FAIL
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
  
  CTEST_ASSERT_TRUE(x == 1);
  CTEST_ASSERT_FALSE(x == 2);

  CTEST_ASSERT_FALSE(func1(0) == func2());
  CTEST_ASSERT_TRUE(func1(1) == func2());
  CTEST_ASSERT_EQ(func1(1), func2());
  CTEST_ASSERT_NEQ(func1(0), func2());

  CTEST_ASSERT_STR_EQ("hello", "hello");
  CTEST_ASSERT_STR_NEQ("hello", "test");

  CTEST_END;
}
```

*Output*

```
========== CTest Results ==========
main.c:main:18 -> CTEST_ASSERT_TRUE(x == 1) ... ok
main.c:main:19 -> CTEST_ASSERT_FALSE(x == 2) ... ok
main.c:main:21 -> CTEST_ASSERT_FALSE(func1(0) == func2()) ... ok
main.c:main:22 -> CTEST_ASSERT_TRUE(func1(1) == func2()) ... ok
main.c:main:23 -> CTEST_ASSERT_EQ(func1(1), func2()) ... ok
main.c:main:24 -> CTEST_ASSERT_NEQ(func1(0), func2()) ... ok
main.c:main:26 -> CTEST_ASSERT_STR_EQ("hello", "hello") ... ok
main.c:main:27 -> CTEST_ASSERT_STR_NEQ("hello", "test") ... ok
Passed: 8
Failed: 0
Total: 8
===================================
```

## With Failed Tests
  
```c
#define CTEST_IMPL
// #define CTEST_ABORT_ON_FAIL
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
```

*Output*

```
========== CTest Results ==========
main.c:main:18 -> CTEST_ASSERT_TRUE(x != 1) ... FAILED
main.c:main:19 -> CTEST_ASSERT_FALSE(x == 2) ... ok
main.c:main:21 -> CTEST_ASSERT_FALSE(func1(0) != func2()) ... FAILED
main.c:main:22 -> CTEST_ASSERT_TRUE(func1(1) == func2()) ... ok
main.c:main:23 -> CTEST_ASSERT_EQ(func1(1), func2()) ... ok
main.c:main:24 -> CTEST_ASSERT_NEQ(func1(0), func2()) ... ok
main.c:main:26 -> CTEST_ASSERT_STR_EQ("helo", "hello") ... FAILED
main.c:main:27 -> CTEST_ASSERT_STR_NEQ("hello", "test") ... ok
Passed: 5
Failed: 3
Total: 8
===================================
Aborted (core dumped)
```

### With `CTEST_ABORT_ON_FAIL` defined

*Output*

```
========== CTest Results ==========
main.c:main:19 -> CTEST_ASSERT_TRUE(x != 1) ... FAILED
Passed: 0
Failed: 1
Total: 1
===================================
Aborted (core dumped)
```