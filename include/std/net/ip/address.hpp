//
// ip/address.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STDNET_IP_ADDRESS_HPP
#define STDNET_IP_ADDRESS_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "std/net/detail/config.hpp"
#include <string>
#include <system_error>
#include <type_traits>
#include "std/net/ip/fwd.hpp"
#include "std/net/ip/address_v4.hpp"
#include "std/net/ip/address_v6.hpp"

#if !defined(STDNET_NO_IOSTREAM)
# include <iosfwd>
#endif // !defined(STDNET_NO_IOSTREAM)

#include "std/net/detail/push_options.hpp"

namespace std {
namespace net {
namespace ip {
namespace detail {

template <class T, class = void>
struct is_convertible_to_address_1 : false_type {};

template <class T>
struct is_convertible_to_address_1<T,
  typename enable_if<is_same<address,
    decltype(address_cast<address>(declval<T>()))>::value>::type> : true_type {};

template <class T>
struct is_convertible_to_address : is_convertible_to_address_1<T> {};

template <>
struct is_convertible_to_address<address> : false_type {};

} // namespace detail

//template <> struct is_convertible_to_address<address> : false_type {};

/// Implements version-independent IP addresses.
/**
 * The ip::address class provides the ability to use either IP version 4 or
 * version 6 addresses.
 *
 * @par Thread Safety
 * @e Distinct @e objects: Safe.@n
 * @e Shared @e objects: Unsafe.
 */
class address
{
public:
  /// Default constructor.
  STDNET_DECL address() STDNET_NOEXCEPT;

  /// Construct from another address type.
  template <typename T,
    typename = typename enable_if<
      detail::is_convertible_to_address<T>::value>::type>
  address(const T& t) STDNET_NOEXCEPT
    : address(address_cast<address>(t))
  {
  }

#if defined(STDNET_HAS_VARIADIC_TEMPLATES)
  /// Explicitly construct from a list of arguments.
  template <typename... T,
    typename = typename enable_if<is_same<address,
      decltype(make_address(declval<T>()...))>::value>::type>
  explicit address(T&&... t)
    : address(make_address(forward<T>(t)...))
  {
  }
#else // defined(STDNET_HAS_VARIADIC_TEMPLATES)
  template <typename T1>
  explicit address(T1& t1, typename enable_if<is_same<address,
    decltype(make_address(declval<T1&>()))>::value>::type* = 0)
      { *this = make_address(t1); }
  template <typename T1>
  explicit address(const T1& t1, typename enable_if<is_same<address,
    decltype(make_address(declval<T1>()))>::value>::type* = 0)
      { *this = make_address(t1); }
  template <typename T1, typename T2>
  address(T1& t1, T2& t2, typename enable_if<is_same<address,
    decltype(make_address(declval<T1&>(), declval<T2&>()))>::value>::type* = 0)
      { *this = make_address(t1, t2); }
  template <typename T1, typename T2>
  address(T1& t1, const T2& t2, typename enable_if<is_same<address,
    decltype(make_address(declval<T1&>(), declval<T2>()))>::value>::type* = 0)
      { *this = make_address(t1, t2); }
  template <typename T1, typename T2>
  address(const T1& t1, T2& t2, typename enable_if<is_same<address,
    decltype(make_address(declval<T1>(), declval<T2&>()))>::value>::type* = 0)
      { *this = make_address(t1, t2); }
  template <typename T1, typename T2>
  address(const T1& t1, const T2& t2, typename enable_if<is_same<address,
    decltype(make_address(declval<T1>(), declval<T2>()))>::value>::type* = 0)
      { *this = make_address(t1, t2); }
#endif // defined(STDNET_HAS_VARIADIC_TEMPLATES)

  /// Copy constructor.
  STDNET_DECL address(const address& other) STDNET_NOEXCEPT;

#if defined(STDNET_HAS_MOVE)
  /// Move constructor.
  STDNET_DECL address(address&& other) STDNET_NOEXCEPT;
#endif // defined(STDNET_HAS_MOVE)

  /// Assign from another address.
  STDNET_DECL address& operator=(const address& other) STDNET_NOEXCEPT;

#if defined(STDNET_HAS_MOVE)
  /// Move-assign from another address.
  STDNET_DECL address& operator=(address&& other) STDNET_NOEXCEPT;
#endif // defined(STDNET_HAS_MOVE)

  /// Get whether the address is an IP version 4 address.
  bool is_v4() const STDNET_NOEXCEPT
  {
    return type_ == ipv4;
  }

  /// Get whether the address is an IP version 6 address.
  bool is_v6() const STDNET_NOEXCEPT
  {
    return type_ == ipv6;
  }

  /// Get the address as a string in dotted decimal format.
  STDNET_DECL std::string to_string() const;

  /// Get the address as a string in dotted decimal format.
  STDNET_DECL std::string to_string(std::error_code& ec) const;

  /// Determine whether the address is a loopback address.
  STDNET_DECL bool is_loopback() const STDNET_NOEXCEPT;

  /// Determine whether the address is unspecified.
  STDNET_DECL bool is_unspecified() const STDNET_NOEXCEPT;

  /// Determine whether the address is a multicast address.
  STDNET_DECL bool is_multicast() const STDNET_NOEXCEPT;

  /// Compare two addresses for equality.
  STDNET_DECL friend bool operator==(const address& a1,
      const address& a2) STDNET_NOEXCEPT;

  /// Compare two addresses for inequality.
  friend bool operator!=(const address& a1, const address& a2) STDNET_NOEXCEPT
  {
    return !(a1 == a2);
  }

  /// Compare addresses for ordering.
  STDNET_DECL friend bool operator<(const address& a1,
      const address& a2) STDNET_NOEXCEPT;

  /// Compare addresses for ordering.
  friend bool operator>(const address& a1, const address& a2) STDNET_NOEXCEPT
  {
    return a2 < a1;
  }

  /// Compare addresses for ordering.
  friend bool operator<=(const address& a1, const address& a2) STDNET_NOEXCEPT
  {
    return !(a2 < a1);
  }

  /// Compare addresses for ordering.
  friend bool operator>=(const address& a1, const address& a2) STDNET_NOEXCEPT
  {
    return !(a1 < a2);
  }

private:
  friend class address_v4;
  friend class address_v6;

  // The type of the address.
  enum { ipv4, ipv6 } type_;

  // The underlying IPv4 address.
  address_v4 ipv4_address_;

  // The underlying IPv6 address.
  address_v6 ipv6_address_;

  template <class T> friend T address_cast(const address&,
    typename enable_if<is_same<T, address_v4>::value>::type*);
  template <class T> friend T address_cast(const address&,
    typename enable_if<is_same<T, address_v6>::value>::type*);
  template <class T> friend T address_cast(const address_v4&,
    typename enable_if<is_same<T, address>::value>::type*) STDNET_NOEXCEPT;
  template <class T> friend T address_cast(const address_v6&,
    typename enable_if<is_same<T, address>::value>::type*) STDNET_NOEXCEPT;
};

/// Create an address from an IPv4 address string in dotted decimal form,
/// or from an IPv6 address in hexadecimal notation.
STDNET_DECL address make_address(const char* str);

/// Create an address from an IPv4 address string in dotted decimal form,
/// or from an IPv6 address in hexadecimal notation.
STDNET_DECL address make_address(const char* str,
    std::error_code& ec) STDNET_NOEXCEPT;

/// Create an address from an IPv4 address string in dotted decimal form,
/// or from an IPv6 address in hexadecimal notation.
STDNET_DECL address make_address(const std::string& str);

/// Create an address from an IPv4 address string in dotted decimal form,
/// or from an IPv6 address in hexadecimal notation.
STDNET_DECL address make_address(const std::string& str,
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
 * @relates std::net::ip::address
 */
template <typename Elem, typename Traits>
std::basic_ostream<Elem, Traits>& operator<<(
    std::basic_ostream<Elem, Traits>& os, const address& addr);

#endif // !defined(STDNET_NO_IOSTREAM)

} // namespace ip
} // namespace net
} // namespace std

#include "std/net/detail/pop_options.hpp"

#include "std/net/ip/impl/address.hpp"
#if defined(STDNET_HEADER_ONLY)
# include "std/net/ip/impl/address.ipp"
#endif // defined(STDNET_HEADER_ONLY)

#include "std/net/ip/address_cast.hpp"

#endif // STDNET_IP_ADDRESS_HPP
