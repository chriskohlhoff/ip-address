//
// system_errors.hpp
// ~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_DETAIL_SYSTEM_ERRORS_HPP
#define ASIO_DETAIL_SYSTEM_ERRORS_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/detail/config.hpp"
#include <system_error>
#if defined(ASIO_WINDOWS) || defined(__CYGWIN__)
# include <winerror.h>
#else
# include <cerrno>
#endif

#if defined(GENERATING_DOCUMENTATION)
/// INTERNAL ONLY.
# define ASIO_NATIVE_ERROR(e) implementation_defined
/// INTERNAL ONLY.
# define ASIO_SOCKET_ERROR(e) implementation_defined
/// INTERNAL ONLY.
# define ASIO_NETDB_ERROR(e) implementation_defined
/// INTERNAL ONLY.
# define ASIO_GETADDRINFO_ERROR(e) implementation_defined
/// INTERNAL ONLY.
# define ASIO_WIN_OR_POSIX(e_win, e_posix) implementation_defined
#elif defined(ASIO_WINDOWS) || defined(__CYGWIN__)
# define ASIO_NATIVE_ERROR(e) e
# define ASIO_SOCKET_ERROR(e) WSA ## e
# define ASIO_NETDB_ERROR(e) WSA ## e
# define ASIO_GETADDRINFO_ERROR(e) WSA ## e
# define ASIO_WIN_OR_POSIX(e_win, e_posix) e_win
#else
# define ASIO_NATIVE_ERROR(e) e
# define ASIO_SOCKET_ERROR(e) e
# define ASIO_NETDB_ERROR(e) e
# define ASIO_GETADDRINFO_ERROR(e) e
# define ASIO_WIN_OR_POSIX(e_win, e_posix) e_posix
#endif

#include "asio/detail/push_options.hpp"

namespace asio {
namespace detail {
namespace syserrc {

enum system_errors
{
  /// Permission denied.
  access_denied = ASIO_SOCKET_ERROR(EACCES),

  /// Address family not supported by protocol.
  address_family_not_supported = ASIO_SOCKET_ERROR(EAFNOSUPPORT),

  /// Address already in use.
  address_in_use = ASIO_SOCKET_ERROR(EADDRINUSE),

  /// Transport endpoint is already connected.
  already_connected = ASIO_SOCKET_ERROR(EISCONN),

  /// Operation already in progress.
  already_started = ASIO_SOCKET_ERROR(EALREADY),

  /// Broken pipe.
  broken_pipe = ASIO_WIN_OR_POSIX(
      ASIO_NATIVE_ERROR(ERROR_BROKEN_PIPE),
      ASIO_NATIVE_ERROR(EPIPE)),

  /// A connection has been aborted.
  connection_aborted = ASIO_SOCKET_ERROR(ECONNABORTED),

  /// Connection refused.
  connection_refused = ASIO_SOCKET_ERROR(ECONNREFUSED),

  /// Connection reset by peer.
  connection_reset = ASIO_SOCKET_ERROR(ECONNRESET),

  /// Bad file descriptor.
  bad_descriptor = ASIO_SOCKET_ERROR(EBADF),

  /// Bad address.
  fault = ASIO_SOCKET_ERROR(EFAULT),

  /// No route to host.
  host_unreachable = ASIO_SOCKET_ERROR(EHOSTUNREACH),

  /// Operation now in progress.
  in_progress = ASIO_SOCKET_ERROR(EINPROGRESS),

  /// Interrupted system call.
  interrupted = ASIO_SOCKET_ERROR(EINTR),

  /// Invalid argument.
  invalid_argument = ASIO_SOCKET_ERROR(EINVAL),

  /// Message too long.
  message_size = ASIO_SOCKET_ERROR(EMSGSIZE),

  /// The name was too long.
  name_too_long = ASIO_SOCKET_ERROR(ENAMETOOLONG),

  /// Network is down.
  network_down = ASIO_SOCKET_ERROR(ENETDOWN),

  /// Network dropped connection on reset.
  network_reset = ASIO_SOCKET_ERROR(ENETRESET),

  /// Network is unreachable.
  network_unreachable = ASIO_SOCKET_ERROR(ENETUNREACH),

  /// Too many open files.
  no_descriptors = ASIO_SOCKET_ERROR(EMFILE),

  /// No buffer space available.
  no_buffer_space = ASIO_SOCKET_ERROR(ENOBUFS),

  /// Cannot allocate memory.
  no_memory = ASIO_WIN_OR_POSIX(
      ASIO_NATIVE_ERROR(ERROR_OUTOFMEMORY),
      ASIO_NATIVE_ERROR(ENOMEM)),

  /// Operation not permitted.
  no_permission = ASIO_WIN_OR_POSIX(
      ASIO_NATIVE_ERROR(ERROR_ACCESS_DENIED),
      ASIO_NATIVE_ERROR(EPERM)),

  /// Protocol not available.
  no_protocol_option = ASIO_SOCKET_ERROR(ENOPROTOOPT),

  /// Transport endpoint is not connected.
  not_connected = ASIO_SOCKET_ERROR(ENOTCONN),

  /// Socket operation on non-socket.
  not_socket = ASIO_SOCKET_ERROR(ENOTSOCK),

  /// Operation cancelled.
  operation_aborted = ASIO_WIN_OR_POSIX(
      ASIO_NATIVE_ERROR(ERROR_OPERATION_ABORTED),
      ASIO_NATIVE_ERROR(ECANCELED)),

  /// Operation not supported.
  operation_not_supported = ASIO_SOCKET_ERROR(EOPNOTSUPP),

  /// Cannot send after transport endpoint shutdown.
  shut_down = ASIO_SOCKET_ERROR(ESHUTDOWN),

  /// Connection timed out.
  timed_out = ASIO_SOCKET_ERROR(ETIMEDOUT),

  /// Resource temporarily unavailable.
  try_again = ASIO_WIN_OR_POSIX(
      ASIO_NATIVE_ERROR(ERROR_RETRY),
      ASIO_NATIVE_ERROR(EAGAIN)),

  /// The socket is marked non-blocking and the requested operation would block.
  would_block = ASIO_SOCKET_ERROR(EWOULDBLOCK)
};

} // namespace syserrc

/// Returns the error category used for the system errors produced by asio.
extern ASIO_DECL const std::error_category& system_category();

} // namespace detail
} // namespace asio

namespace std {

template<> struct is_error_code_enum<asio::detail::syserrc::system_errors>
{
  static const bool value = true;
};

} // namespace std

namespace asio {
namespace detail {
namespace syserrc {

inline std::error_code make_error_code(system_errors e)
{
  return std::error_code(
      static_cast<int>(e), asio::detail::system_category());
}

} // namespace syserrc
} // namespace detail
} // namespace asio

#include "asio/detail/pop_options.hpp"

#undef ASIO_NATIVE_ERROR
#undef ASIO_SOCKET_ERROR
#undef ASIO_NETDB_ERROR
#undef ASIO_GETADDRINFO_ERROR
#undef ASIO_WIN_OR_POSIX

#if defined(ASIO_HEADER_ONLY)
# include "asio/detail/impl/system_errors.ipp"
#endif // defined(ASIO_HEADER_ONLY)

#endif // ASIO_DETAIL_SYSTEM_ERRORS_HPP
