//
// literals.hpp
// ~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STDNET_LITERALS_HPP
#define STDNET_LITERALS_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "std/net/ip/address.hpp"
#include "std/net/ip/address_v4.hpp"
#include "std/net/ip/address_v6.hpp"

#include "std/net/detail/push_options.hpp"

namespace std {
namespace experimental {
inline namespace literals {
inline namespace net_literals {

inline net::ip::address operator"" _ip(const char* str, std::size_t)
{
  return net::ip::make_address(str);
}

inline net::ip::address_v4 operator"" _ipv4(const char* str, std::size_t)
{
  return net::ip::make_address_v4(str);
}

inline net::ip::address_v6 operator"" _ipv6(const char* str, std::size_t)
{
  return net::ip::make_address_v6(str);
}

} // inline namespace net_literals
} // inline namespace literals
} // namespace experimental
} // namespace std

#include "std/net/detail/pop_options.hpp"

#endif // STDNET_LITERALS_HPP
