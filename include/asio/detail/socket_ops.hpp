//
// detail/socket_ops.hpp
// ~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_DETAIL_SOCKET_OPS_HPP
#define ASIO_DETAIL_SOCKET_OPS_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/detail/config.hpp"

#include <system_error>
#include "asio/detail/socket_types.hpp"

#include "asio/detail/push_options.hpp"

namespace asio {
namespace detail {
namespace socket_ops {

ASIO_DECL const char* inet_ntop(int af, const void* src, char* dest,
    size_t length, unsigned long scope_id, std::error_code& ec);

ASIO_DECL int inet_pton(int af, const char* src, void* dest,
    unsigned long* scope_id, std::error_code& ec);

ASIO_DECL u_long_type network_to_host_long(u_long_type value);

ASIO_DECL u_long_type host_to_network_long(u_long_type value);

ASIO_DECL u_short_type network_to_host_short(u_short_type value);

ASIO_DECL u_short_type host_to_network_short(u_short_type value);

} // namespace socket_ops
} // namespace detail
} // namespace asio

#include "asio/detail/pop_options.hpp"

#if defined(ASIO_HEADER_ONLY)
# include "asio/detail/impl/socket_ops.ipp"
#endif // defined(ASIO_HEADER_ONLY)

#endif // ASIO_DETAIL_SOCKET_OPS_HPP
