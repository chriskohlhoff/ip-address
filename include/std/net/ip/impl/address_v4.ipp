//
// ip/impl/address_v4.ipp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STDNET_IP_IMPL_ADDRESS_V4_IPP
#define STDNET_IP_IMPL_ADDRESS_V4_IPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "std/net/detail/config.hpp"
#include <climits>
#include <stdexcept>
#include "std/net/detail/socket_ops.hpp"
#include "std/net/detail/system_errors.hpp"
#include "std/net/detail/throw_error.hpp"
#include "std/net/detail/throw_exception.hpp"
#include "std/net/ip/address_v4.hpp"

#include "std/net/detail/push_options.hpp"

namespace std {
namespace experimental {
namespace net {
namespace ip {

address_v4::bytes_type address_v4::to_bytes() const STDNET_NOEXCEPT
{
  using namespace std; // For memcpy.
  bytes_type bytes;
  memcpy(bytes.data(), &addr_.s_addr, 4);
  return bytes;
}

unsigned long address_v4::to_ulong() const STDNET_NOEXCEPT
{
  return std::experimental::net::detail::socket_ops::network_to_host_long(addr_.s_addr);
}

std::string address_v4::to_string() const
{
  std::error_code ec;
  std::string addr = to_string(ec);
  std::experimental::net::detail::throw_error(ec);
  return addr;
}

std::string address_v4::to_string(std::error_code& ec) const
{
  char addr_str[std::experimental::net::detail::max_addr_v4_str_len];
  const char* addr =
    std::experimental::net::detail::socket_ops::inet_ntop(AF_INET, &addr_, addr_str,
        std::experimental::net::detail::max_addr_v4_str_len, 0, ec);
  if (addr == 0)
    return std::string();
  return addr;
}

bool address_v4::is_loopback() const STDNET_NOEXCEPT
{
  return (to_ulong() & 0xFF000000) == 0x7F000000;
}

bool address_v4::is_unspecified() const STDNET_NOEXCEPT
{
  return to_ulong() == 0;
}

bool address_v4::is_class_a() const STDNET_NOEXCEPT
{
  return (to_ulong() & 0x80000000) == 0;
}

bool address_v4::is_class_b() const STDNET_NOEXCEPT
{
  return (to_ulong() & 0xC0000000) == 0x80000000;
}

bool address_v4::is_class_c() const STDNET_NOEXCEPT
{
  return (to_ulong() & 0xE0000000) == 0xC0000000;
}

bool address_v4::is_multicast() const STDNET_NOEXCEPT
{
  return (to_ulong() & 0xF0000000) == 0xE0000000;
}

address_v4 address_v4::broadcast(const address_v4& addr,
    const address_v4& mask) STDNET_NOEXCEPT
{
  return address_v4(addr.to_ulong() | (mask.to_ulong() ^ 0xFFFFFFFF));
}

address_v4 address_v4::netmask(const address_v4& addr) STDNET_NOEXCEPT
{
  if (addr.is_class_a())
    return address_v4(0xFF000000);
  if (addr.is_class_b())
    return address_v4(0xFFFF0000);
  if (addr.is_class_c())
    return address_v4(0xFFFFFF00);
  return address_v4(0xFFFFFFFF);
}

address_v4 make_address_v4(const std::array<unsigned char, 4>& bytes)
{
#if UCHAR_MAX > 0xFF
  if (bytes[0] > 0xFF || bytes[1] > 0xFF
      || bytes[2] > 0xFF || bytes[3] > 0xFF)
  {
    std::out_of_range ex("address_v4 from bytes_type");
    std::experimental::net::detail::throw_exception(ex);
  }
#endif // UCHAR_MAX > 0xFF

  using namespace std; // For memcpy.
  address_v4 tmp;
  memcpy(&tmp.addr_.s_addr, bytes.data(), 4);
  return tmp;
}

address_v4 make_address_v4(unsigned long addr)
{
#if ULONG_MAX > 0xFFFFFFFF
  if (addr > 0xFFFFFFFF)
  {
    std::out_of_range ex("address_v4 from unsigned long");
    std::experimental::net::detail::throw_exception(ex);
  }
#endif // ULONG_MAX > 0xFFFFFFFF

  address_v4 tmp;
  tmp.addr_.s_addr =
    std::experimental::net::detail::socket_ops::host_to_network_long(
      static_cast<std::experimental::net::detail::u_long_type>(addr));
  return tmp;
}

address_v4 make_address_v4(const char* str)
{
  std::error_code ec;
  address_v4 addr = make_address_v4(str, ec);
  std::experimental::net::detail::throw_error(ec);
  return addr;
}

address_v4 make_address_v4(const char* str,
    std::error_code& ec) STDNET_NOEXCEPT
{
  address_v4::bytes_type bytes;
  if (std::experimental::net::detail::socket_ops::inet_pton(
        AF_INET, str, bytes.data(), 0, ec) <= 0)
    return address_v4();
  return make_address_v4(bytes);
}

address_v4 make_address_v4(const std::string& str)
{
  return make_address_v4(str.c_str());
}

address_v4 make_address_v4(const std::string& str,
    std::error_code& ec) STDNET_NOEXCEPT
{
  return make_address_v4(str.c_str(), ec);
}

} // namespace ip
} // namespace net
} // namespace experimental
} // namespace std

#include "std/net/detail/pop_options.hpp"

#endif // STDNET_IP_IMPL_ADDRESS_V4_IPP
