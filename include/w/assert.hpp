//
// libcppwrap - A collection of C++ wrappers for native APIs
// Copyright (C) 2021-2023 David A. Norris <danorris@gmail.com>
// Published under the MIT license - https://opensource.org/licenses/MIT
//

#pragma once

#include <cerrno>
#include <system_error>

namespace w
{
	/**
	 * Throw a std::system_error with `errno` if @p value `==` @p x, otherwise return @p value.
	 *
	 * @tparam T The type of @p value and @p x.
	 * @param value The value to check.
	 * @param x The value to test for.
	 * @param message The message to include in the exception.
	 * @return @p value.
	 * @throw std::system_error @p value `==` @p x. The error code is set to `errno`.
	 */
	template <typename T>
	T throw_if_eq(T value, T x, const char *message)
	{
		if (value == x)
			throw std::system_error(errno, std::generic_category(), message);
		return value;
	}

	/**
	 * Throw a std::system_error with `errno` if @p value `<` @p x, otherwise return @p value.
	 *
	 * @tparam T The type of @p value and @p x.
	 * @param value The value to check.
	 * @param x The value to test for.
	 * @param message The message to include in the exception.
	 * @return @p value.
	 * @throw std::system_error @p value `<` @p x. The error code is set to `errno`.
	 */
	template <typename T>
	T throw_if_lt(T value, T x, const char *message)
	{
		if (value < x)
			throw std::system_error(errno, std::generic_category(), message);
		return value;
	}

	/**
	 * Throw a std::system_error with `errno` if @p value `!=` @p x, otherwise return @p value.
	 *
	 * @tparam T The type of @p value and @p x.
	 * @param value The value to check.
	 * @param x The value to test for.
	 * @param message The message to include in the exception.
	 * @return @p value.
	 * @throw std::system_error @p value `!=` @p x. The error code is set to `errno`.
	 */
	template <typename T>
	T throw_if_ne(T value, T x, const char *message)
	{
		if (value != x)
			throw std::system_error(errno, std::generic_category(), message);
		return value;
	}

	/**
	 * Throw a std::system_error with `errno` if @p value `!= 0`, otherwise return @p value.
	 *
	 * @tparam T The type of @p value.
	 * @param value The value to check.
	 * @param message The message to include in the exception.
	 * @return @p value.
	 * @throw std::system_error @p value `!= 0`. The error code is set to `errno`.
	 */
	template <typename T>
	T throw_if_nz(T value, const char *message)
	{
		if (value != 0)
			throw std::system_error(errno, std::generic_category(), message);
		return value;
	}

	/**
	 * Throw a std::system_error with `errno` if @p value `== 0`, otherwise return @p value.
	 *
	 * @tparam T The type of @p value.
	 * @param value The value to check.
	 * @param message The message to include in the exception.
	 * @return @p value.
	 * @throw std::system_error @p value `== 0`. The error code is set to `errno`.
	 */
	template <typename T>
	T throw_if_z(T value, const char *message)
	{
		if (value == 0)
			throw std::system_error(errno, std::generic_category(), message);
		return value;
	}
}
