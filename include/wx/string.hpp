//
// libcppwrap - A collection of C++ wrappers for native APIs
// Copyright (C) 2021-2023 David A. Norris <danorris@gmail.com>
// Published under the MIT license - https://opensource.org/licenses/MIT
//

#pragma once

#include <algorithm>
#include <cctype>
#include <cerrno>
#include <cstdlib>
#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace wx
{
    /**
     * Parses a string as a numeric value with strict formatting and range checks.
     *
     * @tparam T The numeric type to convert to.
     * @param str The string to convert.
     * @param base The numeric base.
     * @return The parsed value.
     * @throw std::range_error The string is valid but the number is out of range.
     * @throw std::runtime_error The string is invalid.
     */
    template <typename T>
    typename std::enable_if<std::is_integral_v<T> && std::is_unsigned_v<T>, T>::type
    number(const char *str, int base = 10)
    {
        char *end;
        errno = 0;
        unsigned long long l = std::strtoull(str, &end, base);
        if (end == str || *end)
            throw std::runtime_error("invalid numeric string");
        else if (l > std::numeric_limits<T>::max() || (l == std::numeric_limits<unsigned long long>::max() && errno == ERANGE))
            throw std::range_error("number is out of range");
        return l;
    }

    /**
     * @copydoc number
     */
    template <typename T>
    typename std::enable_if<std::is_integral_v<T> && std::is_signed_v<T>, T>::type
    number(const char *str, int base = 10)
    {
        char *end;
        errno = 0;
        long long l = std::strtoll(str, &end, base);
        if (end == str || *end)
            throw std::runtime_error("invalid numeric string");
        else if (l > std::numeric_limits<T>::max() || (l == std::numeric_limits<long long>::max() && errno == ERANGE))
            throw std::range_error("number is out of range");
        else if (l < std::numeric_limits<T>::min() || (l == std::numeric_limits<long long>::min() && errno == ERANGE))
            throw std::range_error("number is out of range");
        return l;
    }

    /**
     * Parses a string as a numeric value with strict formatting and range checks.
     *
     * @tparam T The numeric type to convert to.
     * @param str The string to convert.
     * @return The parsed value.
     * @throw std::range_error The string is valid but the number is out of range.
     * @throw std::runtime_error The string is invalid.
     */
    template <typename T>
    typename std::enable_if<std::is_floating_point_v<T>, T>::type
    number(const char *str)
    {
        char *end;
        errno = 0;
        double d = std::strtod(str, &end);
        if (end == str || *end)
            throw std::runtime_error("invalid numeric string");
        else if (errno == ERANGE)
            throw std::range_error("number is out of range");
        return d;
    }

    /**
     * @copydoc number
     */
    template <typename T>
    typename std::enable_if<std::is_integral_v<T>, T>::type
    number(const std::string& str, int base = 10)
    {
        return number<T>(str.c_str(), base);
    }

    /**
     * Parses a string as a numeric value with strict formatting and range checks.
     *
     * @tparam T The numeric type to convert to.
     * @param str The string to convert.
     * @return The parsed value.
     * @throw std::range_error The string is valid but the number is out of range.
     * @throw std::runtime_error The string is invalid.
     */
    template <typename T>
    typename std::enable_if<std::is_floating_point_v<T>, T>::type
    number(const std::string& str)
    {
        return number<T>(str.c_str());
    }

    /**
     * Removes trailing whitespace from a string in-place.
     *
     * @param str The string from which to remove trailing whitespace.
     * @return A reference to @p str.
     */
    static inline std::string& rtrim(std::string& str) noexcept
    {
        auto from = std::find_if(
            str.rbegin(),
            str.rend(),
            [](unsigned char ch) { return !std::isspace(ch) && ch != '\0'; });

        str.erase(from.base(), str.end());

        return str;
    }
}
