//
// libcppwrap - A collection of C++ wrappers for native APIs
// Copyright (C) 2021-2023 David A. Norris <danorris@gmail.com>
// Published under the MIT license - https://opensource.org/licenses/MIT
//

#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <stdexcept>

#include <ifaddrs.h>
#include <netinet/ip6.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <w/handle.hpp>
#include <w/iterators.hpp>
#include <w/posix.hpp>

namespace w
{
	/**
	 * Wraps `struct sockaddr_in`, adding constructors for convenient initialization.
	 */
	struct ipv4_address : sockaddr_in
	{
		/**
		 * Initializes an IPv4 "any" address.
		 *
		 * @param port The port number.
		 * @throw std::system_error An error occurred parsing @p address.
		 */
		ipv4_address(std::uint16_t port = 0);

		/**
		 * Initializes an IPv4 address.
		 *
		 * @param address A numeric string representation of the IPv4 address.
		 * @param port The port number.
		 * @throw std::system_error An error occurred parsing @p address.
		 */
		ipv4_address(const char *address, std::uint16_t port = 0);

		/**
		 * Initializes an IPv4 address.
		 *
		 * @param address The IPv4 address.
		 * @param port The port number.
		 * @throw std::system_error An error occurred parsing @p address.
		 */
		ipv4_address(const in_addr& address, std::uint16_t port = 0);
	};

	/**
	 * Wraps `struct sockaddr_in6`, adding constructors for convenient initialization.
	 */
	struct ipv6_address : sockaddr_in6
	{
		/**
		 * Initializes an IPv6 "any" address.
		 *
		 * @param port The port number.
		 * @throw std::system_error An error occurred parsing @p address.
		 */
		ipv6_address(std::uint16_t port = 0);

		/**
		 * Initializes an IPv6 address.
		 *
		 * @param address A numeric string representation of the IPv6 address.
		 * @param port The port number.
		 * @throw std::system_error An error occurred parsing @p address.
		 */
		ipv6_address(const char *address, std::uint16_t port = 0);

		/**
		 * Initializes an IPv6 address.
		 *
		 * @param address A numeric string representation of the IPv6 address.
		 * @param port The port number.
		 * @param interface_index The index of the interface to scope to.
		 * @throw std::system_error An error occurred parsing @p address.
		 */
		ipv6_address(const char *address, std::uint16_t port, unsigned interface_index);

		/**
		 * Initializes an IPv6 address.
		 *
		 * @param address A numeric string representation of the IPv6 address.
		 * @param port The port number.
		 * @param interface_name The name of the interface to scope to.
		 * @throw std::system_error An error occurred parsing @p address or looking up
		 *        @p interface_name.
		 */
		ipv6_address(const char *address, std::uint16_t port, const char *interface_name);

		/**
		 * Initializes an IPv6 address.
		 *
		 * @param address The IPv6 address.
		 * @param port The port number.
		 * @throw std::system_error An error occurred parsing @p address.
		 */
		ipv6_address(const in6_addr& address, std::uint16_t port = 0);

		/**
		 * Initializes an IPv6 address.
		 *
		 * @param address The IPv6 address.
		 * @param port The port number.
		 * @param interface_index The index of the interface to scope to.
		 * @throw std::system_error An error occurred parsing @p address.
		 */
		ipv6_address(const in6_addr& address, std::uint16_t port, unsigned interface_index);

		/**
		 * Initializes an IPv6 address.
		 *
		 * @param address The IPv6 address.
		 * @param port The port number.
		 * @param interface_name The name of the interface to scope to.
		 * @throw std::system_error An error occurred parsing @p address or looking up
		 *        @p interface_name.
		 */
		ipv6_address(const in6_addr& address, std::uint16_t port, const char *interface_name);
	};

	/**
	 * Accepts a connection on a socket.
	 *
	 * @param sockfd The socket on which to accept a connection.
	 * @param addr A pointer to the address of the remote endpoint.
	 * @param addrlen A pointer to the size of the structure pointed to by @p addr, in bytes. On
	 *        return, the value holds the actual size of the source address. This might be larger
	 *        than the input value, in which case the address was truncated.
	 * @return The socket file descriptor of the new connection.
	 * @throw std::system_error An error occurred.
	 */
	w::fd accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

	/**
	 * Accepts a connection on a socket.
	 *
	 * @tparam Address The type of @p addr.
	 * @param sockfd The socket on which to accept a connection.
	 * @param addr A reference to the address of the remote endpoint.
	 * @throw std::system_error An error occurred.
	 * @throw std::runtime_error The structure referenced by @p addr is not the correct size to
	 *        hold the remote address.
	 */
	template <typename Address>
	w::fd accept(int sockfd, Address& addr)
	{
		socklen_t addrlen = sizeof(addr);
		w::fd fd = w::accept(sockfd, reinterpret_cast<struct sockaddr *>(&addr), &addrlen);

		if (addrlen != sizeof(addr))
			throw std::runtime_error(
				"provided structure is not the correct size to hold receive connect address");

		return fd;
	}

	/**
	 * Binds a socket to an address.
	 *
	 * @param sockfd The socket to bind.
	 * @param addr A pointer to the address to bind to.
	 * @param addrlen The size of the structure pointed to by @p addr, in bytes.
	 * @throw std::system_error An error occurred.
	 */
	void bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

	/**
	 * Binds a socket.
	 *
	 * @tparam Address The type of @p addr.
	 * @param sockfd The socket to bind.
	 * @param addr A reference to the address to bind to.
	 * @throw std::system_error An error occurred.
	 */
	template <typename Address>
	void bind(int sockfd, const Address& addr)
	{
		w::bind(sockfd, reinterpret_cast<const struct sockaddr *>(&addr), sizeof(addr));
	}

	/**
	 * Connects a socket.
	 *
	 * @param sockfd The socket to connect.
	 * @param addr A pointer to the address to connect to.
	 * @param addrlen The size of the structure pointed to by @p addr, in bytes.
	 * @throw std::system_error An error occurred.
	 */
	void connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

	/**
	 * Connects a socket.
	 *
	 * @tparam Address The type of @p addr.
	 * @param sockfd The socket to connect.
	 * @param addr A reference to the address to connect to.
	 * @throw std::system_error An error occurred.
	 */
	template <typename Address>
	void connect(int sockfd, const Address& addr)
	{
		w::connect(sockfd, reinterpret_cast<const struct sockaddr *>(&addr), sizeof(addr));
	}

	/**
	 * Gets a pointer to the head of a linked list of the system's network interfaces.
	 *
	 * @return A pointer to the head of a linked list of the system's network interfaces.
	 * @throw std::system_error An error occurred.
	 */
	w::handle<struct ifaddrs *, nullptr, ::freeifaddrs> getifaddrs();

	/**
	 * Gets an option on a socket.
	 *
	 * @param sockfd The socket on which to get the option.
	 * @param level The option level.
	 * @param optname The option to get.
	 * @param optval A pointer to the value to be filled in.
	 * @param optlen A pointer to the size of the value pointed to by @p optval. On return, the
	 *        value holds the actual size of the value. This might be larger than the input value,
	 *        in which case the value was truncated.
	 * @return The return value of the option (which cannot be -1).
	 * @throw std::system_error An error occurred.
	 */
	int getsockopt(int sockfd, int level, int optname,
		void *optval = nullptr, socklen_t *optlen = nullptr);

	/**
	 * Gets an option on a socket.
	 *
	 * @param sockfd The socket on which to get the option.
	 * @param level The option level.
	 * @param optname The option to get.
	 * @return The value returned for the option.
	 * @throw std::system_error An error occurred.
	 * @throw std::runtime_error @p Value has the wrong size for this option.
	 */
	template <typename Value>
	Value getsockopt(int sockfd, int level, int optname)
	{
		Value optval;
		socklen_t optlen = sizeof(optval);
		w::getsockopt(sockfd, level, optname, &optval, &optlen);
		if (optlen != sizeof(optval))
			throw std::runtime_error(
				"wrong size value expected for socket option");
		return optval;
	}

	/**
	 * Returns the index of the specified network interface.
	 *
	 * @param ifname The name of the network interface.
	 * @return The nonzero index of the network interface.
	 * @throw std::system_error An error occurred.
	 */
	unsigned if_nametoindex(const char *ifname);

	/**
	 * Converts an IPv4 or IPv6 address to a string.
	 *
	 * @param af The address family.
	 * @param src A pointer to the address.
	 * @param dst A pointer to a character buffer to be filled.
	 * @param size The size of the buffer pointed to by @p dst, in bytes.
	 * @return @p dst.
	 * @throw std::system_error An error occurred.
	 */
	char *inet_ntop(int af, const void *src, char *dst, socklen_t size);

	/**
	 * Converts an IPv4 or IPv6 address from a string to a binary value.
	 *
	 * @param af The address family.
	 * @param src A pointer to the address string.
	 * @param dst A pointer to a socket address structure to be filled.
	 * @return @p dst.
	 * @throw std::system_error An error occurred.
	 */
	void *inet_pton(int af, const char *src, void *dst);

	/**
	 * Converts an IPv4 or IPv6 address from a string to a binary value.
	 *
	 * @param af The address family.
	 * @param src A pointer to the address string.
	 * @param dst A reference to a socket address structure to be filled.
	 * @return @p dst.
	 * @throw std::system_error An error occurred.
	 */
	template <typename Address>
	Address& inet_pton(int af, const char *src, Address& dst)
	{
		w::inet_pton(af, src, static_cast<void *>(&dst));
		return dst;
	}

	/**
	 * Puts a socket in a listening state.
	 *
	 * @param sockfd The socket to listen on.
	 * @param backlog The maximum number of pending connections to allow.
	 * @throw std::system_error An error occurred.
	 */
	void listen(int sockfd, int backlog);

	/**
	 * Receives a message from a socket.
	 *
	 * @param sockfd The socket to receive from.
	 * @param buf A pointer to an array where received data should be stored.
	 * @param len The maximum number of bytes to receive.
	 * @param flags A bitwise combination of flags.
	 * @return The number of bytes successfully received (which may be zero).
	 * @throw std::system_error An error occurred. This includes `EAGAIN` on non-blocking sockets.
	 */
	std::size_t recv(int sockfd, void *buf, std::size_t len, int flags);

	/**
	 * Receives a message from a socket.
	 *
	 * @param sockfd The socket to receive from.
	 * @param msg A pointer to a message structure to be filled in.
	 * @param flags A bitwise combination of flags.
	 * @return The number of bytes successfully received (which may be zero).
	 * @throw std::system_error An error occurred. This includes `EAGAIN` on non-blocking sockets.
	 */
	std::size_t recvmsg(int sockfd, struct msghdr *msg, int flags);

	/**
	 * Receives a message from a socket.
	 *
	 * @param sockfd The socket to receive from.
	 * @param buf A pointer to an array where received data should be stored.
	 * @param len The maximum number of bytes to receive.
	 * @param flags A bitwise combination of flags.
	 * @param src_addr A pointer to the source address.
	 * @param addrlen A pointer to the size of the structure pointed to by @p src_addr, in bytes.
	 *        On return, the value holds the actual size of the source address. This might be
	 *        larger than the input value, in which case the address was truncated.
	 * @return The number of bytes successfully received (which may be zero).
	 * @throw std::system_error An error occurred. This includes `EAGAIN` on non-blocking sockets.
	 * @throw std::runtime_error The structure pointed to by @p src_addr is too small to hold the
	 *        source address.
	 */
	std::size_t recvfrom(int sockfd, void *buf, std::size_t len, int flags,
		struct sockaddr *src_addr = nullptr, socklen_t *addrlen = nullptr);

	/**
	 * Receives a message from a socket.
	 *
	 * @tparam Address The type of @p src_addr.
	 * @param sockfd The socket to receive from.
	 * @param buf A pointer to an array where received data should be stored.
	 * @param len The maximum number of bytes to receive.
	 * @param flags A bitwise combination of flags.
	 * @param src_addr A reference to the source address.
	 * @return The number of bytes successfully received (which may be zero).
	 * @throw std::system_error An error occurred. This includes `EAGAIN` on non-blocking sockets.
	 * @throw std::runtime_error The structure referenced by @p src_addr is not the correct size
	 *        to hold the source address.
	 */
	template <typename Address>
	std::size_t recvfrom(int sockfd, void *buf, std::size_t len, int flags, Address& src_addr)
	{
		socklen_t addrlen = sizeof(src_addr);
		std::size_t rv = w::recvfrom(sockfd, buf, len, flags,
			reinterpret_cast<struct sockaddr *>(&src_addr), &addrlen);

		if (addrlen != sizeof(src_addr))
			throw std::runtime_error(
				"provided structure is not the correct size to hold receive source address");

		return rv;
	}

	/**
	 * Sends a message on a socket.
	 *
	 * @param sockfd The socket to send from.
	 * @param buf A pointer to the data to send.
	 * @param len The maximum number of bytes to send.
	 * @param flags A bitwise combination of flags.
	 * @return The number of bytes successfully accepted for transmission (which may be zero).
	 * @throw std::system_error An error occurred. This includes `EAGAIN` on non-blocking sockets.
	 */
	std::size_t send(int sockfd, const void *buf, std::size_t len, int flags);

	/**
	 * Sends a message on a socket.
	 *
	 * @param sockfd The socket to send from.
	 * @param msg A pointer to a message structure describing the message to send.
	 * @param flags A bitwise combination of flags.
	 * @return The number of bytes successfully accepted for transmission (which may be zero).
	 * @throw std::system_error An error occurred. This includes `EAGAIN` on non-blocking sockets.
	 */
	std::size_t sendmsg(int sockfd, const struct msghdr *msg, int flags);

	/**
	 * Sends a message on a socket.
	 *
	 * @param sockfd The socket to send from.
	 * @param buf A pointer to the data to send.
	 * @param len The maximum number of bytes to send.
	 * @param flags A bitwise combination of flags.
	 * @param dest_addr A pointer to the destination address.
	 * @param addrlen The size of the structure pointed to by @p dest_addr, in bytes.
	 * @return The number of bytes successfully accepted for transmission (which may be zero).
	 * @throw std::system_error An error occurred. This includes `EAGAIN` on non-blocking sockets.
	 */
	std::size_t sendto(int sockfd, const void *buf, std::size_t len,
		int flags, const struct sockaddr *dest_addr, socklen_t addrlen);

	/**
	 * Sends a message on a socket.
	 *
	 * @tparam Address The type of @p dest_addr.
	 * @param sockfd The socket to send from.
	 * @param buf A pointer to the data to send.
	 * @param len The number of bytes to send.
	 * @param flags A bitwise combination of flags.
	 * @param dest_addr A reference to the destination address.
	 * @return The number of bytes successfully accepted for transmission (which may be zero).
	 * @throw std::system_error An error occurred. This includes `EAGAIN` on non-blocking sockets.
	 */
	template <typename Address>
	std::size_t sendto(int sockfd, const void *buf, std::size_t len,
		int flags, const Address& dest_addr)
	{
		return w::sendto(sockfd, buf, len, flags,
			reinterpret_cast<const struct sockaddr *>(&dest_addr), sizeof(dest_addr));
	}

	/**
	 * Sets an option on a socket.
	 *
	 * @param sockfd The socket on which to set the option.
	 * @param level The option level.
	 * @param optname The option to set.
	 * @param optval A pointer to the value to set.
	 * @param optlen The size of the value pointed to by @p optval, in bytes.
	 * @return The return value of the option (which cannot be -1).
	 * @throw std::system_error An error occurred.
	 */
	int setsockopt(int sockfd, int level, int optname,
		const void *optval = nullptr, socklen_t optlen = 0);

	/**
	 * Sets an option on a socket.
	 *
	 * @tparam Value The type of @p optval.
	 * @param sockfd The socket on which to set the option.
	 * @param level The option level.
	 * @param optname The option to set.
	 * @param optval A reference to the value to set.
	 * @throw std::system_error An error occurred.
	 */
	template <typename Value>
	void setsockopt(int sockfd, int level, int optname, const Value& optval)
	{
		w::setsockopt(sockfd, level, optname, &optval, sizeof(optval));
	}

	/**
	 * Shuts down part of a full-duplex connection.
	 *
	 * @param sockfd The socket to shut down.
	 * @param how A flag specifying what to shut down.
	 * @throw std::system_error An error occurred.
	 */
	void shutdown(int sockfd, int how);

	/**
	 * Creates a socket.
	 *
	 * @param domain The communication domain.
	 * @param type The type of socket.
	 * @param protocol The protocol on which to communicate.
	 * @return The created socket.
	 * @throw std::system_error An error occurred.
	*/
	w::fd socket(int domain, int type, int protocol = 0);
}

namespace w::detail
{
	inline const struct ifaddrs *get_next_ifaddrs(const struct ifaddrs *cur)
	{
		return cur->ifa_next;
	}
}

/**
 * Returns an input iterator pointing to the `struct ifaddrs` entry @p ptr.
 *
 * @param ptr The `struct ifaddrs` entry to point to.
 * @return An input iterator.
 */
inline auto begin(const struct ifaddrs *ptr) noexcept
{
	return w::const_list_iterator<struct ifaddrs, w::detail::get_next_ifaddrs>(ptr);
}

/**
 * Returns an input iterator pointing to the end of a `struct ifaddrs` array.
 *
 * @param ptr The `struct ifaddrs` array to point to the end of.
 * @return An input iterator.
 */
inline auto end(const struct ifaddrs *ptr) noexcept
{
	return w::const_list_iterator<struct ifaddrs, w::detail::get_next_ifaddrs>();
}

/**
 * Compares two IPv6 addresses.
 *
 * @param a The first address to compare.
 * @param b The second address to compare.
 * @return `true` if @p a and @p b are the same address.
 */
inline bool operator==(const in6_addr& a, const in6_addr& b) noexcept
{
	return 0 == std::memcmp(&a, &b, sizeof(in6_addr));
}

/**
 * Compares two IPv6 addresses.
 *
 * @param a The first address to compare.
 * @param b The second address to compare.
 * @return `true` if @p a and @p b are different addresses.
 */
inline bool operator!=(const in6_addr& a, const in6_addr& b) noexcept
{
	return !(a == b);
}

/**
 * Compares two IPv6 addresses.
 *
 * @param a The first address to compare.
 * @param b The second address to compare.
 * @return `true` if @p a is a lower address than @p b.
 */
inline bool operator<(const in6_addr& a, const in6_addr& b) noexcept
{
	return std::memcmp(&a, &b, sizeof(in6_addr)) < 0;
}
