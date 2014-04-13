//
// ip/impl/address_v4_conversion.ipp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STDNET_IP_IMPL_ADDRESS_V4_CONVERSION_IPP
#define STDNET_IP_IMPL_ADDRESS_V4_CONVERSION_IPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <typeinfo>
#include "std/net/detail/throw_exception.hpp"
#include "std/net/ip/address_v4.hpp"

#include "std/net/detail/push_options.hpp"

namespace std {
namespace net {
namespace ip {

address_v4::address_v4(const address& addr)
{
  if (addr.type_ != address::ipv4)
  {
    std::bad_cast ex;
    std::net::detail::throw_exception(ex);
  }

  *this = addr.ipv4_address_;
}

address_v4::operator address() const
{
  address addr;
  addr.type_ = address::ipv4;
  addr.ipv4_address_ = *this;
  return addr;
}

} // namespace ip
} // namespace net
} // namespace std

#include "std/net/detail/pop_options.hpp"

#endif // STDNET_IP_IMPL_ADDRESS_V4_CONVERSION_IPP
