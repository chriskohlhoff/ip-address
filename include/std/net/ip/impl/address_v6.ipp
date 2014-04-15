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
#include <cstring>
#include <stdexcept>
#include <typeinfo>
#include "std/net/detail/socket_ops.hpp"
#include "std/net/detail/system_errors.hpp"
#include "std/net/detail/throw_error.hpp"
#include "std/net/detail/throw_exception.hpp"
#include "std/net/ip/address_v4.hpp"
#include "std/net/ip/address_v6.hpp"

#include "std/net/detail/push_options.hpp"

namespace std {
namespace net {
namespace ip {

address_v6::address_v6() STDNET_NOEXCEPT
  : addr_(),
    scope_id_(0)
{
}

address_v6::address_v6(const address_v6& other) STDNET_NOEXCEPT
  : addr_(other.addr_),
    scope_id_(other.scope_id_)
{
}

#if defined(STDNET_HAS_MOVE)
address_v6::address_v6(address_v6&& other) STDNET_NOEXCEPT
  : addr_(other.addr_),
    scope_id_(other.scope_id_)
{
}
#endif // defined(STDNET_HAS_MOVE)

address_v6& address_v6::operator=(const address_v6& other) STDNET_NOEXCEPT
{
  addr_ = other.addr_;
  scope_id_ = other.scope_id_;
  return *this;
}

#if defined(STDNET_HAS_MOVE)
address_v6& address_v6::operator=(address_v6&& other) STDNET_NOEXCEPT
{
  addr_ = other.addr_;
  scope_id_ = other.scope_id_;
  return *this;
}
#endif // defined(STDNET_HAS_MOVE)

address_v6::bytes_type address_v6::to_bytes() const STDNET_NOEXCEPT
{
  using namespace std; // For memcpy.
  bytes_type bytes;
  memcpy(bytes.data(), addr_.s6_addr, 16);
  return bytes;
}

std::string address_v6::to_string() const
{
  std::error_code ec;
  std::string addr = to_string(ec);
  std::net::detail::throw_error(ec);
  return addr;
}

std::string address_v6::to_string(std::error_code& ec) const
{
  char addr_str[std::net::detail::max_addr_v6_str_len];
  const char* addr =
    std::net::detail::socket_ops::inet_ntop(AF_INET6, &addr_, addr_str,
        std::net::detail::max_addr_v6_str_len, scope_id_, ec);
  if (addr == 0)
    return std::string();
  return addr;
}

address_v4 address_v6::to_v4() const
{
  if (!is_v4_mapped() && !is_v4_compatible())
  {
    std::bad_cast ex;
    std::net::detail::throw_exception(ex);
  }

  address_v4::bytes_type v4_bytes = { { addr_.s6_addr[12],
    addr_.s6_addr[13], addr_.s6_addr[14], addr_.s6_addr[15] } };
  return address_v4(v4_bytes);
}

bool address_v6::is_loopback() const STDNET_NOEXCEPT
{
  return ((addr_.s6_addr[0] == 0) && (addr_.s6_addr[1] == 0)
      && (addr_.s6_addr[2] == 0) && (addr_.s6_addr[3] == 0)
      && (addr_.s6_addr[4] == 0) && (addr_.s6_addr[5] == 0)
      && (addr_.s6_addr[6] == 0) && (addr_.s6_addr[7] == 0)
      && (addr_.s6_addr[8] == 0) && (addr_.s6_addr[9] == 0)
      && (addr_.s6_addr[10] == 0) && (addr_.s6_addr[11] == 0)
      && (addr_.s6_addr[12] == 0) && (addr_.s6_addr[13] == 0)
      && (addr_.s6_addr[14] == 0) && (addr_.s6_addr[15] == 1));
}

bool address_v6::is_unspecified() const STDNET_NOEXCEPT
{
  return ((addr_.s6_addr[0] == 0) && (addr_.s6_addr[1] == 0)
      && (addr_.s6_addr[2] == 0) && (addr_.s6_addr[3] == 0)
      && (addr_.s6_addr[4] == 0) && (addr_.s6_addr[5] == 0)
      && (addr_.s6_addr[6] == 0) && (addr_.s6_addr[7] == 0)
      && (addr_.s6_addr[8] == 0) && (addr_.s6_addr[9] == 0)
      && (addr_.s6_addr[10] == 0) && (addr_.s6_addr[11] == 0)
      && (addr_.s6_addr[12] == 0) && (addr_.s6_addr[13] == 0)
      && (addr_.s6_addr[14] == 0) && (addr_.s6_addr[15] == 0));
}

bool address_v6::is_link_local() const STDNET_NOEXCEPT
{
  return ((addr_.s6_addr[0] == 0xfe) && ((addr_.s6_addr[1] & 0xc0) == 0x80));
}

bool address_v6::is_site_local() const STDNET_NOEXCEPT
{
  return ((addr_.s6_addr[0] == 0xfe) && ((addr_.s6_addr[1] & 0xc0) == 0xc0));
}

bool address_v6::is_v4_mapped() const STDNET_NOEXCEPT
{
  return ((addr_.s6_addr[0] == 0) && (addr_.s6_addr[1] == 0)
      && (addr_.s6_addr[2] == 0) && (addr_.s6_addr[3] == 0)
      && (addr_.s6_addr[4] == 0) && (addr_.s6_addr[5] == 0)
      && (addr_.s6_addr[6] == 0) && (addr_.s6_addr[7] == 0)
      && (addr_.s6_addr[8] == 0) && (addr_.s6_addr[9] == 0)
      && (addr_.s6_addr[10] == 0xff) && (addr_.s6_addr[11] == 0xff));
}

bool address_v6::is_v4_compatible() const STDNET_NOEXCEPT
{
  return ((addr_.s6_addr[0] == 0) && (addr_.s6_addr[1] == 0)
      && (addr_.s6_addr[2] == 0) && (addr_.s6_addr[3] == 0)
      && (addr_.s6_addr[4] == 0) && (addr_.s6_addr[5] == 0)
      && (addr_.s6_addr[6] == 0) && (addr_.s6_addr[7] == 0)
      && (addr_.s6_addr[8] == 0) && (addr_.s6_addr[9] == 0)
      && (addr_.s6_addr[10] == 0) && (addr_.s6_addr[11] == 0)
      && !((addr_.s6_addr[12] == 0)
        && (addr_.s6_addr[13] == 0)
        && (addr_.s6_addr[14] == 0)
        && ((addr_.s6_addr[15] == 0) || (addr_.s6_addr[15] == 1))));
}

bool address_v6::is_multicast() const STDNET_NOEXCEPT
{
  return (addr_.s6_addr[0] == 0xff);
}

bool address_v6::is_multicast_global() const STDNET_NOEXCEPT
{
  return ((addr_.s6_addr[0] == 0xff) && ((addr_.s6_addr[1] & 0x0f) == 0x0e));
}

bool address_v6::is_multicast_link_local() const STDNET_NOEXCEPT
{
  return ((addr_.s6_addr[0] == 0xff) && ((addr_.s6_addr[1] & 0x0f) == 0x02));
}

bool address_v6::is_multicast_node_local() const STDNET_NOEXCEPT
{
  return ((addr_.s6_addr[0] == 0xff) && ((addr_.s6_addr[1] & 0x0f) == 0x01));
}

bool address_v6::is_multicast_org_local() const STDNET_NOEXCEPT
{
  return ((addr_.s6_addr[0] == 0xff) && ((addr_.s6_addr[1] & 0x0f) == 0x08));
}

bool address_v6::is_multicast_site_local() const STDNET_NOEXCEPT
{
  return ((addr_.s6_addr[0] == 0xff) && ((addr_.s6_addr[1] & 0x0f) == 0x05));
}

bool operator==(const address_v6& a1, const address_v6& a2) STDNET_NOEXCEPT
{
  using namespace std; // For memcmp.
  return memcmp(&a1.addr_, &a2.addr_,
      sizeof(std::net::detail::in6_addr_type)) == 0
    && a1.scope_id_ == a2.scope_id_;
}

bool operator<(const address_v6& a1, const address_v6& a2) STDNET_NOEXCEPT
{
  using namespace std; // For memcmp.
  int memcmp_result = memcmp(&a1.addr_, &a2.addr_,
      sizeof(std::net::detail::in6_addr_type));
  if (memcmp_result < 0)
    return true;
  if (memcmp_result > 0)
    return false;
  return a1.scope_id_ < a2.scope_id_;
}

address_v6 address_v6::loopback() STDNET_NOEXCEPT
{
  address_v6 tmp;
  tmp.addr_.s6_addr[15] = 1;
  return tmp;
}

address_v6 address_v6::v4_mapped(const address_v4& addr) STDNET_NOEXCEPT
{
  address_v4::bytes_type v4_bytes = addr.to_bytes();
  bytes_type v6_bytes = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xFF, 0xFF,
    v4_bytes[0], v4_bytes[1], v4_bytes[2], v4_bytes[3] } };
  return make_address_v6(v6_bytes);
}

address_v6 address_v6::v4_compatible(const address_v4& addr) STDNET_NOEXCEPT
{
  address_v4::bytes_type v4_bytes = addr.to_bytes();
  bytes_type v6_bytes = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    v4_bytes[0], v4_bytes[1], v4_bytes[2], v4_bytes[3] } };
  return make_address_v6(v6_bytes);
}

address_v6 make_address_v6(const std::array<unsigned char, 16>& bytes,
    unsigned long scope)
{
#if UCHAR_MAX > 0xFF
  for (std::size_t i = 0; i < bytes.size(); ++i)
  {
    if (bytes[i] > 0xFF)
    {
      std::out_of_range ex("address_v6 from bytes_type");
      std::net::detail::throw_exception(ex);
    }
  }
#endif // UCHAR_MAX > 0xFF

  address_v6 tmp;
  using namespace std; // For memcpy.
  memcpy(tmp.addr_.s6_addr, bytes.data(), 16);
  tmp.scope_id_ = scope;
  return tmp;
}

address_v6 make_address_v6(const char* str)
{
  std::error_code ec;
  address_v6 addr = make_address_v6(str, ec);
  std::net::detail::throw_error(ec);
  return addr;
}

address_v6 make_address_v6(const char* str,
    std::error_code& ec) STDNET_NOEXCEPT
{
  address_v6::bytes_type bytes;
  unsigned long scope_id = 0;
  if (std::net::detail::socket_ops::inet_pton(
        AF_INET6, str, bytes.data(), &scope_id, ec) <= 0)
    return address_v6();
  return make_address_v6(bytes, scope_id);
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
} // namespace std

#include "std/net/detail/pop_options.hpp"

#endif // STDNET_IP_IMPL_ADDRESS_V6_IPP
