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
#include "std/net/ip/address_v4.hpp"
#include "std/net/ip/bad_address_cast.hpp"
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
  /// A standard-layout type used to represent an address as an array of bytes.
  struct bytes_type : std::array<unsigned char, 16>
  {
#if defined(STDNET_HAS_VARIADIC_TEMPLATES)
    template <class... T>
    explicit STDNET_CONSTEXPR bytes_type(T... t)
      : std::array<unsigned char, 16>{{static_cast<unsigned char>(t)...}}
    {
    }
#else // defined(STDNET_HAS_VARIADIC_TEMPLATES)
    explicit STDNET_CONSTEXPR bytes_type(
        unsigned char a = 0, unsigned char b = 0,
        unsigned char c = 0, unsigned char d = 0,
        unsigned char e = 0, unsigned char f = 0,
        unsigned char g = 0, unsigned char h = 0,
        unsigned char i = 0, unsigned char j = 0,
        unsigned char k = 0, unsigned char l = 0,
        unsigned char m = 0, unsigned char n = 0,
        unsigned char o = 0, unsigned char p = 0)
# if defined(STDNET_HAS_CONSTEXPR)
      : std::array<unsigned char, 16>{{a, b, c, d, e, f, g, h, i, h, k, l, m, n, o, p}}
    {
    }
# else // defined(STDNET_HAS_CONSTEXPR)
    {
      (*this)[0] = a, (*this)[1] = b, (*this)[2] = c, (*this)[3] = d;
      (*this)[4] = e, (*this)[5] = f, (*this)[6] = g, (*this)[7] = h;
      (*this)[8] = i, (*this)[9] = j, (*this)[10] = k, (*this)[11] = l;
      (*this)[12] = m, (*this)[13] = n, (*this)[14] = o, (*this)[15] = p;
    }
# endif // defined(STDNET_HAS_CONSTEXPR)
#endif // defined(STDNET_HAS_VARIADIC_TEMPLATES)
  };

  /// Default constructor.
  STDNET_CONSTEXPR address_v6() STDNET_NOEXCEPT
    : bytes_(0),
      scope_id_(0)
  {
  }

  /// Implicit construction from bytes, in network byte order.
  STDNET_CONSTEXPR address_v6(const bytes_type& bytes, unsigned long scope = 0)
    : bytes_(
#if UCHAR_MAX > 0xFF
        (bytes[0] > 0xFF || bytes[1] > 0xFF || bytes[2] > 0xFF || bytes[3] > 0xFF
        || bytes[4] > 0xFF || bytes[5] > 0xFF || bytes[6] > 0xFF || bytes[7] > 0xFF
        || bytes[8] > 0xFF || bytes[9] > 0xFF || bytes[10] > 0xFF || bytes[11] > 0xFF
        || bytes[12] > 0xFF || bytes[13] > 0xFF || bytes[14] > 0xFF || bytes[15] > 0xFF)
        ? throw std::out_of_range("address_v6 from bytes_type") :
#endif // UCHAR_MAX > 0xFF
        bytes),
      scope_id_(scope)
  {
  }

#if defined(STDNET_HAS_VARIADIC_TEMPLATES)
  /// Explicitly construct from a list of arguments.
  template <typename... T,
    typename = typename enable_if<is_same<address_v6,
      decltype(make_address_v6(declval<T>()...))>::value>::type>
  explicit STDNET_CONSTEXPR address_v6(T&&... t)
    : address_v6(make_address_v6(static_cast<T&&>(t)...))
  {
  }
#else // defined(STDNET_HAS_VARIADIC_TEMPLATES)
  template <typename T1>
  explicit STDNET_CONSTEXPR address_v6(T1& t1,
    typename enable_if<is_same<address_v6,
      decltype(make_address_v6(declval<T1&>()))>::value>::type* = 0)
        : bytes_(make_address_v6(t1).to_bytes()) {}
  template <typename T1>
  explicit STDNET_CONSTEXPR address_v6(const T1& t1,
    typename enable_if<is_same<address_v6,
      decltype(make_address_v6(declval<T1>()))>::value>::type* = 0)
        : bytes_(make_address_v6(t1).to_bytes()) {}
  template <typename T1, typename T2>
  STDNET_CONSTEXPR address_v6(T1& t1, T2& t2,
    typename enable_if<is_same<address_v6,
      decltype(make_address_v6(declval<T1&>(), declval<T2&>()))>::value>::type* = 0)
        : bytes_(make_address_v6(t1, t2).to_bytes()) {}
  template <typename T1, typename T2>
  STDNET_CONSTEXPR address_v6(T1& t1, const T2& t2,
    typename enable_if<is_same<address_v6,
      decltype(make_address_v6(declval<T1&>(), declval<T2>()))>::value>::type* = 0)
        : bytes_(make_address_v6(t1, t2).to_bytes()) {}
  template <typename T1, typename T2>
  STDNET_CONSTEXPR address_v6(const T1& t1, T2& t2,
    typename enable_if<is_same<address_v6,
      decltype(make_address_v6(declval<T1>(), declval<T2&>()))>::value>::type* = 0)
        : bytes_(make_address_v6(t1, t2).to_bytes()) {}
  template <typename T1, typename T2>
  STDNET_CONSTEXPR address_v6(const T1& t1, const T2& t2,
    typename enable_if<is_same<address_v6,
      decltype(make_address_v6(declval<T1>(), declval<T2>()))>::value>::type* = 0)
        : bytes_(make_address_v6(t1, t2).to_bytes()) {}
#endif // defined(STDNET_HAS_VARIADIC_TEMPLATES)

  /// Copy constructor.
  STDNET_CONSTEXPR address_v6(const address_v6& other) STDNET_NOEXCEPT
    : bytes_(other.bytes_), scope_id_(other.scope_id_)
  {
  }

#if defined(STDNET_HAS_MOVE)
  /// Move constructor.
  address_v6(address_v6&& other) STDNET_NOEXCEPT
    : bytes_(other.bytes_), scope_id_(other.scope_id_)
  {
  }
#endif // defined(STDNET_HAS_MOVE)

  /// Assign from another address.
  address_v6& operator=(const address_v6& other) STDNET_NOEXCEPT
  {
    bytes_ = other.bytes_;
    scope_id_ = other.scope_id_;
    return *this;
  }

#if defined(STDNET_HAS_MOVE)
  /// Move-assign from another address.
  address_v6& operator=(address_v6&& other) STDNET_NOEXCEPT
  {
    bytes_ = other.bytes_;
    scope_id_ = other.scope_id_;
    return *this;
  }
#endif // defined(STDNET_HAS_MOVE)

  /// The scope ID of the address.
  /**
   * Returns the scope ID associated with the IPv6 address.
   */
  STDNET_CONSTEXPR unsigned long scope_id() const STDNET_NOEXCEPT
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
  STDNET_CONSTEXPR bytes_type to_bytes() const STDNET_NOEXCEPT
  {
    return bytes_;
  }

  /// Get the address as a string.
  STDNET_DECL std::string to_string() const;

  /// Get the address as a string.
  STDNET_DECL std::string to_string(std::error_code& ec) const;

  /// Determine whether the address is a loopback address.
  STDNET_CONSTEXPR bool is_loopback() const STDNET_NOEXCEPT
  {
    return ((bytes_[0] == 0) && (bytes_[1] == 0)
        && (bytes_[2] == 0) && (bytes_[3] == 0)
        && (bytes_[4] == 0) && (bytes_[5] == 0)
        && (bytes_[6] == 0) && (bytes_[7] == 0)
        && (bytes_[8] == 0) && (bytes_[9] == 0)
        && (bytes_[10] == 0) && (bytes_[11] == 0)
        && (bytes_[12] == 0) && (bytes_[13] == 0)
        && (bytes_[14] == 0) && (bytes_[15] == 1));
  }

  /// Determine whether the address is unspecified.
  STDNET_CONSTEXPR bool is_unspecified() const STDNET_NOEXCEPT
  {
    return ((bytes_[0] == 0) && (bytes_[1] == 0)
        && (bytes_[2] == 0) && (bytes_[3] == 0)
        && (bytes_[4] == 0) && (bytes_[5] == 0)
        && (bytes_[6] == 0) && (bytes_[7] == 0)
        && (bytes_[8] == 0) && (bytes_[9] == 0)
        && (bytes_[10] == 0) && (bytes_[11] == 0)
        && (bytes_[12] == 0) && (bytes_[13] == 0)
        && (bytes_[14] == 0) && (bytes_[15] == 0));
  }

  /// Determine whether the address is link local.
  STDNET_CONSTEXPR bool is_link_local() const STDNET_NOEXCEPT
  {
    return ((bytes_[0] == 0xfe) && ((bytes_[1] & 0xc0) == 0x80));
  }

  /// Determine whether the address is site local.
  STDNET_CONSTEXPR bool is_site_local() const STDNET_NOEXCEPT
  {
    return ((bytes_[0] == 0xfe) && ((bytes_[1] & 0xc0) == 0xc0));
  }

  /// Determine whether the address is a mapped IPv4 address.
  STDNET_CONSTEXPR bool is_v4_mapped() const STDNET_NOEXCEPT
  {
    return ((bytes_[0] == 0) && (bytes_[1] == 0)
        && (bytes_[2] == 0) && (bytes_[3] == 0)
        && (bytes_[4] == 0) && (bytes_[5] == 0)
        && (bytes_[6] == 0) && (bytes_[7] == 0)
        && (bytes_[8] == 0) && (bytes_[9] == 0)
        && (bytes_[10] == 0xff) && (bytes_[11] == 0xff));
  }

  /// Determine whether the address is a multicast address.
  STDNET_CONSTEXPR bool is_multicast() const STDNET_NOEXCEPT
  {
    return (bytes_[0] == 0xff);
  }

  /// Determine whether the address is a global multicast address.
  STDNET_CONSTEXPR bool is_multicast_global() const STDNET_NOEXCEPT
  {
    return ((bytes_[0] == 0xff) && ((bytes_[1] & 0x0f) == 0x0e));
  }

  /// Determine whether the address is a link-local multicast address.
  STDNET_CONSTEXPR bool is_multicast_link_local() const STDNET_NOEXCEPT
  {
    return ((bytes_[0] == 0xff) && ((bytes_[1] & 0x0f) == 0x02));
  }

  /// Determine whether the address is a node-local multicast address.
  STDNET_CONSTEXPR bool is_multicast_node_local() const STDNET_NOEXCEPT
  {
    return ((bytes_[0] == 0xff) && ((bytes_[1] & 0x0f) == 0x01));
  }

  /// Determine whether the address is a org-local multicast address.
  STDNET_CONSTEXPR bool is_multicast_org_local() const STDNET_NOEXCEPT
  {
    return ((bytes_[0] == 0xff) && ((bytes_[1] & 0x0f) == 0x08));
  }

  /// Determine whether the address is a site-local multicast address.
  STDNET_CONSTEXPR bool is_multicast_site_local() const STDNET_NOEXCEPT
  {
    return ((bytes_[0] == 0xff) && ((bytes_[1] & 0x0f) == 0x05));
  }

  /// Compare two addresses for equality.
  friend bool operator==(const address_v6& a1,
      const address_v6& a2) STDNET_NOEXCEPT
  {
    return a1.bytes_ == a2.bytes_ && a1.scope_id_ == a2.scope_id_;
  }

  /// Compare two addresses for inequality.
  friend bool operator!=(const address_v6& a1,
      const address_v6& a2) STDNET_NOEXCEPT
  {
    return !(a1 == a2);
  }

  /// Compare addresses for ordering.
  friend bool operator<(const address_v6& a1,
      const address_v6& a2) STDNET_NOEXCEPT
  {
    if (a1.bytes_ < a2.bytes_)
      return true;
    if (a1.bytes_ > a2.bytes_)
      return false;
    return a1.scope_id_ < a2.scope_id_;
  }

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
  static STDNET_CONSTEXPR address_v6 any() STDNET_NOEXCEPT
  {
    return address_v6();
  }

  /// Obtain an address object that represents the loopback address.
  static STDNET_CONSTEXPR address_v6 loopback() STDNET_NOEXCEPT
  {
    return bytes_type(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);
  }

private:
  friend STDNET_CONSTEXPR address_v4 make_address_v4(
      v4_mapped_t, const address_v6&);

  // The underlying IPv6 address.
  bytes_type bytes_;

  // The scope ID associated with the address.
  unsigned long scope_id_;
};

/// Construct an address_v6 from raw bytes.
inline STDNET_CONSTEXPR address_v6 make_address_v6(
    const address_v6::bytes_type& bytes, unsigned long scope_id = 0)
{
  return address_v6(bytes, scope_id);
}

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

/// Create an IPv4-mapped address_v6 from an IPv4 address.
inline STDNET_CONSTEXPR address_v6 make_address_v6(
    v4_mapped_t, const address_v4& addr) STDNET_NOEXCEPT
{
  return address_v6::bytes_type(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xFF, 0xFF,
      static_cast<const address_v4::bytes_type>(addr.to_bytes())[0],
      static_cast<const address_v4::bytes_type>(addr.to_bytes())[1],
      static_cast<const address_v4::bytes_type>(addr.to_bytes())[2],
      static_cast<const address_v4::bytes_type>(addr.to_bytes())[3]);
}

/// Create an address_v4 from an IPv4-mapped address_v6.
inline STDNET_CONSTEXPR address_v4 make_address_v4(
    v4_mapped_t, const address_v6& addr)
{
  return !addr.is_v4_mapped() ? throw bad_address_cast() :
    address_v4::bytes_type(addr.bytes_[12], addr.bytes_[13],
        addr.bytes_[14], addr.bytes_[15]);
}

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

#endif // STDNET_IP_ADDRESS_V6_HPP
