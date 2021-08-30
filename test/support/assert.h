/** \file */

#ifndef INCLUDE_ASSERT_H
#define INCLUDE_ASSERT_H

#include "CException.h"

#define assert(_condition) if( !(_condition) ) Throw(0)

#define TEST_ASSERT_FAIL_ASSERT(_code_under_test)             \
{                                                             \
    CEXCEPTION_T _exception;                                  \
    Try                                                       \
    {                                                         \
        _code_under_test;                                     \
        TEST_FAIL_MESSAGE("Code under test did not assert."); \
    }                                                         \
    Catch(_exception) {}                                      \
}

#define TEST_ASSERT_PASS_ASSERT(_code_under_test)                  \
{                                                                  \
    CEXCEPTION_T _exception;                                       \
    Try                                                            \
    {                                                              \
        _code_under_test;                                          \
    }                                                              \
    Catch(_exception)                                              \
    {                                                              \
        TEST_FAIL_MESSAGE("Code under test failed an assertion."); \
    }                                                              \
}

#endif /* INCLUDE_ASSERT_H */