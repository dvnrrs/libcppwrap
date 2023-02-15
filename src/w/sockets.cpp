//
// libcppwrap - A collection of C++ wrappers for native APIs
// Copyright (C) 2021-2023 David A. Norris <danorris@gmail.com>
// Published under the MIT license - https://opensource.org/licenses/MIT
//

#include <cstring>
#include <string>

#include <ifaddrs.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <w/assert.hpp>
#include <w/handle.hpp>
#include <w/posix.hpp>
#include <w/sockets.hpp>

using namespace std::string_literals;

w::ipv4_address::ipv4_address(std::uint16_t port)
{
	sin_family = AF_INET;
	sin_port = htons(port);
	sin_addr.s_addr = INADDR_ANY;
	std::memset(sin_zero, 0, sizeof(sin_zero));
}

w::ipv4_address::ipv4_address(const char *address, std::uint16_t port)
{
	sin_family = AF_INET;
	w::inet_pton(AF_INET, address, sin_addr);
	sin_port = htons(port);
	std::memset(sin_zero, 0, sizeof(sin_zero));
}

w::ipv4_address::ipv4_address(const in_addr& address, std::uint16_t port)
{
	sin_family = AF_INET;
	sin_addr = address;
	sin_port = htons(port);
	std::memset(sin_zero, 0, sizeof(sin_zero));
}

w::ipv6_address::ipv6_address(std::uint16_t port)
{
	sin6_family = AF_INET6;
	sin6_port = htons(port);
	sin6_flowinfo = { };
	std::memset(&sin6_addr, 0, sizeof(sin6_addr));
	sin6_scope_id = 0;
}

w::ipv6_address::ipv6_address(const char *address, std::uint16_t port)
	: ipv6_address(address, port, 0u)
{
}

w::ipv6_address::ipv6_address(const char *address, std::uint16_t port, const char *interface_name)
	: ipv6_address(address, port, w::if_nametoindex(interface_name))
{
}

w::ipv6_address::ipv6_address(const char *address, std::uint16_t port, unsigned interface_index)
{
	sin6_family = AF_INET6;
	sin6_port = htons(port);
	sin6_flowinfo = { };
	w::inet_pton(AF_INET6, address, sin6_addr);
	sin6_scope_id = interface_index;
}


w::ipv6_address::ipv6_address(const in6_addr& address, std::uint16_t port)
	: ipv6_address(address, port, 0u)
{
}

w::ipv6_address::ipv6_address(const in6_addr& address, std::uint16_t port, const char *interface_name)
	: ipv6_address(address, port, w::if_nametoindex(interface_name))
{
}

w::ipv6_address::ipv6_address(const in6_addr& address, std::uint16_t port, unsigned interface_index)
{
	sin6_family = AF_INET6;
	sin6_port = htons(port);
	sin6_flowinfo = { };
	sin6_addr = address;
	sin6_scope_id = interface_index;
}

w::fd w::accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
	return w::throw_if_eq(
		::accept(sockfd, addr, addrlen),
		-1,
		"failed to accept connection on socket");
}

void w::bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	w::throw_if_ne(
		::bind(sockfd, addr, addrlen),
		0,
		"failed to bind socket");
}

void w::connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	w::throw_if_ne(
		::connect(sockfd, addr, addrlen),
		0,
		"failed to connect socket");
}

w::handle<struct ifaddrs *, nullptr, ::freeifaddrs> w::getifaddrs()
{
	struct ifaddrs *ifa;

	w::throw_if_nz(
		::getifaddrs(&ifa),
		"failed to get list of network interfaces");

	return ifa;
}

int w::getsockopt(int sockfd, int level, int optname,
	void *optval, socklen_t *optlen)
{
	return w::throw_if_eq(
		::getsockopt(sockfd, level, optname, optval, optlen),
		-1,
		"failed to get socket option");
}

unsigned w::if_nametoindex(const char *ifname)
{
	return w::throw_if_eq(
		::if_nametoindex(ifname),
		0u,
		("failed to look up index of network interface '"s + ifname + '\'').c_str());
}

char *w::inet_ntop(int af, const void *src, char *dst, socklen_t size)
{
	w::throw_if_eq(
		::inet_ntop(af, src, dst, size),
		static_cast<const char *>(nullptr),
		"failed to stringify IPv4/IPv6 address");

	return dst;
}

void *w::inet_pton(int af, const char *src, void *dst)
{
	int rv = ::inet_pton(af, src, dst);

	if (rv == 0)
		errno = EINVAL;

	w::throw_if_ne(
		rv,
		1,
		"failed to parse IPv4/IPv6 address string");

	return dst;
}

void w::listen(int sockfd, int backlog)
{
	w::throw_if_ne(
		::listen(sockfd, backlog),
		0,
		"failed to put socket in listening state");
}

std::size_t w::recv(int sockfd, void *buf, std::size_t len, int flags)
{
	return static_cast<std::size_t>(
		w::throw_if_lt(
			::recv(sockfd, buf, len, flags),
			ssize_t { 0 },
			"failed to receive from socket"));
}

std::size_t w::recvmsg(int sockfd, struct msghdr *msg, int flags)
{
	return static_cast<std::size_t>(
		w::throw_if_lt(
			::recvmsg(sockfd, msg, flags),
			ssize_t { 0 },
			"failed to receive message from socket"));
}

std::size_t w::recvfrom(int sockfd, void *buf, std::size_t len, int flags,
	struct sockaddr *src_addr, socklen_t *addrlen)
{
	return static_cast<std::size_t>(
		w::throw_if_lt(
			::recvfrom(sockfd, buf, len, flags, src_addr, addrlen),
			ssize_t { 0 },
			"failed to receive from socket with source address"));
}

std::size_t w::send(int sockfd, const void *buf, std::size_t len, int flags)
{
	return static_cast<std::size_t>(
		w::throw_if_lt(
			::send(sockfd, buf, len, flags),
			ssize_t { 0 },
			"failed to send to socket"));
}

std::size_t w::sendmsg(int sockfd, const struct msghdr *msg, int flags)
{
	return static_cast<std::size_t>(
		w::throw_if_lt(
			::sendmsg(sockfd, msg, flags),
			ssize_t { 0 },
			"failed to send message to socket"));
}

std::size_t w::sendto(int sockfd, const void *buf, std::size_t len,
	int flags, const struct sockaddr *dest_addr, socklen_t addrlen)
{
	return static_cast<std::size_t>(
		w::throw_if_lt(
			::sendto(sockfd, buf, len, flags, dest_addr, addrlen),
			ssize_t { 0 },
			"failed to send to socket with destination address"));
}

int w::setsockopt(int sockfd, int level, int optname,
	const void *optval, socklen_t optlen)
{
	return w::throw_if_eq(
		::setsockopt(sockfd, level, optname, optval, optlen),
		-1,
		"failed to set socket option");
}

void w::shutdown(int sockfd, int how)
{
	w::throw_if_ne(
		::shutdown(sockfd, how),
		0,
		"failed to shut down socket");
}

w::fd w::socket(int domain, int type, int protocol)
{
	return w::throw_if_eq(
		::socket(domain, type, protocol),
		-1,
		"failed to create socket");
}
