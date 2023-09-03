#ifndef CTEST_H
#define CTEST_H

#include <aio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PASSED 1
#define FAILED 0

// Put this in the entry point of the file to be tested.
#define CTEST_BEGIN                             \
  do {                                          \
    _init_results();                            \
  } while (0)

// Put this in the exit point of the file to be tested.
#define CTEST_END                               \
  do {                                          \
    _show_results();                            \
  } while (0)

#define _TO_STR(x) #x

#define _PRE_EXPR __FILE__, __func__, __LINE__

// =============== //
//   Assertions    //
// =============== //

// Asserts that the expression is true.
#define CTEST_ASSERT_TRUE(expr)                                         \
  do {                                                                  \
    char *res = (char *)malloc(sizeof(char) * 100);                     \
    if ((expr)) {                                                       \
      sprintf(res, "%s:%s:%d -> %s ... %s", _PRE_EXPR, _TO_STR(CTEST_ASSERT_TRUE(expr)), "ok"); \
      _add_to_results(res, PASSED);                                     \
    } else {                                                            \
      sprintf(res, "%s:%s:%d -> %s ... %s", _PRE_EXPR, _TO_STR(CTEST_ASSERT_TRUE(expr)), "FAILED"); \
      _add_to_results(res, FAILED);                                     \
    }                                                                   \
  } while (0)

// Asserts that the expression is false.
#define CTEST_ASSERT_FALSE(expr)                                        \
  do {                                                                  \
    char *res = (char *)malloc(sizeof(char) * 100);                     \
    if (!(expr)) {                                                      \
      sprintf(res, "%s:%s:%d -> %s ... %s", _PRE_EXPR, _TO_STR(CTEST_ASSERT_FALSE(expr)), "ok"); \
      _add_to_results(res, PASSED);                                     \
    } else {                                                            \
      sprintf(res, "%s:%s:%d -> %s ... %s", _PRE_EXPR, _TO_STR(CTEST_ASSERT_FALSE(expr)), "FAILED"); \
      _add_to_results(res, FAILED);                                     \
    }                                                                   \
  } while (0)

// Asserts that the two expressions are equal.
// Displays the values of the expressions if they are not equal.
#define CTEST_ASSERT_EQ(expr1, expr2)                                   \
  do {                                                                  \
    char *res = (char *)malloc(sizeof(char) * 100);                     \
    if ((expr1) == (expr2)) {                                           \
      sprintf(res, "%s:%s:%d -> %s ... %s", _PRE_EXPR, _TO_STR(CTEST_ASSERT_EQ(expr1, expr2)), "ok"); \
      _add_to_results(res, PASSED);                                     \
    } else {                                                            \
      sprintf(res, "%s:%s:%d -> %s ... %s\n  lhs = %d\n  rhs = %d", _PRE_EXPR, _TO_STR(CTEST_ASSERT_EQ(expr1, expr2)), "FAILED", expr1, expr2); \
      _add_to_results(res, FAILED);                                     \
    }                                                                   \
  } while (0)

// Asserts that the two expressions are not equal.
// Displays the values of the expressions if they are equal.
#define CTEST_ASSERT_NEQ(expr1, expr2)                                  \
  do {                                                                  \
    char *res = (char *)malloc(sizeof(char) * 100);                     \
    if ((expr1) != (expr2)) {                                           \
      sprintf(res, "%s:%s:%d -> %s ... %s", _PRE_EXPR, _TO_STR(CTEST_ASSERT_NEQ(expr1, expr2)), "ok"); \
      _add_to_results(res, PASSED);                                     \
    } else {                                                            \
      sprintf(res, "%s:%s:%d -> %s ... %s\n  lhs = %d\n  rhs = %d", _PRE_EXPR, _TO_STR(CTEST_ASSERT_NEQ(expr1, expr2)), "FAILED", expr1, expr2); \
      _add_to_results(res, FAILED);                                     \
    }                                                                   \
  } while (0)

int ctest_randint(int min, int max);

float ctest_randfloat_clamped();

#ifdef CTEST_IMPL

// =============== //
// Implementation  //
// =============== //

int ctest_randint(int min, int max) {
  return rand() % (max - min + 1) + min;
}

float ctest_randfloat_clamped() {
  return (float)rand() / (float)(RAND_MAX);
}

typedef struct {
  char **exprs;
  size_t len, cap;
  size_t failed;
  size_t passed;
  clock_t start_time, end_time;
  double elapsed_time;
} CTest;

static CTest _result = {0};

void _init_results() {
  _result.exprs = (char **)malloc(sizeof(char *) * 10);
  _result.len = 0;
  _result.cap = 10;
  _result.failed = 0;
  _result.passed = 0;
  _result.end_time;
  _result.elapsed_time;
  _result.start_time = clock();
  srand(time(NULL));
}

void _realloc_exprs() {
  clock_t pause = clock();
  _result.exprs = (char **)realloc(_result.exprs, sizeof(char *) * _result.cap * 2);
  _result.cap *= 2;
  _result.start_time -= pause;
}

void _show_results() {
  _result.end_time = clock();
  _result.elapsed_time = (double)(_result.end_time - _result.start_time) / CLOCKS_PER_SEC;
  printf("========== CTest Results ==========\n");
  for (size_t i = 0; i < _result.len; i++) {
    printf("%s\n", _result.exprs[i]);
  }
  printf("Passed: %zu\n", _result.passed);
  printf("Failed: %zu\n", _result.failed);
  printf("Total: %zu\n", _result.len);
  printf("Time: %f\n", _result.elapsed_time);
  printf("===================================\n");

  for (size_t i = 0; i < _result.len; i++) {
    free(_result.exprs[i]);
  }
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
#ifdef CTEST_ABORT_ON_FAIL
    _show_results();
    abort();
#endif // CTEST_ABORT_ON_FAIL
  }
}

#endif // CTEST_IMPL
#endif // CTEST_H