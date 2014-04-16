//
// detail/impl/throw_error.ipp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STDNET_DETAIL_IMPL_THROW_ERROR_IPP
#define STDNET_DETAIL_IMPL_THROW_ERROR_IPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "std/net/detail/config.hpp"
#include <system_error>
#include "std/net/detail/throw_error.hpp"
#include "std/net/detail/throw_exception.hpp"

#include "std/net/detail/push_options.hpp"

namespace std {
namespace experimental {
namespace net {
namespace detail {

void do_throw_error(const std::error_code& err)
{
  std::system_error e(err);
  std::experimental::net::detail::throw_exception(e);
}

void do_throw_error(const std::error_code& err, const char* location)
{
  std::system_error e(err, location);
  std::experimental::net::detail::throw_exception(e);
}

} // namespace detail
} // namespace net
} // namespace experimental
} // namespace std

#include "std/net/detail/pop_options.hpp"

#endif // STDNET_DETAIL_IMPL_THROW_ERROR_IPP
