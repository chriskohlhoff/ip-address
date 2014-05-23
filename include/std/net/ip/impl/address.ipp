//
// ip/impl/address.ipp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STDNET_IP_IMPL_ADDRESS_IPP
#define STDNET_IP_IMPL_ADDRESS_IPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "std/net/detail/config.hpp"
#include <system_error>
#include <typeinfo>
#include "std/net/detail/throw_error.hpp"
#include "std/net/detail/throw_exception.hpp"
#include "std/net/detail/system_errors.hpp"
#include "std/net/ip/address.hpp"

#include "std/net/detail/push_options.hpp"

namespace std {
namespace experimental {
namespace net {
namespace ip {

std::string address::to_string() const
{
  if (type_ == ipv4)
    return ipv4_address_.to_string();
  if (type_ == ipv6)
    return ipv6_address_.to_string();
  throw bad_address_cast();
}

std::string address::to_string(std::error_code& ec) const
{
  if (type_ == ipv4)
    return ipv4_address_.to_string(ec);
  if (type_ == ipv6)
    return ipv6_address_.to_string(ec);
  throw bad_address_cast();
}

address make_address(const char* str)
{
  std::error_code ec;
  address addr = make_address(str, ec);
  std::experimental::net::detail::throw_error(ec);
  return addr;
}

address make_address(const char* str,
    std::error_code& ec) STDNET_NOEXCEPT
{
  address_v6 ipv6_address = make_address_v6(str, ec);
  if (!ec)
    return ipv6_address;

  address_v4 ipv4_address = make_address_v4(str, ec);
  if (!ec)
    return ipv4_address;

  return address();
}

address make_address(const std::string& str)
{
  return make_address(str.c_str());
}

address make_address(const std::string& str,
    std::error_code& ec) STDNET_NOEXCEPT
{
  return make_address(str.c_str(), ec);
}

} // namespace ip
} // namespace net
} // namespace experimental
} // namespace std

#include "std/net/detail/pop_options.hpp"

#endif // STDNET_IP_IMPL_ADDRESS_IPP
