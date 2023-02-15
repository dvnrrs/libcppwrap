//
// libcppwrap - A collection of C++ wrappers for native APIs
// Copyright (C) 2021-2023 David A. Norris <danorris@gmail.com>
// Published under the MIT license - https://opensource.org/licenses/MIT
//

#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <string>

#include <arpa/inet.h>
#include <sys/socket.h>

#include <w/sockets.hpp>
#include <wx/ipv6.hpp>

using namespace std::string_literals;

bool wx::ipv6::is_link_local(const in6_addr& address) noexcept
{
	return address.s6_addr[0] == 0xfe &&
	       address.s6_addr[1] == 0x80;
}

struct sockaddr_in6 wx::ipv6::get_link_local_address(const char *interface_name)
{
	auto interfaces = w::getifaddrs();

	const auto interface = std::find_if(
		begin(interfaces),
		end(interfaces),
		[=](const ifaddrs& a)
		{
			return a.ifa_addr &&
				a.ifa_name &&
				a.ifa_addr->sa_family == AF_INET6 &&
				!std::strcmp(a.ifa_name, interface_name) &&
				wx::ipv6::is_link_local(reinterpret_cast<const sockaddr_in6 *>(a.ifa_addr)->sin6_addr);
		});

	if (interface == end(interfaces))
		throw std::runtime_error("can't get IPv6 link-local address for '"s + interface_name + "': no such interface or no link-local address");

	return *reinterpret_cast<const sockaddr_in6 *>(interface->ifa_addr);
}

std::string wx::ipv6::to_string(const in6_addr& address)
{
	char str[64];
	w::inet_ntop(AF_INET6, &address, str, sizeof(str));
	return str;
}
