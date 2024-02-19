//
// libcppwrap - A collection of C++ wrappers for native APIs
// Copyright (C) 2021-2023 David A. Norris <danorris@gmail.com>
// Published under the MIT license - https://opensource.org/licenses/MIT
//

#include <chrono>
#include <limits>
#include <cstddef>
#include <cstdint>
#include <stdexcept>

#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/timerfd.h>

#include <w/assert.hpp>
#include <w/linux.hpp>
#include <w/posix.hpp>

w::fd w::epoll_create(int size)
{
	return w::throw_if_eq(
		::epoll_create(size),
		-1,
		"failed to create epoll instance");
}

void w::epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)
{
	w::throw_if_ne(
		::epoll_ctl(epfd, op, fd, event),
		0,
		"failed to add file descriptor to epoll instance");
}

void w::epoll_ctl(int epfd, int op, int fd, std::uint32_t events, void *user_data)
{
	struct epoll_event ev { .events = events, .data { .ptr = user_data } };
	w::epoll_ctl(epfd, op, fd, &ev);
}

void w::epoll_ctl(int epfd, int op, int fd, std::uint32_t events, int user_data)
{
	struct epoll_event ev { .events = events, .data { .fd = user_data } };
	w::epoll_ctl(epfd, op, fd, &ev);
}

void w::epoll_ctl(int epfd, int op, int fd, std::uint32_t events, std::uint32_t user_data)
{
	struct epoll_event ev { .events = events, .data { .u32 = user_data } };
	w::epoll_ctl(epfd, op, fd, &ev);
}

void w::epoll_ctl(int epfd, int op, int fd, std::uint32_t events, std::uint64_t user_data)
{
	struct epoll_event ev { .events = events, .data { .u64 = user_data } };
	w::epoll_ctl(epfd, op, fd, &ev);
}

unsigned w::epoll_wait(int epfd, struct epoll_event *events, int maxevents,
	std::chrono::milliseconds timeout)
{
	if (timeout.count() < std::numeric_limits<int>::min() ||
		timeout.count() > std::numeric_limits<int>::max())
		throw std::invalid_argument("invalid timeout for epoll wait");

	return w::throw_if_lt(
		::epoll_wait(epfd, events, maxevents, timeout.count()),
		0,
		"failed to wait on epoll instance");
}

w::fd w::eventfd(unsigned initval, int flags)
{
	return w::throw_if_eq(
		::eventfd(initval, flags),
		-1,
		"failed to create event file descriptor");
}

std::uint64_t w::eventfd_read(int evfd)
{
	std::uint64_t result;
	w::read(evfd, &result, sizeof(result));
	return result;
}

void w::eventfd_write(int evfd, std::uint64_t value)
{
	w::write(evfd, &value, sizeof(value));
}

w::fd w::timerfd_create(int clockid, int flags)
{
	return w::throw_if_eq(
		::timerfd_create(clockid, flags),
		-1,
		"failed to create timer file descriptor");
}

void w::timerfd_settime(int fd, int flags,
	const struct itimerspec *new_value,
	struct itimerspec *old_value)
{
	w::throw_if_ne(
		::timerfd_settime(fd, flags, new_value, old_value),
		0,
		"failed to set timer file descriptor interval");
}

std::pair<std::chrono::nanoseconds, std::chrono::nanoseconds>
w::timerfd_settime(int fd, int flags,
	std::chrono::nanoseconds interval,
	std::chrono::nanoseconds initial)
{
	struct itimerspec old_value, new_value;
	new_value.it_interval.tv_sec  = interval.count() / 1000000000;
	new_value.it_interval.tv_nsec = interval.count() % 1000000000;
	new_value.it_value.tv_sec  = initial.count() / 1000000000;
	new_value.it_value.tv_nsec = initial.count() % 1000000000;

	w::timerfd_settime(fd, flags, &new_value, &old_value);

	return std::make_pair(
		std::chrono::nanoseconds(
			old_value.it_interval.tv_sec * 1000000000 +
			old_value.it_interval.tv_nsec),
		std::chrono::nanoseconds(
			old_value.it_value.tv_sec * 1000000000 +
			old_value.it_value.tv_nsec));
}
