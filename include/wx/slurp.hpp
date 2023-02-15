//
// libcppwrap - A collection of C++ wrappers for native APIs
// Copyright (C) 2021-2023 David A. Norris <danorris@gmail.com>
// Published under the MIT license - https://opensource.org/licenses/MIT
//

#pragma once

#include <filesystem>
#include <string>
#include <string_view>

namespace wx
{
    /**
     * Reads the contents of a file as a string.
     *
     * @remarks This function attempts to minimize allocations and copies. It reads directly into
     *          a std::string buffer which is dynamically resized as needed. The size of the file
     *          need not be known in advance, so this function supports streamed files such as
     *          `sysfs` entries and pipes.
     *
     * @param path The path of the file to read.
     * @return The contents of the file as a string.
     * @throw std::ios_base::failure The file cannot be opened or a read error occurred.
     *
     * @see slurp()
     */
    std::string read_file_as_string(const char *path);

    /**
     * @copydoc read_file_as_string
     */
    inline std::string read_file_as_string(const std::string& path) { return read_file_as_string(path.c_str()); }

    /**
     * Reads the contents of a file as a string with trailing whitespace removed.
     *
     * @param path The path of the file to read.
     * @return The contents of the file as a string.
     * @throw std::ios_base::failure The file cannot be opened or a read error occurred.
     *
     * @see read_file_as_string()
     */
    std::string slurp(const char *path);

    /**
     * @copydoc slurp
     */
    inline std::string slurp(const std::string& path) { return slurp(path.c_str()); }

    /**
     * @copydoc slurp
     */
    inline std::string slurp(const std::filesystem::path& path) { return slurp(path.c_str()); }

    /**
     * Writes a string to a file.
     *
     * @param path The path of the file to write.
     * @param str The string to write.
     * @throw std::ios_base::failure The file cannot be opened or a write error occurred.
     */
    void spew(const char *path, std::string_view str);

    /**
     * @copydoc spew
     */
    inline void spew(const std::string& path, std::string_view str) { return spew(path.c_str(), str); }

    /**
     * @copydoc spew
     */
    inline void spew(const std::filesystem::path& path, std::string_view str) { return spew(path.c_str(), str); }
}
