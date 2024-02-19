//
// libcppwrap - A collection of C++ wrappers for native APIs
// Copyright (C) 2021-2023 David A. Norris <danorris@gmail.com>
// Published under the MIT license - https://opensource.org/licenses/MIT
//

#pragma once

#include <chrono>
#include <cstdint>
#include <utility>

#include <w/posix.hpp>

#include <sys/epoll.h>

namespace w
{
	/**
	 * Creates an epoll instance.
	 *
	 * @param size This value is ignored, but must be nonzero (see the man page).
	 * @return The file descriptor for the epoll instance.
	 * @throw std::system_error An error occurred.
	 */
	w::fd epoll_create(int size = sizeof(nullptr));

	/**
	 * Manipulates an epoll instance.
	 *
	 * @param epfd The epoll instance file descriptor.
	 * @param op The operation to perform.
	 * @param fd The file descriptor to add, modify or remove from the poll instance.
	 * @param event A pointer to a structure specifying the events of interest.
	 * @throw std::system_error An error occurred.
	 */
	void epoll_ctl(int epfd, int op, int fd, struct epoll_event *event = nullptr);

	/**
	 * Manipulates an epoll instance.
	 *
	 * @param epfd The epoll instance file descriptor.
	 * @param op The operation to perform.
	 * @param fd The file descriptor to add, modify or remove from the poll instance.
	 * @param events A bitwise combination of events of interest.
	 * @param user_data An opaque user data value.
	 * @throw std::system_error An error occurred.
	 */
	void epoll_ctl(int epfd, int op, int fd, std::uint32_t events, void *user_data = nullptr);

	/**
	 * Manipulates an epoll instance.
	 *
	 * @param epfd The epoll instance file descriptor.
	 * @param op The operation to perform.
	 * @param fd The file descriptor to add, modify or remove from the poll instance.
	 * @param events A bitwise combination of events of interest.
	 * @param user_data An opaque user data value.
	 * @throw std::system_error An error occurred.
	 */
	void epoll_ctl(int epfd, int op, int fd, std::uint32_t events, int user_data);

	/**
	 * Manipulates an epoll instance.
	 *
	 * @param epfd The epoll instance file descriptor.
	 * @param op The operation to perform.
	 * @param fd The file descriptor to add, modify or remove from the poll instance.
	 * @param events A bitwise combination of events of interest.
	 * @param user_data An opaque user data value.
	 * @throw std::system_error An error occurred.
	 */
	void epoll_ctl(int epfd, int op, int fd, std::uint32_t events, std::uint32_t user_data);

	/**
	 * Manipulates an epoll instance.
	 *
	 * @param epfd The epoll instance file descriptor.
	 * @param op The operation to perform.
	 * @param fd The file descriptor to add, modify or remove from the poll instance.
	 * @param events A bitwise combination of events of interest.
	 * @param user_data An opaque user data value.
	 * @throw std::system_error An error occurred.
	 */
	void epoll_ctl(int epfd, int op, int fd, std::uint32_t events, std::uint64_t user_data);

	/**
	 * Waits for events to occur on an epoll instance.
	 *
	 * @param epfd The epoll instance file descriptor.
	 * @param events A pointer to an array of structures to receive information about events.
	 * @param maxevents The number of elements in the @p events array.
	 * @param timeout The number of milliseconds
	 * @return The number of events that occurred, which may be zero.
	 * @throw std::invalid_argument @p timeout is out of range.
	 * @throw std::system_error An error occurred.
	 */
	unsigned epoll_wait(int epfd, struct epoll_event *events, int maxevents,
		std::chrono::milliseconds timeout = std::chrono::milliseconds(-1));

	/**
	 * Creates an event file descriptor.
	 *
	 * @param initval The initial value of the eventfd counter.
	 * @param flags Flags that affect the created event file descriptor.
	 */
	w::fd eventfd(unsigned initval = 0, int flags = 0);

	/**
	 * Reads the current counter value of an event file descriptor; see the man page of
	 * `eventfd()` for a description of the counter and blocking semantics.
	 *
	 * @param evfd The event file descriptor.
	 * @return The event counter value.
	 * @throws std::system_error An error occurred.
	 */
	std::uint64_t eventfd_read(int evfd);

	/**
	 * Modifies the counter value of an event file descriptor; see the man page of `eventfd()` for
	 * a description of the counter semantics.
	 *
	 * @param evfd The event file descriptor.
	 * @param value The event counter value.
	 * @throws std::system_error An error occurred.
	 */
	void eventfd_write(int evfd, std::uint64_t value);

	/**
	 * Creates a timer file descriptor.
	 *
	 * @param clockid The identifier of the clock on which the timer should be based.
	 * @param flags A bitwise combination of flags.
	 * @return The file descriptor for the created timer.
	 * @throw std::system_error An error occurred.
	 */
	w::fd timerfd_create(int clockid, int flags = 0);

	/**
	 * Sets the timeout and repetition interval of a timer file descriptor.
	 *
	 * @param fd The timer file descriptor to adjust.
	 * @param flags A bitwise combination of flags.
	 * @param new_value A pointer to a structure containing the repetition interval and initial
	 *        expiration time.
	 * @param old_value A pointer to a structure to be filled in with the previous repetition
	 *        interval and expiration time.
	 * @throw std::system_error An error occurred.
	 */
	void timerfd_settime(int fd, int flags,
		const struct itimerspec *new_value,
		struct itimerspec *old_value);

	/**
	 * Sets the timeout and repetition interval of a timer file descriptor.
	 *
	 * @param fd The timer file descriptor to adjust.
	 * @param flags A bitwise combination of flags.
	 * @param interval The repetition interval.
	 * @param initial The initial expiration time.
	 * @return A pair consisting of the previous repetition interval and initial expiration time,
	 *         in that order.
	 * @throw std::system_error An error occurred.
	 */
	std::pair<std::chrono::nanoseconds, std::chrono::nanoseconds>
	timerfd_settime(int fd, int flags,
		std::chrono::nanoseconds interval,
		std::chrono::nanoseconds initial);
}
