# ctest

A small testing library for testing C/C++ code.

# Usage
1. put `#define CTEST_IMPL` at the top of the file that is being tested in
2. *optional* `#define CTEST_ABORT_ON_FAIL` to immediately abort tests on a failure
3. `#include "./ctest.h"` right beneath it
4. put `CTEST_BEGIN` at the beginning of the main function
5. put `CTEST_END` at the end of the main function

# Functionality

`CTEST_ASSERT_TRUE(expr)` - asserts that the expression is true

`CTEST_ASSERT_FALSE(expr)` - asserts that the expression is false

`CTEST_ASSERT_EQ(expr, expr)` - asserts that the two expressions are equal - shows lhs and rhs if not equal

`CTEST_ASSERT_NEQ(expr, expr)` - asserts that the two expressions are not equal - shows lhs and rhs if equal

`void ctest_tagged_func_create(void (*func)(void), char *name, char *tags);` - creates a tagged function

`void ctest_tagged_funcs_run(char *tag);` - runs all tagged functions with the given tag

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
  CTEST_BEGIN;

  int x = 1;
  
  CTEST_ASSERT_TRUE(x == 1);
  CTEST_ASSERT_FALSE(x == 2);

  CTEST_ASSERT_FALSE(func1(0) == func2());
  CTEST_ASSERT_TRUE(func1(1) == func2());
  CTEST_ASSERT_EQ(func1(1), func2());
  CTEST_ASSERT_NEQ(func1(0), func2());

  CTEST_END;
}
```

*Output*

```
Summary
===================================
main.c:main:18 -> CTEST_ASSERT_TRUE(x == 1) ... ok
main.c:main:19 -> CTEST_ASSERT_FALSE(x == 2) ... ok
main.c:main:21 -> CTEST_ASSERT_FALSE(func1(0) == func2()) ... ok
main.c:main:22 -> CTEST_ASSERT_TRUE(func1(1) == func2()) ... ok
main.c:main:23 -> CTEST_ASSERT_EQ(func1(1), func2()) ... ok
main.c:main:24 -> CTEST_ASSERT_NEQ(func1(0), func2()) ... ok
Passed: 6
Failed: 0
Total: 6
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
  CTEST_BEGIN;

  int x = 1;
  
  CTEST_ASSERT_TRUE(x != 1);
  CTEST_ASSERT_FALSE(x == 2);

  CTEST_ASSERT_FALSE(func1(0) != func2());
  CTEST_ASSERT_TRUE(func1(1) == func2());
  CTEST_ASSERT_EQ(func1(99), func2());
  CTEST_ASSERT_NEQ(func1(0), func2());

  CTEST_END;
}
```

*Output*

```
Summary
===================================
main.c:main:18 -> CTEST_ASSERT_TRUE(x != 1) ... FAILED
main.c:main:19 -> CTEST_ASSERT_FALSE(x == 2) ... ok
main.c:main:21 -> CTEST_ASSERT_FALSE(func1(0) != func2()) ... FAILED
main.c:main:22 -> CTEST_ASSERT_TRUE(func1(1) == func2()) ... ok
main.c:main:23 -> CTEST_ASSERT_EQ(func1(99), func2()) ... FAILED
  lhs = 100
  rhs = 2
main.c:main:24 -> CTEST_ASSERT_NEQ(func1(0), func2()) ... ok
Passed: 3
Failed: 3
Total: 6
===================================
```

### With `CTEST_ABORT_ON_FAIL` defined

*Output*

```
Summary
===================================
main.c:main:18 -> CTEST_ASSERT_TRUE(x != 1) ... FAILED
Passed: 0
Failed: 1
Total: 1
===================================
Aborted (core dumped)
```

# Tagged Functions
You can tag functions with `ctest_tagged_func_create(void (*func)(void), char *name, char *tags);` and run them with `ctest_tagged_funcs_run(char *tag);`

```c
#define CTEST_IMPL
#include "ctest.h"

void helper() {
  CTEST_ASSERT_TRUE(1 == 2);
}

void test1(void) {
  CTEST_ASSERT_TRUE(1 == 1);
  helper();
}

void test2(void) {
  CTEST_ASSERT_EQ(1, 2);
}

void test3(void) {
  CTEST_ASSERT_TRUE(1 == 1);
}

int main(void) {
  CTEST_BEGIN;

  ctest_tagged_func_create(test1, "test 1", "important");
  ctest_tagged_func_create(test2, "test 2", "important");
  ctest_tagged_funcs_run("important");

  CTEST_END;
  return 0;
}
```

*Output*

```
Running test 1 [important]
Running test 2 [important]
Summary
===================================
main.c:test1:9 -> CTEST_ASSERT_TRUE(1 == 1) ... ok
main.c:helper:5 -> CTEST_ASSERT_TRUE(1 == 2) ... FAILED
main.c:test2:14 -> CTEST_ASSERT_EQ(1, 2) ... FAILED
  lhs = 1
  rhs = 2
Passed: 1
Failed: 2
Total: 3
Time: 0.000063
===================================
```
