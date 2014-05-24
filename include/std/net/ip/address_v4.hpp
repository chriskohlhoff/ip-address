//
// ip/address_v4.hpp
// ~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STDNET_IP_ADDRESS_V4_HPP
#define STDNET_IP_ADDRESS_V4_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "std/net/detail/config.hpp"
#include <array>
#include <string>
#include <system_error>
#include "std/net/ip/fwd.hpp"
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

/// Implements IP version 4 style addresses.
/**
 * The ip::address_v4 class provides the ability to use and manipulate IP
 * version 4 addresses.
 *
 * @par Thread Safety
 * @e Distinct @e objects: Safe.@n
 * @e Shared @e objects: Unsafe.
 */
class address_v4
{
public:
  /// A standard-layout type used to represent an address as an array of bytes.
  struct bytes_type : std::array<unsigned char, 4>
  {
#if defined(STDNET_HAS_VARIADIC_TEMPLATES)
    template <class... T>
    explicit STDNET_CONSTEXPR bytes_type(T... t)
      : std::array<unsigned char, 4>{{static_cast<unsigned char>(t)...}}
    {
    }
#else // defined(STDNET_HAS_VARIADIC_TEMPLATES)
    explicit STDNET_CONSTEXPR bytes_type(unsigned char a = 0,
        unsigned char b = 0, unsigned char c = 0, unsigned char d = 0)
# if defined(STDNET_HAS_CONSTEXPR)
      : std::array<unsigned char, 4>{{a, b, c, d}}
    {
    }
# else // defined(STDNET_HAS_CONSTEXPR)
    {
      (*this)[0] = a, (*this)[1] = b, (*this)[2] = c, (*this)[3] = d;
    }
# endif // defined(STDNET_HAS_CONSTEXPR)
#endif // defined(STDNET_HAS_VARIADIC_TEMPLATES)
  };

  /// Default constructor.
  STDNET_CONSTEXPR address_v4() STDNET_NOEXCEPT
    : bytes_(0, 0, 0, 0)
  {
  }

  /// Implicit construction from bytes, in network byte order.
  STDNET_CONSTEXPR address_v4(const bytes_type& bytes)
    : bytes_(
#if UCHAR_MAX > 0xFF
        (bytes[0] > 0xFF || bytes[1] > 0xFF || bytes[2] > 0xFF || bytes[3] > 0xFF)
        ? throw std::out_of_range("address_v4 from bytes_type") :
#endif // UCHAR_MAX > 0xFF
        bytes)
  {
  }

#if defined(STDNET_HAS_VARIADIC_TEMPLATES)
  /// Explicitly construct from a list of arguments.
  template <typename... T,
    typename = typename enable_if<is_same<address_v4,
      decltype(make_address_v4(declval<T>()...))>::value>::type>
  explicit STDNET_CONSTEXPR address_v4(T&&... t)
    : address_v4(make_address_v4(static_cast<T&&>(t)...))
  {
  }
#else // defined(STDNET_HAS_VARIADIC_TEMPLATES)
  template <typename T1>
  explicit STDNET_CONSTEXPR address_v4(T1& t1,
    typename enable_if<is_same<address_v4,
      decltype(make_address_v4(declval<T1&>()))>::value>::type* = 0)
        : bytes_(make_address_v4(t1).to_bytes()) {}
  template <typename T1>
  explicit STDNET_CONSTEXPR address_v4(const T1& t1,
    typename enable_if<is_same<address_v4,
      decltype(make_address_v4(declval<T1>()))>::value>::type* = 0)
        : bytes_(make_address_v4(t1).to_bytes()) {}
  template <typename T1, typename T2>
  STDNET_CONSTEXPR address_v4(T1& t1, T2& t2,
    typename enable_if<is_same<address_v4,
      decltype(make_address_v4(declval<T1&>(), declval<T2&>()))>::value>::type* = 0)
        : bytes_(make_address_v4(t1, t2).to_bytes()) {}
  template <typename T1, typename T2>
  STDNET_CONSTEXPR address_v4(T1& t1, const T2& t2,
    typename enable_if<is_same<address_v4,
      decltype(make_address_v4(declval<T1&>(), declval<T2>()))>::value>::type* = 0)
        : bytes_(make_address_v4(t1, t2).to_bytes()) {}
  template <typename T1, typename T2>
  STDNET_CONSTEXPR address_v4(const T1& t1, T2& t2,
    typename enable_if<is_same<address_v4,
      decltype(make_address_v4(declval<T1>(), declval<T2&>()))>::value>::type* = 0)
        : bytes_(make_address_v4(t1, t2).to_bytes()) {}
  template <typename T1, typename T2>
  STDNET_CONSTEXPR address_v4(const T1& t1, const T2& t2,
    typename enable_if<is_same<address_v4,
      decltype(make_address_v4(declval<T1>(), declval<T2>()))>::value>::type* = 0)
        : bytes_(make_address_v4(t1, t2).to_bytes()) {}
#endif // defined(STDNET_HAS_VARIADIC_TEMPLATES)

  /// Copy constructor.
  STDNET_CONSTEXPR address_v4(const address_v4& other) STDNET_NOEXCEPT
    : bytes_(other.bytes_)
  {
  }

#if defined(STDNET_HAS_MOVE)
  /// Move constructor.
  address_v4(address_v4&& other) STDNET_NOEXCEPT
    : bytes_(other.bytes_)
  {
  }
#endif // defined(STDNET_HAS_MOVE)

  /// Assign from another address.
  address_v4& operator=(const address_v4& other) STDNET_NOEXCEPT
  {
    bytes_ = other.bytes_;
    return *this;
  }

#if defined(STDNET_HAS_MOVE)
  /// Move-assign from another address.
  address_v4& operator=(address_v4&& other) STDNET_NOEXCEPT
  {
    bytes_ = other.bytes_;
    return *this;
  }
#endif // defined(STDNET_HAS_MOVE)

  /// Get the address in bytes, in network byte order.
  STDNET_CONSTEXPR bytes_type to_bytes() const STDNET_NOEXCEPT
  {
    return bytes_;
  }

  /// Get the address as an unsigned long in host byte order
  STDNET_CONSTEXPR unsigned long to_ulong() const STDNET_NOEXCEPT
  {
    return (static_cast<unsigned long>(bytes_[0]) << 24)
      | (static_cast<unsigned long>(bytes_[1]) << 16)
      | (static_cast<unsigned long>(bytes_[2]) << 8)
      | static_cast<unsigned long>(bytes_[3]);
  }

  /// Get the address as a string in dotted decimal format.
  STDNET_DECL std::string to_string() const;

  /// Get the address as a string in dotted decimal format.
  STDNET_DECL std::string to_string(std::error_code& ec) const;

  /// Determine whether the address is a loopback address.
  STDNET_CONSTEXPR bool is_loopback() const STDNET_NOEXCEPT
  {
    return (to_ulong() & 0xFF000000) == 0x7F000000;
  }

  /// Determine whether the address is unspecified.
  STDNET_CONSTEXPR bool is_unspecified() const STDNET_NOEXCEPT
  {
    return to_ulong() == 0;
  }

  /// Determine whether the address is a class A address.
  STDNET_CONSTEXPR bool is_class_a() const STDNET_NOEXCEPT
  {
    return (to_ulong() & 0x80000000) == 0;
  }

  /// Determine whether the address is a class B address.
  STDNET_CONSTEXPR bool is_class_b() const STDNET_NOEXCEPT
  {
    return (to_ulong() & 0xC0000000) == 0x80000000;
  }

  /// Determine whether the address is a class C address.
  STDNET_CONSTEXPR bool is_class_c() const STDNET_NOEXCEPT
  {
    return (to_ulong() & 0xE0000000) == 0xC0000000;
  }

  /// Determine whether the address is a multicast address.
  STDNET_CONSTEXPR bool is_multicast() const STDNET_NOEXCEPT
  {
    return (to_ulong() & 0xF0000000) == 0xE0000000;
  }

  /// Compare two addresses for equality.
  friend bool operator==(const address_v4& a1,
      const address_v4& a2) STDNET_NOEXCEPT
  {
    return a1.bytes_ == a2.bytes_;
  }

  /// Compare two addresses for inequality.
  friend bool operator!=(const address_v4& a1,
      const address_v4& a2) STDNET_NOEXCEPT
  {
    return a1.bytes_ == a2.bytes_;
  }

  /// Compare addresses for ordering.
  friend bool operator<(const address_v4& a1,
      const address_v4& a2) STDNET_NOEXCEPT
  {
    return a1.to_ulong() < a2.to_ulong();
  }

  /// Compare addresses for ordering.
  friend bool operator>(const address_v4& a1,
      const address_v4& a2) STDNET_NOEXCEPT
  {
    return a1.to_ulong() > a2.to_ulong();
  }

  /// Compare addresses for ordering.
  friend bool operator<=(const address_v4& a1,
      const address_v4& a2) STDNET_NOEXCEPT
  {
    return a1.to_ulong() <= a2.to_ulong();
  }

  /// Compare addresses for ordering.
  friend bool operator>=(const address_v4& a1,
      const address_v4& a2) STDNET_NOEXCEPT
  {
    return a1.to_ulong() >= a2.to_ulong();
  }

  /// Obtain an address object that represents any address.
  static STDNET_CONSTEXPR address_v4 any() STDNET_NOEXCEPT
  {
    return address_v4();
  }

  /// Obtain an address object that represents the loopback address.
  static STDNET_CONSTEXPR address_v4 loopback() STDNET_NOEXCEPT
  {
    return address_v4(0x7F000001);
  }

  /// Obtain an address object that represents the broadcast address.
  static STDNET_CONSTEXPR address_v4 broadcast() STDNET_NOEXCEPT
  {
    return address_v4(0xFFFFFFFF);
  }

  /// Obtain an address object that represents the broadcast address that
  /// corresponds to the specified address and netmask.
  static STDNET_CONSTEXPR address_v4 broadcast(const address_v4& addr,
      const address_v4& mask) STDNET_NOEXCEPT
  {
    return address_v4(addr.to_ulong() | (mask.to_ulong() ^ 0xFFFFFFFF));
  }

private:
  // The underlying IPv4 address.
  bytes_type bytes_;
};

/// Construct an address_v4 from raw bytes.
inline STDNET_CONSTEXPR address_v4 make_address_v4(const address_v4::bytes_type& bytes)
{
  return bytes;
}

/// Construct an address_v4 from a unsigned long in host byte order.
inline STDNET_CONSTEXPR address_v4 make_address_v4(unsigned long addr)
{
  return
#if ULONG_MAX > 0xFFFFFFFF
    (addr > 0xFFFFFFFF)
    ? throw std::out_of_range("address_v4 from unsigned long") :
#endif // ULONG_MAX > 0xFFFFFFFF
    address_v4::bytes_type((addr >> 24) & 0xFF,
      (addr >> 16) & 0xFF, (addr >> 8) & 0xFF, addr & 0xFF);
}

/// Create an address_v4 from an IPv4 address string in dotted decimal form.
STDNET_DECL address_v4 make_address_v4(const char* str);

/// Create an address_v4 from an IPv4 address string in dotted decimal form.
STDNET_DECL address_v4 make_address_v4(const char* str,
    std::error_code& ec) STDNET_NOEXCEPT;

/// Create an address_v4 from an IPv4 address string in dotted decimal form.
STDNET_DECL address_v4 make_address_v4(const std::string& str);

/// Create an address_v4 from an IPv4 address string in dotted decimal form.
STDNET_DECL address_v4 make_address_v4(const std::string& str,
    std::error_code& ec) STDNET_NOEXCEPT;

#if defined(STDNET_HAS_CONSTEXPR)

/// The IPv4 unspecified address.
STDNET_CONSTEXPR address_v4 any_v4(0);

/// The IPv4 loopback address.
STDNET_CONSTEXPR address_v4 loopback_v4(0x7F000001);

/// The IPv4 broadcast address.
STDNET_CONSTEXPR address_v4 broadcast_v4(0xFFFFFFFF);

#else // defined(STDNET_HAS_CONSTEXPR)

static const address_v4 any_v4(0);
static const address_v4 loopback_v4(0x7F000001);
static const address_v4 broadcast_v4(0xFFFFFFFF);

#endif // defined(STDNET_HAS_CONSTEXPR)

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
 * @relates std::experimental::net::ip::address_v4
 */
template <typename Elem, typename Traits>
std::basic_ostream<Elem, Traits>& operator<<(
    std::basic_ostream<Elem, Traits>& os, const address_v4& addr);

#endif // !defined(STDNET_NO_IOSTREAM)

} // namespace ip
} // namespace net
} // namespace experimental
} // namespace std

#include "std/net/detail/pop_options.hpp"

#include "std/net/ip/impl/address_v4.hpp"
#if defined(STDNET_HEADER_ONLY)
# include "std/net/ip/impl/address_v4.ipp"
#endif // defined(STDNET_HEADER_ONLY)

#endif // STDNET_IP_ADDRESS_V4_HPP
