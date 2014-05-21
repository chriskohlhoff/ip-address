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
#include "std/net/detail/throw_exception.hpp"
#include "std/net/ip/fwd.hpp"
#include "std/net/ip/address.hpp"
#include "std/net/ip/address_v4.hpp"
#include "std/net/ip/address_v6.hpp"
#include "std/net/ip/bad_address_cast.hpp"

#include "std/net/detail/push_options.hpp"

namespace std {
namespace experimental {
namespace net {
namespace ip {

/// Cast a version-independent address to itself.
template <class T>
inline STDNET_CONSTEXPR T address_cast(const address& addr,
    typename enable_if<is_same<T, address>::value>::type*) STDNET_NOEXCEPT
{
  return addr;
}

/// Cast a version-independent address to an IPv4 address.
/**
 * @throws bad_address_cast if @c a does not represent an IPv4 address.
 */
template <class T>
inline STDNET_CONSTEXPR T address_cast(const address& addr,
    typename enable_if<is_same<T, address_v4>::value>::type*)
{
  return (addr.type_ != address::ipv4)
    ? throw bad_address_cast()
    : addr.ipv4_address_;
}

/// Cast a version-independent address to an IPv6 address.
/**
 * @throws bad_address_cast if @c a does not represent an IPv6 address.
 */
template <class T>
inline STDNET_CONSTEXPR T address_cast(const address& addr,
    typename enable_if<is_same<T, address_v6>::value>::type*)
{
  return (addr.type_ != address::ipv6)
    ? throw bad_address_cast()
    : addr.ipv6_address_;
}

/// Cast an IPv4 address to a version-independent address.
template <class T>
inline STDNET_CONSTEXPR T address_cast(const address_v4& addr,
    typename enable_if<is_same<T, address>::value>::type*) STDNET_NOEXCEPT
{
  return address(address::ipv4, addr, address_v6());
}

/// Cast an IPv4 address to itself.
template <class T>
inline STDNET_CONSTEXPR T address_cast(const address_v4& addr,
    typename enable_if<is_same<T, address_v4>::value>::type*) STDNET_NOEXCEPT
{
  return addr;
}

/// Cast an IPv6 address to a version-independent address.
template <class T>
inline STDNET_CONSTEXPR T address_cast(const address_v6& addr,
    typename enable_if<is_same<T, address>::value>::type*) STDNET_NOEXCEPT
{
  return address(address::ipv6, address_v4(), addr);
}

/// Cast an IPv6 address to itself.
template <class T>
inline STDNET_CONSTEXPR T address_cast(const address_v6& addr,
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
