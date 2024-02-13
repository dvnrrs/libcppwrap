//
// libcppwrap - A collection of C++ wrappers for native APIs
// Copyright (C) 2021-2023 David A. Norris <danorris@gmail.com>
// Published under the MIT license - https://opensource.org/licenses/MIT
//

#include <cstddef>

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <w/assert.hpp>
#include <w/posix.hpp>

int w::fcntl(int fd, int cmd)
{
	return w::throw_if_eq(
		::fcntl(fd, cmd),
		-1,
		"file descriptor control failed");
}

int w::fcntl(int fd, int cmd, void *arg)
{
	return w::throw_if_eq(
		::fcntl(fd, cmd, arg),
		-1,
		"file descriptor control failed");
}

int w::fcntl(int fd, int cmd, const void *arg)
{
	return w::throw_if_eq(
		::fcntl(fd, cmd, arg),
		-1,
		"file descriptor control failed");
}

int w::ioctl(int fd, unsigned long request, void *arg)
{
	return w::throw_if_lt(
		::ioctl(fd, request, arg),
		0,
		"ioctl failed");
}

int w::ioctl(int fd, unsigned long request, const void *arg)
{
	return w::throw_if_lt(
		::ioctl(fd, request, arg),
		0,
		"ioctl failed");
}

std::size_t w::lseek(int fd, off_t offset, int whence)
{
	return static_cast<std::size_t>(
		w::throw_if_lt(
			::lseek(fd, offset, whence),
			static_cast<off_t>(0),
			"lseek failed"));
}

w::mmap_handle w::mmap(void *address, std::size_t length, int prot, int flags, int fd, off_t offset)
{
	void *actual_address = ::mmap(address, length, prot, flags, fd, offset);
	w::throw_if_eq<void *>(actual_address, nullptr, "failed to map file or device into memory");
	return w::memory_region { actual_address, length };
}

w::fd w::open(const char *pathname, int flags)
{
	return w::throw_if_eq(
		::open(pathname, flags),
		-1,
		"failed to open file");
}

w::fd w::open(const char *pathname, int flags, mode_t mode)
{
	return w::throw_if_eq(
		::open(pathname, flags, mode),
		-1,
		"failed to open file");
}

std::size_t w::read(int fd, void *buf, std::size_t count)
{
	return static_cast<std::size_t>(
		w::throw_if_lt(
			::read(fd, buf, count),
			ssize_t { 0 },
			"read error"));
}

std::size_t w::write(int fd, const void *buf, std::size_t count)
{
	return static_cast<std::size_t>(
		w::throw_if_lt(
			::write(fd, buf, count),
			ssize_t { 0 },
			"write error"));
}
