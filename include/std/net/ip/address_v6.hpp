//
// ip/address_v6.hpp
// ~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STDNET_IP_ADDRESS_V6_HPP
#define STDNET_IP_ADDRESS_V6_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "std/net/detail/config.hpp"
#include <array>
#include <string>
#include <system_error>
#include "std/net/ip/fwd.hpp"
#include "std/net/detail/socket_types.hpp"
#include "std/net/detail/winsock_init.hpp"

#if !defined(STDNET_NO_IOSTREAM)
# include <iosfwd>
#endif // !defined(STDNET_NO_IOSTREAM)

#include "std/net/detail/push_options.hpp"

namespace std {
namespace experimental {
namespace net {
namespace ip {

class address;
class address_v4;

/// Implements IP version 6 style addresses.
/**
 * The ip::address_v6 class provides the ability to use and manipulate IP
 * version 6 addresses.
 *
 * @par Thread Safety
 * @e Distinct @e objects: Safe.@n
 * @e Shared @e objects: Unsafe.
 */
class address_v6
{
public:
  /// The type used to represent an address as an array of bytes.
  typedef std::array<unsigned char, 16> bytes_type;

  /// Default constructor.
  STDNET_DECL address_v6() STDNET_NOEXCEPT;

#if defined(STDNET_HAS_VARIADIC_TEMPLATES)
  /// Explicitly construct from a list of arguments.
  template <typename... T,
    typename = typename enable_if<is_same<address_v6,
      decltype(make_address_v6(declval<T>()...))>::value>::type>
  explicit address_v6(T&&... t)
    : address(make_address_v6(forward<T>(t)...))
  {
  }
#else // defined(STDNET_HAS_VARIADIC_TEMPLATES)
  template <typename T1>
  explicit address_v6(T1& t1, typename enable_if<is_same<address_v6,
    decltype(make_address_v6(declval<T1&>()))>::value>::type* = 0)
      { *this = make_address_v6(t1); }
  template <typename T1>
  explicit address_v6(const T1& t1, typename enable_if<is_same<address_v6,
    decltype(make_address_v6(declval<T1>()))>::value>::type* = 0)
      { *this = make_address_v6(t1); }
  template <typename T1, typename T2>
  address_v6(T1& t1, T2& t2, typename enable_if<is_same<address_v6,
    decltype(make_address_v6(declval<T1&>(), declval<T2&>()))>::value>::type* = 0)
      { *this = make_address_v6(t1, t2); }
  template <typename T1, typename T2>
  address_v6(T1& t1, const T2& t2, typename enable_if<is_same<address_v6,
    decltype(make_address_v6(declval<T1&>(), declval<T2>()))>::value>::type* = 0)
      { *this = make_address_v6(t1, t2); }
  template <typename T1, typename T2>
  address_v6(const T1& t1, T2& t2, typename enable_if<is_same<address_v6,
    decltype(make_address_v6(declval<T1>(), declval<T2&>()))>::value>::type* = 0)
      { *this = make_address_v6(t1, t2); }
  template <typename T1, typename T2>
  address_v6(const T1& t1, const T2& t2, typename enable_if<is_same<address_v6,
    decltype(make_address_v6(declval<T1>(), declval<T2>()))>::value>::type* = 0)
      { *this = make_address_v6(t1, t2); }
#endif // defined(STDNET_HAS_VARIADIC_TEMPLATES)

  /// Copy constructor.
  STDNET_DECL address_v6(const address_v6& other) STDNET_NOEXCEPT;

#if defined(STDNET_HAS_MOVE)
  /// Move constructor.
  STDNET_DECL address_v6(address_v6&& other) STDNET_NOEXCEPT;
#endif // defined(STDNET_HAS_MOVE)

  /// Assign from another address.
  STDNET_DECL address_v6& operator=(const address_v6& other) STDNET_NOEXCEPT;

#if defined(STDNET_HAS_MOVE)
  /// Move-assign from another address.
  STDNET_DECL address_v6& operator=(address_v6&& other) STDNET_NOEXCEPT;
#endif // defined(STDNET_HAS_MOVE)

  /// The scope ID of the address.
  /**
   * Returns the scope ID associated with the IPv6 address.
   */
  unsigned long scope_id() const STDNET_NOEXCEPT
  {
    return scope_id_;
  }

  /// The scope ID of the address.
  /**
   * Modifies the scope ID associated with the IPv6 address.
   */
  void scope_id(unsigned long id) STDNET_NOEXCEPT
  {
    scope_id_ = id;
  }

  /// Get the address in bytes, in network byte order.
  STDNET_DECL bytes_type to_bytes() const STDNET_NOEXCEPT;

  /// Get the address as a string.
  STDNET_DECL std::string to_string() const;

  /// Get the address as a string.
  STDNET_DECL std::string to_string(std::error_code& ec) const;

  /// Determine whether the address is a loopback address.
  STDNET_DECL bool is_loopback() const STDNET_NOEXCEPT;

  /// Determine whether the address is unspecified.
  STDNET_DECL bool is_unspecified() const STDNET_NOEXCEPT;

  /// Determine whether the address is link local.
  STDNET_DECL bool is_link_local() const STDNET_NOEXCEPT;

  /// Determine whether the address is site local.
  STDNET_DECL bool is_site_local() const STDNET_NOEXCEPT;

  /// Determine whether the address is a mapped IPv4 address.
  STDNET_DECL bool is_v4_mapped() const STDNET_NOEXCEPT;

  /// Determine whether the address is a multicast address.
  STDNET_DECL bool is_multicast() const STDNET_NOEXCEPT;

  /// Determine whether the address is a global multicast address.
  STDNET_DECL bool is_multicast_global() const STDNET_NOEXCEPT;

  /// Determine whether the address is a link-local multicast address.
  STDNET_DECL bool is_multicast_link_local() const STDNET_NOEXCEPT;

  /// Determine whether the address is a node-local multicast address.
  STDNET_DECL bool is_multicast_node_local() const STDNET_NOEXCEPT;

  /// Determine whether the address is a org-local multicast address.
  STDNET_DECL bool is_multicast_org_local() const STDNET_NOEXCEPT;

  /// Determine whether the address is a site-local multicast address.
  STDNET_DECL bool is_multicast_site_local() const STDNET_NOEXCEPT;

  /// Compare two addresses for equality.
  STDNET_DECL friend bool operator==( const address_v6& a1,
      const address_v6& a2) STDNET_NOEXCEPT;

  /// Compare two addresses for inequality.
  friend bool operator!=(const address_v6& a1,
      const address_v6& a2) STDNET_NOEXCEPT
  {
    return !(a1 == a2);
  }

  /// Compare addresses for ordering.
  STDNET_DECL friend bool operator<(const address_v6& a1,
      const address_v6& a2) STDNET_NOEXCEPT;

  /// Compare addresses for ordering.
  friend bool operator>(const address_v6& a1,
      const address_v6& a2) STDNET_NOEXCEPT
  {
    return a2 < a1;
  }

  /// Compare addresses for ordering.
  friend bool operator<=(const address_v6& a1,
      const address_v6& a2) STDNET_NOEXCEPT
  {
    return !(a2 < a1);
  }

  /// Compare addresses for ordering.
  friend bool operator>=(const address_v6& a1,
      const address_v6& a2) STDNET_NOEXCEPT
  {
    return !(a1 < a2);
  }

  /// Obtain an address object that represents any address.
  static address_v6 any() STDNET_NOEXCEPT
  {
    return address_v6();
  }

  /// Obtain an address object that represents the loopback address.
  STDNET_DECL static address_v6 loopback() STDNET_NOEXCEPT;

private:
  // The underlying IPv6 address.
  std::experimental::net::detail::in6_addr_type addr_;

  // The scope ID associated with the address.
  unsigned long scope_id_;

  friend address_v6 make_address_v6(const bytes_type&, unsigned long);
};

/// Construct an address_v6 from raw bytes.
STDNET_DECL address_v6 make_address_v6(
    const std::array<unsigned char, 16>& bytes, unsigned long scope_id);

/// Create an address_v6 from an IPv6 address string.
STDNET_DECL address_v6 make_address_v6(const char* str);

/// Create an address_v6 from an IPv6 address string.
STDNET_DECL address_v6 make_address_v6(const char* str,
    std::error_code& ec) STDNET_NOEXCEPT;

/// Create an address_v6 from an IPv6 address string.
STDNET_DECL address_v6 make_address_v6(const std::string& str);

/// Create an address_v6 from an IPv6 address string.
STDNET_DECL address_v6 make_address_v6(const std::string& str,
    std::error_code& ec) STDNET_NOEXCEPT;

#if !defined(STDNET_NO_IOSTREAM)

/// Output an address as a string.
/**
 * Used to output a human-readable string for a specified address.
 *
 * @param os The output stream to which the string will be written.
 *
 * @param addr The address to be written.
 *
 * @return The output stream.
 *
 * @relates std::experimental::net::ip::address_v6
 */
template <typename Elem, typename Traits>
std::basic_ostream<Elem, Traits>& operator<<(
    std::basic_ostream<Elem, Traits>& os, const address_v6& addr);

#endif // !defined(STDNET_NO_IOSTREAM)

} // namespace ip
} // namespace net
} // namespace experimental
} // namespace std

#include "std/net/detail/pop_options.hpp"

#include "std/net/ip/impl/address_v6.hpp"
#if defined(STDNET_HEADER_ONLY)
# include "std/net/ip/impl/address_v6.ipp"
#endif // defined(STDNET_HEADER_ONLY)

#include "std/net/ip/address.hpp"

#endif // STDNET_IP_ADDRESS_V6_HPP
