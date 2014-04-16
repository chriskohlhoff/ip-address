//
// system_errors.hpp
// ~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STDNET_DETAIL_SYSTEM_ERRORS_HPP
#define STDNET_DETAIL_SYSTEM_ERRORS_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "std/net/detail/config.hpp"
#include <system_error>
#if defined(STDNET_WINDOWS) || defined(__CYGWIN__)
# include <winerror.h>
#else
# include <cerrno>
#endif

#if defined(GENERATING_DOCUMENTATION)
/// INTERNAL ONLY.
# define STDNET_NATIVE_ERROR(e) implementation_defined
/// INTERNAL ONLY.
# define STDNET_SOCKET_ERROR(e) implementation_defined
/// INTERNAL ONLY.
# define STDNET_NETDB_ERROR(e) implementation_defined
/// INTERNAL ONLY.
# define STDNET_GETADDRINFO_ERROR(e) implementation_defined
/// INTERNAL ONLY.
# define STDNET_WIN_OR_POSIX(e_win, e_posix) implementation_defined
#elif defined(STDNET_WINDOWS) || defined(__CYGWIN__)
# define STDNET_NATIVE_ERROR(e) e
# define STDNET_SOCKET_ERROR(e) WSA ## e
# define STDNET_NETDB_ERROR(e) WSA ## e
# define STDNET_GETADDRINFO_ERROR(e) WSA ## e
# define STDNET_WIN_OR_POSIX(e_win, e_posix) e_win
#else
# define STDNET_NATIVE_ERROR(e) e
# define STDNET_SOCKET_ERROR(e) e
# define STDNET_NETDB_ERROR(e) e
# define STDNET_GETADDRINFO_ERROR(e) e
# define STDNET_WIN_OR_POSIX(e_win, e_posix) e_posix
#endif

#include "std/net/detail/push_options.hpp"

namespace std {
namespace experimental {
namespace net {
namespace detail {
namespace syserrc {

enum system_errors
{
  /// Permission denied.
  access_denied = STDNET_SOCKET_ERROR(EACCES),

  /// Address family not supported by protocol.
  address_family_not_supported = STDNET_SOCKET_ERROR(EAFNOSUPPORT),

  /// Address already in use.
  address_in_use = STDNET_SOCKET_ERROR(EADDRINUSE),

  /// Transport endpoint is already connected.
  already_connected = STDNET_SOCKET_ERROR(EISCONN),

  /// Operation already in progress.
  already_started = STDNET_SOCKET_ERROR(EALREADY),

  /// Broken pipe.
  broken_pipe = STDNET_WIN_OR_POSIX(
      STDNET_NATIVE_ERROR(ERROR_BROKEN_PIPE),
      STDNET_NATIVE_ERROR(EPIPE)),

  /// A connection has been aborted.
  connection_aborted = STDNET_SOCKET_ERROR(ECONNABORTED),

  /// Connection refused.
  connection_refused = STDNET_SOCKET_ERROR(ECONNREFUSED),

  /// Connection reset by peer.
  connection_reset = STDNET_SOCKET_ERROR(ECONNRESET),

  /// Bad file descriptor.
  bad_descriptor = STDNET_SOCKET_ERROR(EBADF),

  /// Bad address.
  fault = STDNET_SOCKET_ERROR(EFAULT),

  /// No route to host.
  host_unreachable = STDNET_SOCKET_ERROR(EHOSTUNREACH),

  /// Operation now in progress.
  in_progress = STDNET_SOCKET_ERROR(EINPROGRESS),

  /// Interrupted system call.
  interrupted = STDNET_SOCKET_ERROR(EINTR),

  /// Invalid argument.
  invalid_argument = STDNET_SOCKET_ERROR(EINVAL),

  /// Message too long.
  message_size = STDNET_SOCKET_ERROR(EMSGSIZE),

  /// The name was too long.
  name_too_long = STDNET_SOCKET_ERROR(ENAMETOOLONG),

  /// Network is down.
  network_down = STDNET_SOCKET_ERROR(ENETDOWN),

  /// Network dropped connection on reset.
  network_reset = STDNET_SOCKET_ERROR(ENETRESET),

  /// Network is unreachable.
  network_unreachable = STDNET_SOCKET_ERROR(ENETUNREACH),

  /// Too many open files.
  no_descriptors = STDNET_SOCKET_ERROR(EMFILE),

  /// No buffer space available.
  no_buffer_space = STDNET_SOCKET_ERROR(ENOBUFS),

  /// Cannot allocate memory.
  no_memory = STDNET_WIN_OR_POSIX(
      STDNET_NATIVE_ERROR(ERROR_OUTOFMEMORY),
      STDNET_NATIVE_ERROR(ENOMEM)),

  /// Operation not permitted.
  no_permission = STDNET_WIN_OR_POSIX(
      STDNET_NATIVE_ERROR(ERROR_ACCESS_DENIED),
      STDNET_NATIVE_ERROR(EPERM)),

  /// Protocol not available.
  no_protocol_option = STDNET_SOCKET_ERROR(ENOPROTOOPT),

  /// Transport endpoint is not connected.
  not_connected = STDNET_SOCKET_ERROR(ENOTCONN),

  /// Socket operation on non-socket.
  not_socket = STDNET_SOCKET_ERROR(ENOTSOCK),

  /// Operation cancelled.
  operation_aborted = STDNET_WIN_OR_POSIX(
      STDNET_NATIVE_ERROR(ERROR_OPERATION_ABORTED),
      STDNET_NATIVE_ERROR(ECANCELED)),

  /// Operation not supported.
  operation_not_supported = STDNET_SOCKET_ERROR(EOPNOTSUPP),

  /// Cannot send after transport endpoint shutdown.
  shut_down = STDNET_SOCKET_ERROR(ESHUTDOWN),

  /// Connection timed out.
  timed_out = STDNET_SOCKET_ERROR(ETIMEDOUT),

  /// Resource temporarily unavailable.
  try_again = STDNET_WIN_OR_POSIX(
      STDNET_NATIVE_ERROR(ERROR_RETRY),
      STDNET_NATIVE_ERROR(EAGAIN)),

  /// The socket is marked non-blocking and the requested operation would block.
  would_block = STDNET_SOCKET_ERROR(EWOULDBLOCK)
};

} // namespace syserrc

/// Returns the error category used for the system errors.
extern STDNET_DECL const std::error_category& system_category();

} // namespace detail
} // namespace net
} // namespace experimental
} // namespace std

namespace std {

template<> struct is_error_code_enum<
  std::experimental::net::detail::syserrc::system_errors>
{
  static const bool value = true;
};

} // namespace std

namespace std {
namespace experimental {
namespace net {
namespace detail {
namespace syserrc {

inline std::error_code make_error_code(system_errors e)
{
  return std::error_code(static_cast<int>(e),
      std::experimental::net::detail::system_category());
}

} // namespace syserrc
} // namespace detail
} // namespace net
} // namespace experimental
} // namespace std

#include "std/net/detail/pop_options.hpp"

#undef STDNET_NATIVE_ERROR
#undef STDNET_SOCKET_ERROR
#undef STDNET_NETDB_ERROR
#undef STDNET_GETADDRINFO_ERROR
#undef STDNET_WIN_OR_POSIX

#if defined(STDNET_HEADER_ONLY)
# include "std/net/detail/impl/system_errors.ipp"
#endif // defined(STDNET_HEADER_ONLY)

#endif // STDNET_DETAIL_SYSTEM_ERRORS_HPP
