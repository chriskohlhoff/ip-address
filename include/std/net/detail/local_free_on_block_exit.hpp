//
// detail/local_free_on_block_exit.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STDNET_DETAIL_LOCAL_FREE_ON_BLOCK_EXIT_HPP
#define STDNET_DETAIL_LOCAL_FREE_ON_BLOCK_EXIT_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "std/net/detail/config.hpp"

#if defined(STDNET_WINDOWS) || defined(__CYGWIN__)

#include "std/net/detail/socket_types.hpp"

#include "std/net/detail/push_options.hpp"

namespace std {
namespace experimental {
namespace net {
namespace detail {

class local_free_on_block_exit
{
public:
  // Constructor blocks all signals for the calling thread.
  explicit local_free_on_block_exit(void* p)
    : p_(p)
  {
  }

  // Destructor restores the previous signal mask.
  ~local_free_on_block_exit()
  {
    ::LocalFree(p_);
  }

private:
  // Disallow copying and assignemnt.
  local_free_on_block_exit(const local_free_on_block_exit&);
  local_free_on_block_exit& operator=(const local_free_on_block_exit&);

  void* p_;
};

} // namespace detail
} // namespace net
} // namespace experimental
} // namespace std

#include "std/net/detail/pop_options.hpp"

#endif // defined(STDNET_WINDOWS) || defined(__CYGWIN__)

#endif // STDNET_DETAIL_LOCAL_FREE_ON_BLOCK_EXIT_HPP
