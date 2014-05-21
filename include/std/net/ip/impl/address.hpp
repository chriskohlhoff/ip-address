//
// ip/impl/address.hpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STDNET_IP_IMPL_ADDRESS_HPP
#define STDNET_IP_IMPL_ADDRESS_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "std/net/detail/throw_error.hpp"
#include "std/net/ip/address_cast.hpp"

#include "std/net/detail/push_options.hpp"

namespace std {
namespace experimental {
namespace net {
namespace ip {

template <typename T, typename>
inline STDNET_CONSTEXPR address::address(const T& t) STDNET_NOEXCEPT
  : address(address_cast<address>(t))
{
}

#if defined(STDNET_HAS_VARIADIC_TEMPLATES)

template <typename... T, typename>
inline STDNET_CONSTEXPR address::address(T&&... t)
  : address(make_address(static_cast<T&&>(t)...))
{
}

#endif // defined(STDNET_HAS_VARIADIC_TEMPLATES)

#if !defined(STDNET_NO_IOSTREAM)

template <typename Elem, typename Traits>
std::basic_ostream<Elem, Traits>& operator<<(
    std::basic_ostream<Elem, Traits>& os, const address& addr)
{
  std::error_code ec;
  std::string s = addr.to_string(ec);
  if (ec)
  {
    if (os.exceptions() & std::basic_ostream<Elem, Traits>::failbit)
      std::experimental::net::detail::throw_error(ec);
    else
      os.setstate(std::basic_ostream<Elem, Traits>::failbit);
  }
  else
    for (std::string::iterator i = s.begin(); i != s.end(); ++i)
      os << os.widen(*i);
  return os;
}

#endif // !defined(STDNET_NO_IOSTREAM)

} // namespace ip
} // namespace net
} // namespace experimental
} // namespace std

#include "std/net/detail/pop_options.hpp"

#endif // STDNET_IP_IMPL_ADDRESS_HPP
