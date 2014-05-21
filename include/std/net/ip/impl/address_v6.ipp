//
// ip/impl/address_v6.ipp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STDNET_IP_IMPL_ADDRESS_V6_IPP
#define STDNET_IP_IMPL_ADDRESS_V6_IPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "std/net/detail/config.hpp"
#include "std/net/detail/socket_ops.hpp"
#include "std/net/detail/system_errors.hpp"
#include "std/net/detail/throw_error.hpp"
#include "std/net/detail/throw_exception.hpp"
#include "std/net/ip/address_v6.hpp"

#include "std/net/detail/push_options.hpp"

namespace std {
namespace experimental {
namespace net {
namespace ip {

std::string address_v6::to_string() const
{
  std::error_code ec;
  std::string addr = to_string(ec);
  std::experimental::net::detail::throw_error(ec);
  return addr;
}

std::string address_v6::to_string(std::error_code& ec) const
{
  char addr_str[std::experimental::net::detail::max_addr_v6_str_len];
  const char* addr =
    std::experimental::net::detail::socket_ops::inet_ntop(
        AF_INET6, bytes_.data(), addr_str,
        std::experimental::net::detail::max_addr_v6_str_len,
        scope_id_, ec);
  if (addr == 0)
    return std::string();
  return addr;
}

address_v6 make_address_v6(const char* str)
{
  std::error_code ec;
  address_v6 addr = make_address_v6(str, ec);
  std::experimental::net::detail::throw_error(ec);
  return addr;
}

address_v6 make_address_v6(const char* str,
    std::error_code& ec) STDNET_NOEXCEPT
{
  address_v6::bytes_type bytes;
  unsigned long scope_id = 0;
  if (std::experimental::net::detail::socket_ops::inet_pton(
        AF_INET6, str, bytes.data(), &scope_id, ec) <= 0)
    return address_v6();
  return address_v6(bytes, scope_id);
}

address_v6 make_address_v6(const std::string& str)
{
  return make_address_v6(str.c_str());
}

address_v6 make_address_v6(const std::string& str,
    std::error_code& ec) STDNET_NOEXCEPT
{
  return make_address_v6(str.c_str(), ec);
}

} // namespace ip
} // namespace net
} // namespace experimental
} // namespace std

#include "std/net/detail/pop_options.hpp"

#endif // STDNET_IP_IMPL_ADDRESS_V6_IPP
