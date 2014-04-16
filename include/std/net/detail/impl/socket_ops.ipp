//
// detail/impl/socket_ops.ipp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STDNET_DETAIL_SOCKET_OPS_IPP
#define STDNET_DETAIL_SOCKET_OPS_IPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "std/net/detail/config.hpp"
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <new>
#include "std/net/detail/socket_ops.hpp"
#include "std/net/detail/system_errors.hpp"

#include "std/net/detail/push_options.hpp"

namespace std {
namespace experimental {
namespace net {
namespace detail {
namespace socket_ops {

#if defined(__hpux)
// HP-UX doesn't declare these functions extern "C", so they are declared again
// here to avoid linker errors about undefined symbols.
extern "C" char* if_indextoname(unsigned int, char*);
extern "C" unsigned int if_nametoindex(const char*);
#endif // defined(__hpux)

inline void clear_last_error()
{
#if defined(STDNET_WINDOWS) || defined(__CYGWIN__)
  WSASetLastError(0);
#else
  errno = 0;
#endif
}

template <typename ReturnType>
inline ReturnType error_wrapper(ReturnType return_value,
    std::error_code& ec)
{
#if defined(STDNET_WINDOWS) || defined(__CYGWIN__)
  ec = std::error_code(WSAGetLastError(),
      std::experimental::net::detail::syserrc::system_category());
#else
  ec = std::error_code(errno,
      std::experimental::net::detail::syserrc::system_category());
#endif
  return return_value;
}

const char* inet_ntop(int af, const void* src, char* dest, size_t length,
    unsigned long scope_id, std::error_code& ec)
{
  clear_last_error();
#if defined(STDNET_WINDOWS) || defined(__CYGWIN__)
  using namespace std; // For memcpy.

  if (af != AF_INET && af != AF_INET6)
  {
    ec = std::experimental::net::detail::syserrc::address_family_not_supported;
    return 0;
  }

  union
  {
    socket_addr_type base;
    sockaddr_storage_type storage;
    sockaddr_in4_type v4;
    sockaddr_in6_type v6;
  } address;
  DWORD address_length;
  if (af == AF_INET)
  {
    address_length = sizeof(sockaddr_in4_type);
    address.v4.sin_family = AF_INET;
    address.v4.sin_port = 0;
    memcpy(&address.v4.sin_addr, src, sizeof(in4_addr_type));
  }
  else // AF_INET6
  {
    address_length = sizeof(sockaddr_in6_type);
    address.v6.sin6_family = AF_INET6;
    address.v6.sin6_port = 0;
    address.v6.sin6_flowinfo = 0;
    address.v6.sin6_scope_id = scope_id;
    memcpy(&address.v6.sin6_addr, src, sizeof(in6_addr_type));
  }

  DWORD string_length = static_cast<DWORD>(length);
#if defined(STDNET_NO_ANSI_APIS)
  LPWSTR string_buffer = (LPWSTR)_alloca(length * sizeof(WCHAR));
  int result = error_wrapper(::WSAAddressToStringW(&address.base,
        address_length, 0, string_buffer, &string_length), ec);
  ::WideCharToMultiByte(CP_ACP, 0, string_buffer, -1, dest, length, 0, 0);
#else
  int result = error_wrapper(::WSAAddressToStringA(
        &address.base, address_length, 0, dest, &string_length), ec);
#endif

  // Windows may set error code on success.
  if (result != socket_error_retval)
    ec = std::error_code();

  // Windows may not set an error code on failure.
  else if (result == socket_error_retval && !ec)
    ec = std::experimental::net::detail::syserrc::invalid_argument;

  return result == socket_error_retval ? 0 : dest;
#else // defined(STDNET_WINDOWS) || defined(__CYGWIN__)
  const char* result = error_wrapper(::inet_ntop(
        af, src, dest, static_cast<int>(length)), ec);
  if (result == 0 && !ec)
    ec = std::experimental::net::detail::syserrc::invalid_argument;
  if (result != 0 && af == AF_INET6 && scope_id != 0)
  {
    using namespace std; // For strcat and sprintf.
    char if_name[IF_NAMESIZE + 1] = "%";
    const in6_addr_type* ipv6_address = static_cast<const in6_addr_type*>(src);
    bool is_link_local = ((ipv6_address->s6_addr[0] == 0xfe)
        && ((ipv6_address->s6_addr[1] & 0xc0) == 0x80));
    if (!is_link_local
        || if_indextoname(static_cast<unsigned>(scope_id), if_name + 1) == 0)
      sprintf(if_name + 1, "%lu", scope_id);
    strcat(dest, if_name);
  }
  return result;
#endif // defined(STDNET_WINDOWS) || defined(__CYGWIN__)
}

int inet_pton(int af, const char* src, void* dest,
    unsigned long* scope_id, std::error_code& ec)
{
  clear_last_error();
#if defined(STDNET_WINDOWS) || defined(__CYGWIN__)
  using namespace std; // For memcpy and strcmp.

  if (af != AF_INET && af != AF_INET6)
  {
    ec = std::experimental::net::detail::syserrc::address_family_not_supported;
    return -1;
  }

  union
  {
    socket_addr_type base;
    sockaddr_storage_type storage;
    sockaddr_in4_type v4;
    sockaddr_in6_type v6;
  } address;
  int address_length = sizeof(sockaddr_storage_type);
#if defined(STDNET_NO_ANSI_APIS)
  int num_wide_chars = strlen(src) + 1;
  LPWSTR wide_buffer = (LPWSTR)_alloca(num_wide_chars * sizeof(WCHAR));
  ::MultiByteToWideChar(CP_ACP, 0, src, -1, wide_buffer, num_wide_chars);
  int result = error_wrapper(::WSAStringToAddressW(
        wide_buffer, af, 0, &address.base, &address_length), ec);
#else
  int result = error_wrapper(::WSAStringToAddressA(
        const_cast<char*>(src), af, 0, &address.base, &address_length), ec);
#endif

  if (af == AF_INET)
  {
    if (result != socket_error_retval)
    {
      memcpy(dest, &address.v4.sin_addr, sizeof(in4_addr_type));
      ec = std::error_code();
    }
    else if (strcmp(src, "255.255.255.255") == 0)
    {
      static_cast<in4_addr_type*>(dest)->s_addr = INADDR_NONE;
      ec = std::error_code();
    }
  }
  else // AF_INET6
  {
    if (result != socket_error_retval)
    {
      memcpy(dest, &address.v6.sin6_addr, sizeof(in6_addr_type));
      if (scope_id)
        *scope_id = address.v6.sin6_scope_id;
      ec = std::error_code();
    }
  }

  // Windows may not set an error code on failure.
  if (result == socket_error_retval && !ec)
    ec = std::experimental::net::detail::syserrc::invalid_argument;

  if (result != socket_error_retval)
    ec = std::error_code();

  return result == socket_error_retval ? -1 : 1;
#else // defined(STDNET_WINDOWS) || defined(__CYGWIN__)
  int result = error_wrapper(::inet_pton(af, src, dest), ec);
  if (result <= 0 && !ec)
    ec = std::experimental::net::detail::syserrc::invalid_argument;
  if (result > 0 && af == AF_INET6 && scope_id)
  {
    using namespace std; // For strchr and atoi.
    *scope_id = 0;
    if (const char* if_name = strchr(src, '%'))
    {
      in6_addr_type* ipv6_address = static_cast<in6_addr_type*>(dest);
      bool is_link_local = ((ipv6_address->s6_addr[0] == 0xfe)
          && ((ipv6_address->s6_addr[1] & 0xc0) == 0x80));
      if (is_link_local)
        *scope_id = if_nametoindex(if_name + 1);
      if (*scope_id == 0)
        *scope_id = atoi(if_name + 1);
    }
  }
  return result;
#endif // defined(STDNET_WINDOWS) || defined(__CYGWIN__)
}

u_long_type network_to_host_long(u_long_type value)
{
  return ntohl(value);
}

u_long_type host_to_network_long(u_long_type value)
{
  return htonl(value);
}

u_short_type network_to_host_short(u_short_type value)
{
  return ntohs(value);
}

u_short_type host_to_network_short(u_short_type value)
{
  return htons(value);
}

} // namespace socket_ops
} // namespace detail
} // namespace net
} // namespace experimental
} // namespace std

#include "std/net/detail/pop_options.hpp"

#endif // STDNET_DETAIL_SOCKET_OPS_IPP
