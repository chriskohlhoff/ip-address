//
// unit_test.hpp
// ~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef UNIT_TEST_HPP
#define UNIT_TEST_HPP

#include "asio/detail/config.hpp"

#if defined(__sun)
# include <stdlib.h> // Needed for lrand48.
#endif // defined(__sun)

#if defined(__BORLANDC__)

// Prevent use of intrinsic for strcmp.
# include <cstring>
# undef strcmp
 
// Suppress error about condition always being true.
# pragma option -w-ccc

#endif // defined(__BORLANDC__)

#if defined(STDNET_MSVC)
# pragma warning (push)
# pragma warning (disable:4244)
# pragma warning (disable:4702)
#endif // defined(STDNET_MSVC)

#include <cassert>
#include <iostream>

#if defined(NDEBUG)
# error NDEBUG must not be defined when building these unit tests
#endif // defined(NDEBUG)

#define STDNET_CHECK(expr) assert(expr)

#define STDNET_CHECK_MESSAGE(expr, message) \
  do { if (!(expr)) { std::cout << message << std::endl; assert(expr); } } while (0)

#define STDNET_WARN_MESSAGE(expr, message) \
  do { if (!(expr)) { std::cout << message << std::endl; } } while (0)

#define STDNET_ERROR(message) assert(0 && message)

#define STDNET_TEST_SUITE(name, tests) \
  int main() \
  { \
    std::cout << name << " test suite begins" << std::endl; \
    tests \
    std::cout << name << " test suite ends" << std::endl; \
    return 0; \
  }

#define STDNET_TEST_CASE(test) \
  test(); \
  std::cout << #test << " passed" << std::endl;

#if defined(STDNET_MSVC)
# pragma warning (pop)
#endif // defined(STDNET_MSVC)

inline void null_test()
{
}

#endif // UNIT_TEST_HPP
