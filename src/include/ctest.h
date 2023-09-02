#ifndef CTEST_INCLUDED
#define CTEST_INCLUDED

#include <aio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void _show_results();
void _add_to_results(char *expr, int passed);

#define PASSED 1
#define FAILED 0

#define START_CTEST \
  do { \
    _init_results(); \
  } while (0)

#define END_CTEST \
  do { \
    _show_results(); \
  } while (0)

#define _CTEST_STR(x) #x

// func: file:line: MACRO_NAME -> expr
#define ASSERT_TRUE(expr) \
  do { \
    char *res = (char *)malloc(sizeof(char) * 100); \
    if ((expr)) { \
      sprintf(res, "%s:%s:%d: %s -> %s", __FILE__, __func__, __LINE__, _CTEST_STR(ASSERT_TRUE(expr)), "PASSED"); \
      _add_to_results(res, PASSED); \
    } else { \
      sprintf(res, "%s:%s:%d: %s -> %s", __FILE__, __func__, __LINE__, _CTEST_STR(ASSERT_TRUE(expr)), "FAILED"); \
      _add_to_results(res, FAILED); \
    } \
  } while (0)

#define ASSERT_FALSE(expr) \
  do { \
    char *res = (char *)malloc(sizeof(char) * 100); \
    if (!(expr)) { \
      sprintf(res, "%s:%s:%d: %s -> %s", __FILE__, __func__, __LINE__, _CTEST_STR(ASSERT_FALSE(expr)), "PASSED"); \
      _add_to_results(res, PASSED); \
    } else { \
      sprintf(res, "%s:%s:%d: %s -> %s", __FILE__, __func__, __LINE__, _CTEST_STR(ASSERT_FALSE(expr)), "FAILED"); \
      _add_to_results(res, FAILED); \
    } \
  } while (0)

#define ASSERT_EQ(expr1, expr2) \
  do { \
    char *res = (char *)malloc(sizeof(char) * 100); \
    if ((expr1) == (expr2)) { \
      sprintf(res, "%s:%s:%d: %s -> %s", __FILE__, __func__, __LINE__, _CTEST_STR(ASSERT_EQ(expr1, expr2)), "PASSED"); \
      _add_to_results(res, PASSED); \
    } else { \
      sprintf(res, "%s:%s:%d: %s -> %s", __FILE__, __func__, __LINE__, _CTEST_STR(ASSERT_EQ(expr1, expr2)), "FAILED"); \
      _add_to_results(res, FAILED); \
    } \
  } while (0)

#define ASSERT_NEQ(expr1, expr2) \
  do { \
    char *res = (char *)malloc(sizeof(char) * 100); \
    if ((expr1) != (expr2)) { \
      sprintf(res, "%s:%s:%d: %s -> %s", __FILE__, __func__, __LINE__, _CTEST_STR(ASSERT_NEQ(expr1, expr2)), "PASSED"); \
      _add_to_results(res, PASSED); \
    } else { \
      sprintf(res, "%s:%s:%d: %s -> %s", __FILE__, __func__, __LINE__, _CTEST_STR(ASSERT_NEQ(expr1, expr2)), "FAILED"); \
      _add_to_results(res, FAILED); \
    } \
  } while (0)

#endif // CTEST_INCLUDED

#ifdef CTEST_IMPL

typedef struct {
  char **exprs;
  size_t len, cap;
  size_t failed;
  size_t passed;
} CTest;

static CTest _result = {0};

void _init_results() {
  _result.exprs = (char **)malloc(sizeof(char *) * 10);
  _result.len = 0;
  _result.cap = 10;
  _result.failed = 0;
  _result.passed = 0;
}

void _realloc_exprs() {
  _result.exprs = (char **)realloc(_result.exprs, sizeof(char *) * _result.cap * 2);
  _result.cap *= 2;
}

void _add_to_results(char *expr, int passed) {
  if (_result.len == _result.cap) {
    _realloc_exprs();
  }
  _result.exprs[_result.len++] = expr;
  if (passed) {
    _result.passed++;
  } else {
    _result.failed++;
  }
}

void _show_results() {
  printf("========== CTest Results ==========\n");
  for (size_t i = 0; i < _result.len; i++) {
    printf("%s\n", _result.exprs[i]);
  }
  printf("Passed: %zu\n", _result.passed);
  printf("Failed: %zu\n", _result.failed);
  printf("Total: %zu\n", _result.len);
  printf("===================================\n");

  for (size_t i = 0; i < _result.len; i++) {
    free(_result.exprs[i]);
  }

  if (_result.failed > 0) {
    abort();
  }
}

#endif // CTEST_IMPL
