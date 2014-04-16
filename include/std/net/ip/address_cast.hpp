//
// ip/address_cast.hpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STDNET_IP_ADDRESS_CAST_HPP
#define STDNET_IP_ADDRESS_CAST_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "std/net/detail/config.hpp"
#include <typeinfo>
#include "std/net/detail/throw_exception.hpp"
#include "std/net/ip/fwd.hpp"
#include "std/net/ip/address.hpp"
#include "std/net/ip/address_v4.hpp"
#include "std/net/ip/address_v6.hpp"

#include "std/net/detail/push_options.hpp"

namespace std {
namespace experimental {
namespace net {
namespace ip {

/// Exception thrown on a failed address_cast.
class bad_address_cast
  : public bad_cast
{
public:
  virtual const char* what() const STDNET_NOEXCEPT
  {
    return "bad address cast";
  }
};

/// Cast a version-independent address to itself.
template <class T>
inline T address_cast(const address& addr,
    typename enable_if<is_same<T, address>::value>::type*) STDNET_NOEXCEPT
{
  return addr;
}

/// Cast a version-independent address to an IPv4 address.
/**
 * @throws bad_addrss_cast if @c a does not represent an IPv4 address.
 */
template <class T>
inline T address_cast(const address& addr,
    typename enable_if<is_same<T, address_v4>::value>::type*)
{
  if (addr.type_ != address::ipv4)
  {
    bad_address_cast ex;
    std::experimental::net::detail::throw_exception(ex);
  }

  return addr.ipv4_address_;
}

/// Cast a version-independent address to an IPv6 address.
/**
 * @throws bad_addrss_cast if @c a does not represent an IPv6 address.
 */
template <class T>
inline T address_cast(const address& addr,
    typename enable_if<is_same<T, address_v6>::value>::type*)
{
  if (addr.type_ != address::ipv6)
  {
    bad_address_cast ex;
    std::experimental::net::detail::throw_exception(ex);
  }

  return addr.ipv6_address_;
}

template <class T>
inline T address_cast(const address_v4& addr,
    typename enable_if<is_same<T, address>::value>::type*) STDNET_NOEXCEPT
{
  address new_addr;
  new_addr.type_ = address::ipv4;
  new_addr.ipv4_address_ = addr;
  return new_addr;
}

template <class T>
inline T address_cast(const address_v4& addr,
    typename enable_if<is_same<T, address_v4>::value>::type*) STDNET_NOEXCEPT
{
  return addr;
}

template <class T>
T address_cast(const address_v4& addr,
    typename enable_if<is_same<T, address_v6>::value>::type*) STDNET_NOEXCEPT
{
  address_v4::bytes_type v4_bytes = addr.to_bytes();
  address_v6::bytes_type v6_bytes = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0xFF, 0xFF, v4_bytes[0], v4_bytes[1], v4_bytes[2], v4_bytes[3] } };
  return address_v6(v6_bytes);
}

template <class T>
inline T address_cast(const address_v6& addr,
    typename enable_if<is_same<T, address>::value>::type*) STDNET_NOEXCEPT
{
  address new_addr;
  new_addr.type_ = address::ipv6;
  new_addr.ipv6_address_ = addr;
  return new_addr;
}

template <class T>
T address_cast(const address_v6& addr,
    typename enable_if<is_same<T, address_v4>::value>::type*)
{
  if (!addr.is_v4_mapped())
  {
    bad_address_cast ex;
    std::experimental::net::detail::throw_exception(ex);
  }

  address_v6::bytes_type v6_bytes = addr.to_bytes();
  address_v4::bytes_type v4_bytes = { {
    v6_bytes[12], v6_bytes[13], v6_bytes[14], v6_bytes[15] } };
  return address_v4(v4_bytes);
}

template <class T>
inline T address_cast(const address_v6& addr,
    typename enable_if<is_same<T, address_v6>::value>::type*) STDNET_NOEXCEPT
{
  return addr;
}

} // namespace ip
} // namespace net
} // namespace experimental
} // namespace std

#include "std/net/detail/pop_options.hpp"

#endif // STDNET_IP_ADDRESS_CAST_HPP
