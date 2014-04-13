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
#include "std/net/detail/socket_types.hpp"
#include "std/net/detail/winsock_init.hpp"

#if !defined(STDNET_NO_IOSTREAM)
# include <iosfwd>
#endif // !defined(STDNET_NO_IOSTREAM)

#include "std/net/detail/push_options.hpp"

namespace std {
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
  /// The type used to represent an address as an array of bytes.
  typedef std::array<unsigned char, 4> bytes_type;

  /// Default constructor.
  address_v4() STDNET_NOEXCEPT
  {
    addr_.s_addr = 0;
  }

  /// Construct an address from raw bytes.
  STDNET_DECL explicit address_v4(const bytes_type& bytes);

  /// Construct an address from a unsigned long in host byte order.
  STDNET_DECL explicit address_v4(unsigned long addr);

  /// Explicitly convert from a version-independent address.
  /**
   * @throws bad_cast if addr is not an IPv4 address.
   */
  STDNET_DECL explicit address_v4(const address& addr);

  /// Copy constructor.
  address_v4(const address_v4& other) STDNET_NOEXCEPT
    : addr_(other.addr_)
  {
  }

#if defined(STDNET_HAS_MOVE)
  /// Move constructor.
  address_v4(address_v4&& other) STDNET_NOEXCEPT
    : addr_(other.addr_)
  {
  }
#endif // defined(STDNET_HAS_MOVE)

  /// Assign from another address.
  address_v4& operator=(const address_v4& other) STDNET_NOEXCEPT
  {
    addr_ = other.addr_;
    return *this;
  }

#if defined(STDNET_HAS_MOVE)
  /// Move-assign from another address.
  address_v4& operator=(address_v4&& other) STDNET_NOEXCEPT
  {
    addr_ = other.addr_;
    return *this;
  }
#endif // defined(STDNET_HAS_MOVE)

  /// Implicitly convert to a version-independent address.
  STDNET_DECL operator address() const;

  /// Get the address in bytes, in network byte order.
  STDNET_DECL bytes_type to_bytes() const STDNET_NOEXCEPT;

  /// Get the address as an unsigned long in host byte order
  STDNET_DECL unsigned long to_ulong() const STDNET_NOEXCEPT;

  /// Get the address as a string in dotted decimal format.
  STDNET_DECL std::string to_string() const;

  /// Get the address as a string in dotted decimal format.
  STDNET_DECL std::string to_string(std::error_code& ec) const;

  /// Create an address from an IP address string in dotted decimal form.
  STDNET_DECL static address_v4 from_string(const char* str);

  /// Create an address from an IP address string in dotted decimal form.
  STDNET_DECL static address_v4 from_string(
      const char* str, std::error_code& ec);

  /// Create an address from an IP address string in dotted decimal form.
  STDNET_DECL static address_v4 from_string(const std::string& str);

  /// Create an address from an IP address string in dotted decimal form.
  STDNET_DECL static address_v4 from_string(
      const std::string& str, std::error_code& ec);

  /// Determine whether the address is a loopback address.
  STDNET_DECL bool is_loopback() const STDNET_NOEXCEPT;

  /// Determine whether the address is unspecified.
  STDNET_DECL bool is_unspecified() const STDNET_NOEXCEPT;

  /// Determine whether the address is a class A address.
  STDNET_DECL bool is_class_a() const STDNET_NOEXCEPT;

  /// Determine whether the address is a class B address.
  STDNET_DECL bool is_class_b() const STDNET_NOEXCEPT;

  /// Determine whether the address is a class C address.
  STDNET_DECL bool is_class_c() const STDNET_NOEXCEPT;

  /// Determine whether the address is a multicast address.
  STDNET_DECL bool is_multicast() const STDNET_NOEXCEPT;

  /// Compare two addresses for equality.
  friend bool operator==(const address_v4& a1,
      const address_v4& a2) STDNET_NOEXCEPT
  {
    return a1.addr_.s_addr == a2.addr_.s_addr;
  }

  /// Compare two addresses for inequality.
  friend bool operator!=(const address_v4& a1,
      const address_v4& a2) STDNET_NOEXCEPT
  {
    return a1.addr_.s_addr != a2.addr_.s_addr;
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
  static address_v4 any() STDNET_NOEXCEPT
  {
    return address_v4();
  }

  /// Obtain an address object that represents the loopback address.
  static address_v4 loopback() STDNET_NOEXCEPT
  {
    return address_v4(0x7F000001);
  }

  /// Obtain an address object that represents the broadcast address.
  static address_v4 broadcast() STDNET_NOEXCEPT
  {
    return address_v4(0xFFFFFFFF);
  }

  /// Obtain an address object that represents the broadcast address that
  /// corresponds to the specified address and netmask.
  STDNET_DECL static address_v4 broadcast(const address_v4& addr,
      const address_v4& mask) STDNET_NOEXCEPT;

  /// Obtain the netmask that corresponds to the address, based on its address
  /// class.
  STDNET_DECL static address_v4 netmask(
      const address_v4& addr) STDNET_NOEXCEPT;

private:
  // The underlying IPv4 address.
  std::net::detail::in4_addr_type addr_;
};

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
 * @relates std::net::ip::address_v4
 */
template <typename Elem, typename Traits>
std::basic_ostream<Elem, Traits>& operator<<(
    std::basic_ostream<Elem, Traits>& os, const address_v4& addr);

#endif // !defined(STDNET_NO_IOSTREAM)

} // namespace ip
} // namespace net
} // namespace std

#include "std/net/detail/pop_options.hpp"

#include "std/net/ip/impl/address_v4.hpp"
#if defined(STDNET_HEADER_ONLY)
# include "std/net/ip/impl/address_v4.ipp"
#endif // defined(STDNET_HEADER_ONLY)

#include "std/net/ip/address.hpp"

#endif // STDNET_IP_ADDRESS_V4_HPP
