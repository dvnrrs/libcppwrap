//
// libcppwrap - A collection of C++ wrappers for native APIs
// Copyright (C) 2021-2023 David A. Norris <danorris@gmail.com>
// Published under the MIT license - https://opensource.org/licenses/MIT
//

#pragma once

#include <cstddef>

#include <w/handle.hpp>

#include <unistd.h>
#include <sys/mman.h>

namespace w
{
	/**
	 * An RAII util::handle type for POSIX file descriptors.
	 */
	typedef w::handle<int, -1, ::close> fd;

	/**
	 * A structure holding details about a memory-mapped file or device. Such a structure is the
	 * resource type of a w::mmap_handle RAII handle.
	 */
	struct memory_region
	{
		void *address;			/**< The (actual, not requested) base address of the mapping. */
		std::size_t length;		/**< The length of the mapping in bytes. */

		/**
		 * Constructs a memory region.
		 * @param address The (actual, not requested) base address of the mapping.
		 * @param The length of the mapping in bytes.
		 */
		constexpr memory_region(void *address, std::size_t length) noexcept : address(address), length(length) { }

		/**
		 * Constructs an empty memory region.
		 */
		constexpr memory_region() noexcept : memory_region(nullptr, 0) { }

		/**
		 * Compares two memory regions.
		 * @param rhs A reference to the memory region to compare.
		 * @return `false` if this object and @p rhs both have the same address and length.
		 */
		constexpr bool operator!=(const memory_region& rhs) const noexcept { return rhs.address != address || rhs.length != length; }
	};

	namespace detail
	{
		inline void munmap(const memory_region& region)
		{
			::munmap(region.address, region.length);
		}
	}

	/**
	 * An RAII util::handle type for mmap() entries.
	 */
	typedef w::handle<memory_region, memory_region { }, detail::munmap> mmap_handle;

	/**
	 * Controls a file descriptor.
	 *
	 * @param fd The file descriptor to control.
	 * @param cmd The control command code.
	 * @return A result code from the command.
	 * @throw std:system_error An error occurred.
	 */
	int fcntl(int fd, int cmd);

	/**
	 * Controls a file descriptor.
	 *
	 * @param fd The file descriptor to control.
	 * @param cmd The control command code.
	 * @param arg A pointer to a command-specific input argument.
	 * @return A result code from the command.
	 * @throw std:system_error An error occurred.
	 */
	int fcntl(int fd, int cmd, void *arg);

	/**
	 * Controls a file descriptor.
	 *
	 * @param fd The file descriptor to control.
	 * @param cmd The control command code.
	 * @param arg A pointer to a command-specific output argument.
	 * @return A result code from the command.
	 * @throw std:system_error An error occurred.
	 */
	int fcntl(int fd, int cmd, const void *arg);

	/**
	 * Controls a file descriptor.
	 *
	 * @tparam Argument The type of the command-specific argument.
	 * @param fd The file descriptor to control.
	 * @param cmd The control command code.
	 * @param arg A reference to a command-specific input argument.
	 * @return A result code from the command.
	 * @throw std:system_error An error occurred.
	 */
	template <typename Argument>
	int fcntl(int fd, int cmd, const Argument& arg)
	{
		return w::fcntl(fd, cmd, &arg);
	}

	/**
	 * Controls a file descriptor.
	 *
	 * @tparam Argument The type of the command-specific argument.
	 * @param fd The file descriptor to control.
	 * @param cmd The control command code.
	 * @return An @p Argument filled in by the command.
	 * @throw std:system_error An error occurred.
	 */
	template <typename Argument>
	Argument fcntl(int fd, int cmd)
	{
		Argument arg;
		w::fcntl(fd, cmd, &arg);
		return arg;
	}

	/**
	 * Controls a device.
	 *
	 * @param fd The file descriptor of the device to control.
	 * @param request The control request code.
	 * @param arg A pointer to a request-specific output argument.
	 * @return A nonnegative result code from the control request.
	 * @throw std:system_error An error occurred.
	 */
	int ioctl(int fd, unsigned long request, void *arg);

	/**
	 * Controls a device.
	 *
	 * @param fd The file descriptor of the device to control.
	 * @param request The control request code.
	 * @param arg A pointer to a request-specific input argument.
	 * @return A nonnegative result code from the control request.
	 * @throw std:system_error An error occurred.
	 */
	int ioctl(int fd, unsigned long request, const void *arg);

	/**
	 * Controls a device.
	 *
	 * @tparam Argument The type of @p arg.
	 * @param fd The file descriptor of the device to control.
	 * @param request The control request code.
	 * @param arg A pointer to a request-specific input argument.
	 * @return A nonnegative result code from the control request.
	 * @throw std:system_error An error occurred.
	 */
	template <typename Argument>
	int ioctl(int fd, unsigned request, const Argument& arg)
	{
		return w::ioctl(fd, request, &arg);
	}

	/**
	 * Controls a device.
	 *
	 * @tparam Argument The type of the request's output value.
	 * @param fd The file descriptor of the device to control.
	 * @param request The control request code.
	 * @return The output value.
	 * @throw std:system_error An error occurred.
	 */
	template <typename Argument>
	Argument ioctl(int fd, unsigned request)
	{
		Argument arg;
		w::ioctl(fd, request, &arg);
		return arg;
	}

	/**
	 * Sets the position of a file pointer for a file descriptor.
	 *
	 * @param fd The file descriptor to seek in.
	 * @param offset The offset to seek to.
	 * @param whence `SEEK_SET`, `SEEK_CUR` or `SEEK_END`.
	 * @return The new offset relative to the beginning of the file.
	 * @throw std::system_error An error occurred.
	 */
	std::size_t lseek(int fd, off_t offset, int whence);

	/**
	 * Maps a file or device into memory.
	 *
	 * @param address The requested virtual address to map the file or device into.
	 * @param length The number of bytes to map.
	 * @param prot The protection flags for the mapping.
	 * @param flags Additional flags.
	 * @param fd The file descriptor of the file or device to map.
	 * @param offset The offset in bytes of the file or device to map.
	 * @return An RAII handle for the mapping.
	 * @throw std::system_error An error occurred.
	 */
	w::mmap_handle mmap(void *address, std::size_t length, int prot, int flags, int fd, off_t offset);

	/**
	 * Opens and possibly creates a file.
	 *
	 * @param pathname The path of the file to open and/or create.
	 * @param flags A bitwise combination of flags.
	 * @return A file descriptor for the opened file.
	 * @throw std:system_error An error occurred.
	 */
	w::fd open(const char *pathname, int flags);

	/**
	 * Opens and possibly creates a file.
	 *
	 * @param pathname The path of the file to open and/or create.
	 * @param flags A bitwise combination of flags.
	 * @param mode The access mode for the file.
	 * @return A file descriptor for the opened file.
	 * @throw std:system_error An error occurred.
	 */
	w::fd open(const char *pathname, int flags, mode_t mode);

	/**
	 * Reads data from a file descriptor.
	 *
	 * @param fd The file descriptor to read from.
	 * @param buf A pointer to an array where read data should be stored.
	 * @param count The maximum number of bytes to read.
	 * @return The number of bytes actually read, which may be zero.
	 * @throw std::system_error An error occurred. This includes `EAGAIN` on non-blocking files.
	 */
	std::size_t read(int fd, void *buf, std::size_t count);

	/**
	 * Writes data to a file descriptor.
	 *
	 * @param fd The file descriptor to write to.
	 * @param buf A pointer to an array of data to write.
	 * @param count The maximum number of bytes to write.
	 * @return The number of bytes actually written, which may be zero.
	 * @throw std::system_error An error occurred. This includes `EAGAIN` on non-blocking files.
	 */
	std::size_t write(int fd, const void *buf, std::size_t count);
}
