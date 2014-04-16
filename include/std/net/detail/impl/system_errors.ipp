//
// impl/system_errors.ipp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STDNET_DETAIL_IMPL_SYSTEM_ERRORS_IPP
#define STDNET_DETAIL_IMPL_SYSTEM_ERRORS_IPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "std/net/detail/config.hpp"
#include "std/net/detail/local_free_on_block_exit.hpp"
#include "std/net/detail/system_errors.hpp"

#include "std/net/detail/push_options.hpp"

namespace std {
namespace experimental {
namespace net {
namespace detail {
namespace syserrc {

class system_category : public std::error_category
{
public:
  const char* name() const STDNET_ERROR_CATEGORY_NOEXCEPT
  {
    return "std.net.system";
  }

  std::string message(int value) const
  {
#if defined(STDNET_WINDOWS) || defined(__CYGWIN__)
    char* msg = 0;
    DWORD length = ::FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER
        | FORMAT_MESSAGE_FROM_SYSTEM
        | FORMAT_MESSAGE_IGNORE_INSERTS, 0, value,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (char*)&msg, 0, 0);
    detail::local_free_on_block_exit local_free_obj(msg);
    if (length && msg[length - 1] == '\n')
      msg[--length] = '\0';
    if (length && msg[length - 1] == '\r')
      msg[--length] = '\0';
    if (length)
      return msg;
    else
      return "std.net.system error";
#else // defined(STDNET_WINDOWS)
#if !defined(__sun)
    if (value == ECANCELED)
      return "Operation aborted.";
#endif // !defined(__sun)
#if defined(__sun) || defined(__QNX__) || defined(__SYMBIAN32__)
    using namespace std;
    return strerror(value);
#elif defined(__MACH__) && defined(__APPLE__) \
  || defined(__NetBSD__) || defined(__FreeBSD__) || defined(__OpenBSD__) \
  || defined(_AIX) || defined(__hpux) || defined(__osf__) \
  || defined(__ANDROID__)
    char buf[256] = "";
    using namespace std;
    strerror_r(value, buf, sizeof(buf));
    return buf;
#else
    char buf[256] = "";
    return strerror_r(value, buf, sizeof(buf));
#endif
#endif // defined(STDNET_WINDOWS)
  }
};

} // namespace syserrc

const std::error_category& system_category()
{
  static syserrc::system_category instance;
  return instance;
}

} // namespace detail
} // namespace net
} // namespace experimental
} // namespace std

#include "std/net/detail/pop_options.hpp"

#endif // STDNET_DETAIL_IMPL_SYSTEM_ERRORS_IPP
