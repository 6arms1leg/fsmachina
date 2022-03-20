/** \file */

#ifndef ASSERT_H
#define ASSERT_H

#include <stdbool.h>

#include "CException.h" /* Exception handling lib. interface */

/* `assert` replacement for unit testing */
#define assert(condition_) if (false == (condition_)) Throw(0)

/* Unit testing assertion that `assert` is triggered */
#define TEST_ASSERT_FAIL_ASSERT(codeUnderTest_)               \
do {                                                          \
    CEXCEPTION_T exception_;                                  \
    Try {                                                     \
        codeUnderTest_;                                       \
        TEST_FAIL_MESSAGE("Code under test did not assert."); \
    }                                                         \
    Catch(exception_) {;}                                     \
} while (false)

/* Unit testing assertion that `assert` is *not* triggered */
#define TEST_ASSERT_PASS_ASSERT(codeUnderTest_)                    \
do {                                                               \
    CEXCEPTION_T exception_;                                       \
    Try {                                                          \
        codeUnderTest_;                                            \
    }                                                              \
    Catch(exception_) {                                            \
        TEST_FAIL_MESSAGE("Code under test failed an assertion."); \
    }                                                              \
} while (false)

#endif /* ASSERT_H */
