//
// address_v4.cpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// Test that header file is self-contained.
#include "std/net/ip/address_v4.hpp"

#include "../unit_test.hpp"
#include <sstream>
#include "std/net/ip/address_v6.hpp"

//------------------------------------------------------------------------------

// ip_address_v4_compile test
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
// The following test checks that all public member functions on the class
// ip::address_v4 compile and link correctly. Runtime failures are ignored.

namespace ip_address_v4_compile {

void test()
{
  namespace ip = std::experimental::net::ip;

  try
  {
    std::error_code ec;
    std::string string_value;

    // address_v4 constructors.

    ip::address_v4 addr1;
    const ip::address_v4::bytes_type const_bytes_value = { { 127, 0, 0, 1 } };
    ip::address_v4 addr2(const_bytes_value);
    const unsigned long const_ulong_value = 0x7F000001;
    ip::address_v4 addr3(const_ulong_value);
    ip::address_v4 addr4("127.0.0.1");
    ip::address_v4 addr5("127.0.0.1", ec);
    ip::address_v4 addr6(string_value);
    ip::address_v4 addr7(string_value, ec);

    // address_v4 functions.

    bool b = addr1.is_loopback();
    (void)b;

    b = addr1.is_unspecified();
    (void)b;

    b = addr1.is_class_a();
    (void)b;

    b = addr1.is_class_b();
    (void)b;

    b = addr1.is_class_c();
    (void)b;

    b = addr1.is_multicast();
    (void)b;

    ip::address_v4::bytes_type bytes_value = addr1.to_bytes();
    (void)bytes_value;

    unsigned long ulong_value = addr1.to_ulong();
    (void)ulong_value;

    string_value = addr1.to_string();
    string_value = addr1.to_string(ec);

    // address_v4 static functions.

    addr1 = ip::address_v4::any();

    addr1 = ip::address_v4::loopback();

    addr1 = ip::address_v4::broadcast();

    addr1 = ip::address_v4::broadcast(addr2, addr3);

    addr1 = ip::address_v4::netmask(addr2);

    // address_v4 comparisons.

    b = (addr1 == addr2);
    (void)b;

    b = (addr1 != addr2);
    (void)b;

    b = (addr1 < addr2);
    (void)b;

    b = (addr1 > addr2);
    (void)b;

    b = (addr1 <= addr2);
    (void)b;

    b = (addr1 >= addr2);
    (void)b;

    // address_v4 creation.

    addr1 = ip::make_address_v4(const_bytes_value);
    addr1 = ip::make_address_v4(const_ulong_value);
    addr1 = ip::make_address_v4(ip::v4_mapped, ip::address_v6());
    addr1 = ip::make_address_v4("127.0.0.1");
    addr1 = ip::make_address_v4("127.0.0.1", ec);
    addr1 = ip::make_address_v4(string_value);
    addr1 = ip::make_address_v4(string_value, ec);

    // address_v4 I/O.

    std::ostringstream os;
    os << addr1;

    std::wostringstream wos;
    wos << addr1;
  }
  catch (std::exception&)
  {
  }
}

} // namespace ip_address_v4_compile

//------------------------------------------------------------------------------

// ip_address_v4_runtime test
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
// The following test checks that the various public member functions meet the
// necessary postconditions.

namespace ip_address_v4_runtime {

void test()
{
  using std::experimental::net::ip::address_v4;

  address_v4 a1;
  STDNET_CHECK(a1.to_bytes()[0] == 0);
  STDNET_CHECK(a1.to_bytes()[1] == 0);
  STDNET_CHECK(a1.to_bytes()[2] == 0);
  STDNET_CHECK(a1.to_bytes()[3] == 0);
  STDNET_CHECK(a1.to_ulong() == 0);

  address_v4::bytes_type b1 = {{ 1, 2, 3, 4 }};
  address_v4 a2(b1);
  STDNET_CHECK(a2.to_bytes()[0] == 1);
  STDNET_CHECK(a2.to_bytes()[1] == 2);
  STDNET_CHECK(a2.to_bytes()[2] == 3);
  STDNET_CHECK(a2.to_bytes()[3] == 4);
  STDNET_CHECK(((a2.to_ulong() >> 24) & 0xFF) == b1[0]);
  STDNET_CHECK(((a2.to_ulong() >> 16) & 0xFF) == b1[1]);
  STDNET_CHECK(((a2.to_ulong() >> 8) & 0xFF) == b1[2]);
  STDNET_CHECK((a2.to_ulong() & 0xFF) == b1[3]);

  address_v4 a3(0x01020304);
  STDNET_CHECK(a3.to_bytes()[0] == 1);
  STDNET_CHECK(a3.to_bytes()[1] == 2);
  STDNET_CHECK(a3.to_bytes()[2] == 3);
  STDNET_CHECK(a3.to_bytes()[3] == 4);
  STDNET_CHECK(a3.to_ulong() == 0x01020304);

  STDNET_CHECK(address_v4(0x7F000001).is_loopback());
  STDNET_CHECK(address_v4(0x7F000002).is_loopback());
  STDNET_CHECK(!address_v4(0x00000000).is_loopback());
  STDNET_CHECK(!address_v4(0x01020304).is_loopback());

  STDNET_CHECK(address_v4(0x00000000).is_unspecified());
  STDNET_CHECK(!address_v4(0x7F000001).is_unspecified());
  STDNET_CHECK(!address_v4(0x01020304).is_unspecified());

  STDNET_CHECK(address_v4(0x01000000).is_class_a());
  STDNET_CHECK(address_v4(0x7F000000).is_class_a());
  STDNET_CHECK(!address_v4(0x80000000).is_class_a());
  STDNET_CHECK(!address_v4(0xBFFF0000).is_class_a());
  STDNET_CHECK(!address_v4(0xC0000000).is_class_a());
  STDNET_CHECK(!address_v4(0xDFFFFF00).is_class_a());
  STDNET_CHECK(!address_v4(0xE0000000).is_class_a());
  STDNET_CHECK(!address_v4(0xEFFFFFFF).is_class_a());
  STDNET_CHECK(!address_v4(0xF0000000).is_class_a());
  STDNET_CHECK(!address_v4(0xFFFFFFFF).is_class_a());

  STDNET_CHECK(!address_v4(0x01000000).is_class_b());
  STDNET_CHECK(!address_v4(0x7F000000).is_class_b());
  STDNET_CHECK(address_v4(0x80000000).is_class_b());
  STDNET_CHECK(address_v4(0xBFFF0000).is_class_b());
  STDNET_CHECK(!address_v4(0xC0000000).is_class_b());
  STDNET_CHECK(!address_v4(0xDFFFFF00).is_class_b());
  STDNET_CHECK(!address_v4(0xE0000000).is_class_b());
  STDNET_CHECK(!address_v4(0xEFFFFFFF).is_class_b());
  STDNET_CHECK(!address_v4(0xF0000000).is_class_b());
  STDNET_CHECK(!address_v4(0xFFFFFFFF).is_class_b());

  STDNET_CHECK(!address_v4(0x01000000).is_class_c());
  STDNET_CHECK(!address_v4(0x7F000000).is_class_c());
  STDNET_CHECK(!address_v4(0x80000000).is_class_c());
  STDNET_CHECK(!address_v4(0xBFFF0000).is_class_c());
  STDNET_CHECK(address_v4(0xC0000000).is_class_c());
  STDNET_CHECK(address_v4(0xDFFFFF00).is_class_c());
  STDNET_CHECK(!address_v4(0xE0000000).is_class_c());
  STDNET_CHECK(!address_v4(0xEFFFFFFF).is_class_c());
  STDNET_CHECK(!address_v4(0xF0000000).is_class_c());
  STDNET_CHECK(!address_v4(0xFFFFFFFF).is_class_c());

  STDNET_CHECK(!address_v4(0x01000000).is_multicast());
  STDNET_CHECK(!address_v4(0x7F000000).is_multicast());
  STDNET_CHECK(!address_v4(0x80000000).is_multicast());
  STDNET_CHECK(!address_v4(0xBFFF0000).is_multicast());
  STDNET_CHECK(!address_v4(0xC0000000).is_multicast());
  STDNET_CHECK(!address_v4(0xDFFFFF00).is_multicast());
  STDNET_CHECK(address_v4(0xE0000000).is_multicast());
  STDNET_CHECK(address_v4(0xEFFFFFFF).is_multicast());
  STDNET_CHECK(!address_v4(0xF0000000).is_multicast());
  STDNET_CHECK(!address_v4(0xFFFFFFFF).is_multicast());

  address_v4 a4 = address_v4::any();
  STDNET_CHECK(a4.to_bytes()[0] == 0);
  STDNET_CHECK(a4.to_bytes()[1] == 0);
  STDNET_CHECK(a4.to_bytes()[2] == 0);
  STDNET_CHECK(a4.to_bytes()[3] == 0);
  STDNET_CHECK(a4.to_ulong() == 0);

  address_v4 a5 = address_v4::loopback();
  STDNET_CHECK(a5.to_bytes()[0] == 0x7F);
  STDNET_CHECK(a5.to_bytes()[1] == 0);
  STDNET_CHECK(a5.to_bytes()[2] == 0);
  STDNET_CHECK(a5.to_bytes()[3] == 0x01);
  STDNET_CHECK(a5.to_ulong() == 0x7F000001);

  address_v4 a6 = address_v4::broadcast();
  STDNET_CHECK(a6.to_bytes()[0] == 0xFF);
  STDNET_CHECK(a6.to_bytes()[1] == 0xFF);
  STDNET_CHECK(a6.to_bytes()[2] == 0xFF);
  STDNET_CHECK(a6.to_bytes()[3] == 0xFF);
  STDNET_CHECK(a6.to_ulong() == 0xFFFFFFFF);

  address_v4 class_a_net(0xFF000000);
  address_v4 class_b_net(0xFFFF0000);
  address_v4 class_c_net(0xFFFFFF00);
  address_v4 other_net(0xFFFFFFFF);
  STDNET_CHECK(address_v4::netmask(address_v4(0x01000000)) == class_a_net);
  STDNET_CHECK(address_v4::netmask(address_v4(0x7F000000)) == class_a_net);
  STDNET_CHECK(address_v4::netmask(address_v4(0x80000000)) == class_b_net);
  STDNET_CHECK(address_v4::netmask(address_v4(0xBFFF0000)) == class_b_net);
  STDNET_CHECK(address_v4::netmask(address_v4(0xC0000000)) == class_c_net);
  STDNET_CHECK(address_v4::netmask(address_v4(0xDFFFFF00)) == class_c_net);
  STDNET_CHECK(address_v4::netmask(address_v4(0xE0000000)) == other_net);
  STDNET_CHECK(address_v4::netmask(address_v4(0xEFFFFFFF)) == other_net);
  STDNET_CHECK(address_v4::netmask(address_v4(0xF0000000)) == other_net);
  STDNET_CHECK(address_v4::netmask(address_v4(0xFFFFFFFF)) == other_net);
}

} // namespace ip_address_v4_runtime

//------------------------------------------------------------------------------

STDNET_TEST_SUITE
(
  "ip/address_v4",
  STDNET_TEST_CASE(ip_address_v4_compile::test)
  STDNET_TEST_CASE(ip_address_v4_runtime::test)
)
