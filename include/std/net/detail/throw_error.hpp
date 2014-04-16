//
// detail/throw_error.hpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STDNET_DETAIL_THROW_ERROR_HPP
#define STDNET_DETAIL_THROW_ERROR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "std/net/detail/config.hpp"
#include <system_error>

#include "std/net/detail/push_options.hpp"

namespace std {
namespace experimental {
namespace net {
namespace detail {

STDNET_DECL void do_throw_error(const std::error_code& err);

STDNET_DECL void do_throw_error(const std::error_code& err,
    const char* location);

inline void throw_error(const std::error_code& err)
{
  if (err)
    do_throw_error(err);
}

inline void throw_error(const std::error_code& err,
    const char* location)
{
  if (err)
    do_throw_error(err, location);
}

} // namespace detail
} // namespace net
} // namespace experimental
} // namespace std

#include "std/net/detail/pop_options.hpp"

#if defined(STDNET_HEADER_ONLY)
# include "std/net/detail/impl/throw_error.ipp"
#endif // defined(STDNET_HEADER_ONLY)

#endif // STDNET_DETAIL_THROW_ERROR_HPP
