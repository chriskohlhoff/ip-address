//
// detail/config.hpp
// ~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STDNET_DETAIL_CONFIG_HPP
#define STDNET_DETAIL_CONFIG_HPP

// Default to a header-only implementation. The user must specifically request
// separate compilation by defining either STDNET_SEPARATE_COMPILATION or
// STDNET_DYN_LINK (as a DLL/shared library implies separate compilation).
#if !defined(STDNET_HEADER_ONLY)
# if !defined(STDNET_SEPARATE_COMPILATION)
#  if !defined(STDNET_DYN_LINK)
#   define STDNET_HEADER_ONLY 1
#  endif // !defined(STDNET_DYN_LINK)
# endif // !defined(STDNET_SEPARATE_COMPILATION)
#endif // !defined(STDNET_HEADER_ONLY)

#if defined(STDNET_HEADER_ONLY)
# define STDNET_DECL inline
#else // defined(STDNET_HEADER_ONLY)
# if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__CODEGEARC__)
// We need to import/export our code only if the user has specifically asked
// for it by defining STDNET_DYN_LINK.
#  if defined(STDNET_DYN_LINK)
// Export if this is our own source, otherwise import.
#   if defined(STDNET_SOURCE)
#    define STDNET_DECL __declspec(dllexport)
#   else // defined(STDNET_SOURCE)
#    define STDNET_DECL __declspec(dllimport)
#   endif // defined(STDNET_SOURCE)
#  endif // defined(STDNET_DYN_LINK)
# endif // defined(_MSC_VER) || defined(__BORLANDC__) || defined(__CODEGEARC__)
#endif // defined(STDNET_HEADER_ONLY)

// If STDNET_DECL isn't defined yet define it now.
#if !defined(STDNET_DECL)
# define STDNET_DECL
#endif // !defined(STDNET_DECL)

// Microsoft Visual C++ detection.
#if !defined(STDNET_MSVC)
# if defined(_MSC_VER) && !defined(__MWERKS__) && !defined(__EDG_VERSION__)
#  define STDNET_MSVC _MSC_VER
# endif // defined(_MSC_VER) && !defined(__MWERKS__) && !defined(__EDG_VERSION__)
#endif // defined(STDNET_MSVC)

// Support move construction and assignment on compilers known to allow it.
#if !defined(STDNET_HAS_MOVE)
# if !defined(STDNET_DISABLE_MOVE)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define STDNET_HAS_MOVE 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
# endif // !defined(STDNET_DISABLE_MOVE)
#endif // !defined(STDNET_HAS_MOVE)

// If STDNET_MOVE_CAST isn't defined, and move support is available, define
// STDNET_MOVE_ARG and STDNET_MOVE_CAST to take advantage of rvalue
// references and perfect forwarding.
#if defined(STDNET_HAS_MOVE) && !defined(STDNET_MOVE_CAST)
# define STDNET_MOVE_ARG(type) type&&
# define STDNET_MOVE_CAST(type) static_cast<type&&>
#endif // defined(STDNET_HAS_MOVE) && !defined(STDNET_MOVE_CAST)

// If STDNET_MOVE_CAST still isn't defined, default to a C++03-compatible
// implementation. Note that older g++ and MSVC versions don't like it when you
// pass a non-member function through a const reference, so for most compilers
// we'll play it safe and stick with the old approach of passing the handler by
// value.
#if !defined(STDNET_MOVE_CAST)
# if defined(__GNUC__)
#  if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 1)) || (__GNUC__ > 4)
#   define STDNET_MOVE_ARG(type) const type&
#  else // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 1)) || (__GNUC__ > 4)
#   define STDNET_MOVE_ARG(type) type
#  endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 1)) || (__GNUC__ > 4)
# elif defined(STDNET_MSVC)
#  if (_MSC_VER >= 1400)
#   define STDNET_MOVE_ARG(type) const type&
#  else // (_MSC_VER >= 1400)
#   define STDNET_MOVE_ARG(type) type
#  endif // (_MSC_VER >= 1400)
# else
#  define STDNET_MOVE_ARG(type) type
# endif
# define STDNET_MOVE_CAST(type) static_cast<const type&>
#endif // !defined_STDNET_MOVE_CAST

// Support variadic templates on compilers known to allow it.
#if !defined(STDNET_HAS_VARIADIC_TEMPLATES)
# if !defined(STDNET_DISABLE_VARIADIC_TEMPLATES)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 3)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define STDNET_HAS_VARIADIC_TEMPLATES 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 3)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
# if defined(__clang__)
#  if __has_feature(cxx_variadic_templates)
#   define STDNET_HAS_VARIADIC_TEMPLATES 1
#  endif // __has_feature(cxx_noexcept)
# endif // defined(__clang__)
# endif // !defined(STDNET_DISABLE_VARIADIC_TEMPLATES)
#endif // !defined(STDNET_HAS_VARIADIC_TEMPLATES)

// Support the noexcept specifier on compilers known to allow it.
#if !defined(STDNET_NOEXCEPT)
# if defined(__GNUC__)
#  if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
#   if defined(__GXX_EXPERIMENTAL_CXX0X__)
#    define STDNET_NOEXCEPT noexcept
#   endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#  endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
# endif // defined(__GNUC__)
# if defined(__clang__)
#  if __has_feature(cxx_noexcept)
#   define STDNET_NOEXCEPT noexcept
#  endif // __has_feature(cxx_noexcept)
# endif // defined(__clang__)
# if !defined(STDNET_NOEXCEPT)
#  define STDNET_NOEXCEPT
# endif // !defined(STDNET_NOEXCEPT)
#endif // !defined(STDNET_NOEXCEPT)

// Support deleted functions on compilers known to allow it.
#if !defined(STDNET_DELETED)
# if defined(__GNUC__)
#  if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
#   if defined(__GXX_EXPERIMENTAL_CXX0X__)
#    define STDNET_DELETED = delete
#   endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#  endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
# endif // defined(__GNUC__)
# if defined(__clang__)
#  if __has_feature(cxx_deleted_functions)
#   define STDNET_DELETED = delete
#  endif // __has_feature(cxx_noexcept)
# endif // defined(__clang__)
# if !defined(STDNET_DELETED)
#  define STDNET_DELETED
# endif // !defined(STDNET_DELETED)
#endif // !defined(STDNET_DELETED)

// Support relaxed constexpr on compilers known to allow it.
#if !defined(STDNET_CONSTEXPR)
# if defined(__GNUC__)
#  if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
#   if defined(__GXX_EXPERIMENTAL_CXX0X__)
#    define STDNET_HAS_CONSTEXPR 1
#    define STDNET_CONSTEXPR constexpr
#   endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#  endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
# endif // defined(__GNUC__)
# if defined(__clang__)
#  if __has_feature(cxx_constexpr)
#   define STDNET_HAS_CONSTEXPR 1
#   define STDNET_CONSTEXPR constexpr
#  endif // __has_feature(cxx_constexpr)
# endif // defined(__clang__)
# if !defined(STDNET_CONSTEXPR)
#  define STDNET_CONSTEXPR
# endif // !defined(STDNET_CONSTEXPR)
#endif // !defined(STDNET_CONSTEXPR)

// Standard library support for system errors.
#if !defined(STDNET_HAS_STD_SYSTEM_ERROR)
# if !defined(STDNET_DISABLE_STD_SYSTEM_ERROR)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define STDNET_HAS_STD_SYSTEM_ERROR 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
# endif // !defined(STDNET_DISABLE_STD_SYSTEM_ERROR)
#endif // !defined(STDNET_HAS_STD_SYSTEM_ERROR)

// Compliant C++11 compilers put noexcept specifiers on error_category members.
#if !defined(STDNET_ERROR_CATEGORY_NOEXCEPT)
# if defined(__GNUC__)
#  if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
#   if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define STDNET_ERROR_CATEGORY_NOEXCEPT noexcept(true)
#   endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#  endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
# endif // defined(__GNUC__)
# if defined(__clang__)
#  if __has_feature(cxx_noexcept)
#    define STDNET_ERROR_CATEGORY_NOEXCEPT noexcept(true)
#  endif // __has_feature(cxx_noexcept)
# endif // defined(__clang__)
# if !defined(STDNET_ERROR_CATEGORY_NOEXCEPT)
#  define STDNET_ERROR_CATEGORY_NOEXCEPT
# endif // !defined(STDNET_ERROR_CATEGORY_NOEXCEPT)
#endif // !defined(STDNET_ERROR_CATEGORY_NOEXCEPT)

// Standard library support for arrays.
#if !defined(STDNET_HAS_STD_ARRAY)
# if !defined(STDNET_DISABLE_STD_ARRAY)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 3)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define STDNET_HAS_STD_ARRAY 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 3)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(STDNET_MSVC)
#   if (_MSC_VER >= 1600)
#    define STDNET_HAS_STD_ARRAY 1
#   endif // (_MSC_VER >= 1600)
#  endif // defined(STDNET_MSVC)
# endif // !defined(STDNET_DISABLE_STD_ARRAY)
#endif // !defined(STDNET_HAS_STD_ARRAY)

// Standard library support for shared_ptr and weak_ptr.
#if !defined(STDNET_HAS_STD_SHARED_PTR)
# if !defined(STDNET_DISABLE_STD_SHARED_PTR)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 3)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define STDNET_HAS_STD_SHARED_PTR 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 3)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(STDNET_MSVC)
#   if (_MSC_VER >= 1600)
#    define STDNET_HAS_STD_SHARED_PTR 1
#   endif // (_MSC_VER >= 1600)
#  endif // defined(STDNET_MSVC)
# endif // !defined(STDNET_DISABLE_STD_SHARED_PTR)
#endif // !defined(STDNET_HAS_STD_SHARED_PTR)

// Standard library support for atomic operations.
#if !defined(STDNET_HAS_STD_ATOMIC)
# if !defined(STDNET_DISABLE_STD_ATOMIC)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define STDNET_HAS_STD_ATOMIC 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
# endif // !defined(STDNET_DISABLE_STD_ATOMIC)
#endif // !defined(STDNET_HAS_STD_ATOMIC)

// Standard library support for chrono. Some standard libraries (such as the
// libstdc++ shipped with gcc 4.6) provide monotonic_clock as per early C++0x
// drafts, rather than the eventually standardised name of steady_clock.
#if !defined(STDNET_HAS_STD_CHRONO)
# if !defined(STDNET_DISABLE_STD_CHRONO)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define STDNET_HAS_STD_CHRONO 1
#     if ((__GNUC__ == 4) && (__GNUC_MINOR__ == 6))
#      define STDNET_HAS_STD_CHRONO_MONOTONIC_CLOCK 1
#     endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ == 6))
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
# endif // !defined(STDNET_DISABLE_STD_CHRONO)
#endif // !defined(STDNET_HAS_STD_CHRONO)

// Standard library support for addressof.
#if !defined(STDNET_HAS_STD_ADDRESSOF)
# if !defined(STDNET_DISABLE_STD_ADDRESSOF)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define STDNET_HAS_STD_ADDRESSOF 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
# endif // !defined(STDNET_DISABLE_STD_ADDRESSOF)
#endif // !defined(STDNET_HAS_STD_ADDRESSOF)

// Standard library support for the function class.
#if !defined(STDNET_HAS_STD_FUNCTION)
# if !defined(STDNET_DISABLE_STD_FUNCTION)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define STDNET_HAS_STD_FUNCTION 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
# endif // !defined(STDNET_DISABLE_STD_FUNCTION)
#endif // !defined(STDNET_HAS_STD_FUNCTION)

// Standard library support for type traits.
#if !defined(STDNET_HAS_STD_TYPE_TRAITS)
# if !defined(STDNET_DISABLE_STD_TYPE_TRAITS)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define STDNET_HAS_STD_TYPE_TRAITS 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
# endif // !defined(STDNET_DISABLE_STD_TYPE_TRAITS)
#endif // !defined(STDNET_HAS_STD_TYPE_TRAITS)

// Standard library support for the cstdint header.
#if !defined(STDNET_HAS_CSTDINT)
# if !defined(STDNET_DISABLE_CSTDINT)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define STDNET_HAS_CSTDINT 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
# endif // !defined(STDNET_DISABLE_CSTDINT)
#endif // !defined(STDNET_HAS_CSTDINT)

// Windows target.
#if !defined(STDNET_WINDOWS)
# if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#  define STDNET_WINDOWS 1
# endif // defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#endif // !defined(STDNET_WINDOWS)

// Windows: target OS version.
#if defined(STDNET_WINDOWS) || defined(__CYGWIN__)
# if !defined(_WIN32_WINNT) && !defined(_WIN32_WINDOWS)
#  if defined(_MSC_VER) || defined(__BORLANDC__)
#   pragma message( \
  "Please define _WIN32_WINNT or _WIN32_WINDOWS appropriately. For example:\n"\
  "- add -D_WIN32_WINNT=0x0501 to the compiler command line; or\n"\
  "- add _WIN32_WINNT=0x0501 to your project's Preprocessor Definitions.\n"\
  "Assuming _WIN32_WINNT=0x0501 (i.e. Windows XP target).")
#  else // defined(_MSC_VER) || defined(__BORLANDC__)
#   warning Please define _WIN32_WINNT or _WIN32_WINDOWS appropriately.
#   warning For example, add -D_WIN32_WINNT=0x0501 to the compiler command line.
#   warning Assuming _WIN32_WINNT=0x0501 (i.e. Windows XP target).
#  endif // defined(_MSC_VER) || defined(__BORLANDC__)
#  define _WIN32_WINNT 0x0501
# endif // !defined(_WIN32_WINNT) && !defined(_WIN32_WINDOWS)
# if defined(_MSC_VER)
#  if defined(_WIN32) && !defined(WIN32)
#   if !defined(_WINSOCK2API_)
#    define WIN32 // Needed for correct types in winsock2.h
#   else // !defined(_WINSOCK2API_)
#    error Please define the macro WIN32 in your compiler options
#   endif // !defined(_WINSOCK2API_)
#  endif // defined(_WIN32) && !defined(WIN32)
# endif // defined(_MSC_VER)
# if defined(__BORLANDC__)
#  if defined(__WIN32__) && !defined(WIN32)
#   if !defined(_WINSOCK2API_)
#    define WIN32 // Needed for correct types in winsock2.h
#   else // !defined(_WINSOCK2API_)
#    error Please define the macro WIN32 in your compiler options
#   endif // !defined(_WINSOCK2API_)
#  endif // defined(__WIN32__) && !defined(WIN32)
# endif // defined(__BORLANDC__)
# if defined(__CYGWIN__)
#  if !defined(__USE_W32_SOCKETS)
#   error You must add -D__USE_W32_SOCKETS to your compiler options.
#  endif // !defined(__USE_W32_SOCKETS)
# endif // defined(__CYGWIN__)
#endif // defined(STDNET_WINDOWS) || defined(__CYGWIN__)

// Windows: minimise header inclusion.
#if defined(STDNET_WINDOWS) || defined(__CYGWIN__)
# if !defined(STDNET_NO_WIN32_LEAN_AND_MEAN)
#  if !defined(WIN32_LEAN_AND_MEAN)
#   define WIN32_LEAN_AND_MEAN
#  endif // !defined(WIN32_LEAN_AND_MEAN)
# endif // !defined(STDNET_NO_WIN32_LEAN_AND_MEAN)
#endif // defined(STDNET_WINDOWS) || defined(__CYGWIN__)

// Windows: suppress definition of "min" and "max" macros.
#if defined(STDNET_WINDOWS) || defined(__CYGWIN__)
# if !defined(STDNET_NO_NOMINMAX)
#  if !defined(NOMINMAX)
#   define NOMINMAX 1
#  endif // !defined(NOMINMAX)
# endif // !defined(STDNET_NO_NOMINMAX)
#endif // defined(STDNET_WINDOWS) || defined(__CYGWIN__)

// Windows: No ANSI API calls.
#if !defined(STDNET_NO_ANSI_APIS)
# if defined(STDNET_WINDOWS) && defined(UNDER_CE)
#  define STDNET_NO_ANSI_APIS 1
# endif // defined(STDNET_WINDOWS) && defined(UNDER_CE)
#endif // !defined(STDNET_NO_ANSI_APIS)

// Windows: IO Completion Ports.
#if !defined(STDNET_HAS_IOCP)
# if defined(STDNET_WINDOWS) || defined(__CYGWIN__)
#  if defined(_WIN32_WINNT) && (_WIN32_WINNT >= 0x0400)
#   if !defined(UNDER_CE)
#    if !defined(STDNET_DISABLE_IOCP)
#     define STDNET_HAS_IOCP 1
#    endif // !defined(STDNET_DISABLE_IOCP)
#   endif // !defined(UNDER_CE)
#  endif // defined(_WIN32_WINNT) && (_WIN32_WINNT >= 0x0400)
# endif // defined(STDNET_WINDOWS) || defined(__CYGWIN__)
#endif // !defined(STDNET_HAS_IOCP)

// Linux: epoll, eventfd and timerfd.
#if defined(__linux__)
# include <linux/version.h>
# if !defined(STDNET_HAS_EPOLL)
#  if !defined(STDNET_DISABLE_EPOLL)
#   if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,45)
#    define STDNET_HAS_EPOLL 1
#   endif // LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,45)
#  endif // !defined(STDNET_DISABLE_EPOLL)
# endif // !defined(STDNET_HAS_EPOLL)
# if !defined(STDNET_HAS_EVENTFD)
#  if !defined(STDNET_DISABLE_EVENTFD)
#   if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,22)
#    define STDNET_HAS_EVENTFD 1
#   endif // LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,22)
#  endif // !defined(STDNET_DISABLE_EVENTFD)
# endif // !defined(STDNET_HAS_EVENTFD)
# if !defined(STDNET_HAS_TIMERFD)
#  if defined(STDNET_HAS_EPOLL)
#   if (__GLIBC__ > 2) || (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 8)
#    define STDNET_HAS_TIMERFD 1
#   endif // (__GLIBC__ > 2) || (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 8)
#  endif // defined(STDNET_HAS_EPOLL)
# endif // !defined(STDNET_HAS_TIMERFD)
#endif // defined(__linux__)

// Mac OS X, FreeBSD, NetBSD, OpenBSD: kqueue.
#if (defined(__MACH__) && defined(__APPLE__)) \
  || defined(__FreeBSD__) \
  || defined(__NetBSD__) \
  || defined(__OpenBSD__)
# if !defined(STDNET_HAS_KQUEUE)
#  if !defined(STDNET_DISABLE_KQUEUE)
#   define STDNET_HAS_KQUEUE 1
#  endif // !defined(STDNET_DISABLE_KQUEUE)
# endif // !defined(STDNET_HAS_KQUEUE)
#endif // (defined(__MACH__) && defined(__APPLE__))
       //   || defined(__FreeBSD__)
       //   || defined(__NetBSD__)
       //   || defined(__OpenBSD__)

// Solaris: /dev/poll.
#if defined(__sun)
# if !defined(STDNET_HAS_DEV_POLL)
#  if !defined(STDNET_DISABLE_DEV_POLL)
#   define STDNET_HAS_DEV_POLL 1
#  endif // !defined(STDNET_DISABLE_DEV_POLL)
# endif // !defined(STDNET_HAS_DEV_POLL)
#endif // defined(__sun)

// Serial ports.
#if !defined(STDNET_HAS_SERIAL_PORT)
# if defined(STDNET_HAS_IOCP) \
    || !defined(STDNET_WINDOWS) && !defined(__CYGWIN__)
#  if !defined(__SYMBIAN32__)
#   if !defined(STDNET_DISABLE_SERIAL_PORT)
#    define STDNET_HAS_SERIAL_PORT 1
#   endif // !defined(STDNET_DISABLE_SERIAL_PORT)
#  endif // !defined(__SYMBIAN32__)
# endif // defined(STDNET_HAS_IOCP)
        //   || !defined(STDNET_WINDOWS) && !defined(__CYGWIN__)
#endif // !defined(STDNET_HAS_SERIAL_PORT)

// Windows: stream handles.
#if !defined(STDNET_HAS_WINDOWS_STREAM_HANDLE)
# if !defined(STDNET_DISABLE_WINDOWS_STREAM_HANDLE)
#  if defined(STDNET_HAS_IOCP)
#   define STDNET_HAS_WINDOWS_STREAM_HANDLE 1
#  endif // defined(STDNET_HAS_IOCP)
# endif // !defined(STDNET_DISABLE_WINDOWS_STREAM_HANDLE)
#endif // !defined(STDNET_HAS_WINDOWS_STREAM_HANDLE)

// Windows: random access handles.
#if !defined(STDNET_HAS_WINDOWS_RANDOM_ACCESS_HANDLE)
# if !defined(STDNET_DISABLE_WINDOWS_RANDOM_ACCESS_HANDLE)
#  if defined(STDNET_HAS_IOCP)
#   define STDNET_HAS_WINDOWS_RANDOM_ACCESS_HANDLE 1
#  endif // defined(STDNET_HAS_IOCP)
# endif // !defined(STDNET_DISABLE_WINDOWS_RANDOM_ACCESS_HANDLE)
#endif // !defined(STDNET_HAS_WINDOWS_RANDOM_ACCESS_HANDLE)

// Windows: object handles.
#if !defined(STDNET_HAS_WINDOWS_OBJECT_HANDLE)
# if !defined(STDNET_DISABLE_WINDOWS_OBJECT_HANDLE)
#  if defined(STDNET_WINDOWS) || defined(__CYGWIN__)
#   if !defined(UNDER_CE)
#    define STDNET_HAS_WINDOWS_OBJECT_HANDLE 1
#   endif // !defined(UNDER_CE)
#  endif // defined(STDNET_WINDOWS) || defined(__CYGWIN__)
# endif // !defined(STDNET_DISABLE_WINDOWS_OBJECT_HANDLE)
#endif // !defined(STDNET_HAS_WINDOWS_OBJECT_HANDLE)

// Windows: OVERLAPPED wrapper.
#if !defined(STDNET_HAS_WINDOWS_OVERLAPPED_PTR)
# if !defined(STDNET_DISABLE_WINDOWS_OVERLAPPED_PTR)
#  if defined(STDNET_HAS_IOCP)
#   define STDNET_HAS_WINDOWS_OVERLAPPED_PTR 1
#  endif // defined(STDNET_HAS_IOCP)
# endif // !defined(STDNET_DISABLE_WINDOWS_OVERLAPPED_PTR)
#endif // !defined(STDNET_HAS_WINDOWS_OVERLAPPED_PTR)

// POSIX: stream-oriented file descriptors.
#if !defined(STDNET_HAS_POSIX_STREAM_DESCRIPTOR)
# if !defined(STDNET_DISABLE_POSIX_STREAM_DESCRIPTOR)
#  if !defined(STDNET_WINDOWS) && !defined(__CYGWIN__)
#   define STDNET_HAS_POSIX_STREAM_DESCRIPTOR 1
#  endif // !defined(STDNET_WINDOWS) && !defined(__CYGWIN__)
# endif // !defined(STDNET_DISABLE_POSIX_STREAM_DESCRIPTOR)
#endif // !defined(STDNET_HAS_POSIX_STREAM_DESCRIPTOR)

// UNIX domain sockets.
#if !defined(STDNET_HAS_LOCAL_SOCKETS)
# if !defined(STDNET_DISABLE_LOCAL_SOCKETS)
#  if !defined(STDNET_WINDOWS) && !defined(__CYGWIN__)
#   define STDNET_HAS_LOCAL_SOCKETS 1
#  endif // !defined(STDNET_WINDOWS) && !defined(__CYGWIN__)
# endif // !defined(STDNET_DISABLE_LOCAL_SOCKETS)
#endif // !defined(STDNET_HAS_LOCAL_SOCKETS)

// Can use sigaction() instead of signal().
#if !defined(STDNET_HAS_SIGACTION)
# if !defined(STDNET_DISABLE_SIGACTION)
#  if !defined(STDNET_WINDOWS) && !defined(__CYGWIN__)
#   define STDNET_HAS_SIGACTION 1
#  endif // !defined(STDNET_WINDOWS) && !defined(__CYGWIN__)
# endif // !defined(STDNET_DISABLE_SIGACTION)
#endif // !defined(STDNET_HAS_SIGACTION)

// Can use signal().
#if !defined(STDNET_HAS_SIGNAL)
# if !defined(STDNET_DISABLE_SIGNAL)
#  if !defined(UNDER_CE)
#   define STDNET_HAS_SIGNAL 1
#  endif // !defined(UNDER_CE)
# endif // !defined(STDNET_DISABLE_SIGNAL)
#endif // !defined(STDNET_HAS_SIGNAL)

// Whether standard iostreams are disabled.
//#if !defined(STDNET_NO_IOSTREAM)
//# define STDNET_NO_IOSTREAM 1
//#endif // !defined(STDNET_NO_IOSTREAM)

// Whether exception handling is disabled.
//#if !defined(STDNET_NO_EXCEPTIONS)
//# define STDNET_NO_EXCEPTIONS 1
//#endif // !defined(STDNET_NO_EXCEPTIONS)

// Whether the typeid operator is supported.
//#if !defined(STDNET_NO_TYPEID)
//# define STDNET_NO_TYPEID 1
//#endif // !defined(STDNET_NO_TYPEID)

// On POSIX (and POSIX-like) platforms we need to include unistd.h in order to
// get access to the various platform feature macros, e.g. to be able to test
// for threads support.
#if !defined(STDNET_HAS_UNISTD_H)
# if defined(unix) \
  || defined(__unix) \
  || defined(_XOPEN_SOURCE) \
  || defined(_POSIX_SOURCE) \
  || (defined(__MACH__) && defined(__APPLE__)) \
  || defined(__FreeBSD__) \
  || defined(__NetBSD__) \
  || defined(__OpenBSD__) \
  || defined(__linux__)
#  define STDNET_HAS_UNISTD_H 1
# endif
#endif // !defined(STDNET_HAS_UNISTD_H)
#if defined(STDNET_HAS_UNISTD_H)
# include <unistd.h>
#endif // defined(STDNET_HAS_UNISTD_H)

// Threads.
#if !defined(STDNET_HAS_THREADS)
# if !defined(STDNET_DISABLE_THREADS)
#  if defined(_MSC_VER) && defined(_MT)
#   define STDNET_HAS_THREADS 1
#  elif defined(__BORLANDC__) && defined(__MT__)
#   define STDNET_HAS_THREADS 1
#  elif defined(_POSIX_THREADS)
#   define STDNET_HAS_THREADS 1
#  endif // defined(_MSC_VER) && defined(_MT)
# endif // !defined(STDNET_DISABLE_THREADS)
#endif // !defined(STDNET_HAS_THREADS)

// POSIX threads.
#if !defined(STDNET_HAS_PTHREADS)
# if defined(STDNET_HAS_THREADS)
#  if defined(_POSIX_THREADS)
#   define STDNET_HAS_PTHREADS 1
#  endif // defined(_POSIX_THREADS)
# endif // defined(STDNET_HAS_THREADS)
#endif // !defined(STDNET_HAS_PTHREADS)

// Helper to prevent macro expansion.
#define STDNET_PREVENT_MACRO_SUBSTITUTION

// Helper to define in-class constants.
#if !defined(STDNET_STATIC_CONSTANT)
# define STDNET_STATIC_CONSTANT(type, assignment) \
   static const type assignment
#endif // !defined(STDNET_STATIC_CONSTANT)

// Microsoft Visual C++'s secure C runtime library.
#if !defined(STDNET_HAS_SECURE_RTL)
# if !defined(STDNET_DISABLE_SECURE_RTL)
#  if defined(STDNET_MSVC) \
    && (STDNET_MSVC >= 1400) \
    && !defined(UNDER_CE)
#   define STDNET_HAS_SECURE_RTL 1
#  endif // defined(STDNET_MSVC)
         // && (STDNET_MSVC >= 1400)
         // && !defined(UNDER_CE)
# endif // !defined(STDNET_DISABLE_SECURE_RTL)
#endif // !defined(STDNET_HAS_SECURE_RTL)

// Handler hooking. Disabled for ancient Borland C++ and gcc compilers.
#if !defined(STDNET_HAS_HANDLER_HOOKS)
# if !defined(STDNET_DISABLE_HANDLER_HOOKS)
#  if defined(__GNUC__)
#   if (__GNUC__ >= 3)
#    define STDNET_HAS_HANDLER_HOOKS 1
#   endif // (__GNUC__ >= 3)
#  elif !defined(__BORLANDC__)
#   define STDNET_HAS_HANDLER_HOOKS 1
#  endif // !defined(__BORLANDC__)
# endif // !defined(STDNET_DISABLE_HANDLER_HOOKS)
#endif // !defined(STDNET_HAS_HANDLER_HOOKS)

// Support for the __thread keyword extension.
#if !defined(STDNET_DISABLE_THREAD_KEYWORD_EXTENSION)
# if defined(__linux__)
#  if defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))
#   if ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 3)) || (__GNUC__ > 3)
#    if !defined(__INTEL_COMPILER) && !defined(__ICL)
#     define STDNET_HAS_THREAD_KEYWORD_EXTENSION 1
#    elif defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 1100)
#     define STDNET_HAS_THREAD_KEYWORD_EXTENSION 1
#    endif // defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 1100)
#   endif // ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 3)) || (__GNUC__ > 3)
#  endif // defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))
# endif // defined(__linux__)
#endif // !defined(STDNET_DISABLE_THREAD_KEYWORD_EXTENSION)

// Support for POSIX ssize_t typedef.
#if !defined(STDNET_DISABLE_SSIZE_T)
# if defined(__linux__) \
   || (defined(__MACH__) && defined(__APPLE__))
#  define STDNET_HAS_SSIZE_T 1
# endif // defined(__linux__)
        //   || (defined(__MACH__) && defined(__APPLE__))
#endif // !defined(STDNET_DISABLE_SSIZE_T)

#endif // STDNET_DETAIL_CONFIG_HPP
