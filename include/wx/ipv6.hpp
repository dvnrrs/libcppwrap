//
// libcppwrap - A collection of C++ wrappers for native APIs
// Copyright (C) 2021-2023 David A. Norris <danorris@gmail.com>
// Published under the MIT license - https://opensource.org/licenses/MIT
//

#pragma once

#include <string>

#include <netinet/ip6.h>

namespace wx::ipv6
{
	/**
	 * Tests if an IPv6 address has link-local scope. An address has link-local scope if and only
	 * if it is in the range [`fe80::`, `fe81::`).
	 *
	 * @param address The IPv6 address to test.
	 * @return `true` if @p address has link-local scope.
	 */

	bool is_link_local(const in6_addr& address) noexcept;

	/**
	 * Gets the first link-local address for the specified network interface. Note that it is
	 * possible for an interface to have more than one link-local address. In this case, one is
	 * selected at random (and subsequent calls may not return the same address). It is also
	 * possible for an interface to have no link-local address (for example, if IPv6 is
	 * disabled).
	 *
	 * @param interface_name The name of the network interface.
	 * @return The IPv6 link-local address.
	 * @throw std::system_error An error occurred.
	 * @throw std::runtime_error The specified interface doesn't exist or has no link-local
	 *        address.
	 */
	struct sockaddr_in6 get_link_local_address(const char *interface_name);

	/**
	 * Produces a string representation of an IPv6 address.
	 *
	 * @param address The IPv6 address to convert.
	 * @return A string representation of @p address, using `::` shorthand notation where
	 *         possible.
	 */
	std::string to_string(const in6_addr& address);
}
