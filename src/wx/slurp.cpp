//
// libcppwrap - A collection of C++ wrappers for native APIs
// Copyright (C) 2021-2023 David A. Norris <danorris@gmail.com>
// Published under the MIT license - https://opensource.org/licenses/MIT
//

#include <cstddef>
#include <fstream>
#include <ios>
#include <stdexcept>
#include <string>
#include <string_view>

#include <wx/slurp.hpp>
#include <wx/string.hpp>

using namespace std::string_literals;

std::string wx::read_file_as_string(const char *path)
{
	// Note: It's impossible to determine the length of some files (notably, sysfs) in advance, so
	// we have to use a dynamically-growing buffer strategy. The default initial buffer size must
	// be chosen to be large enough to accommodate a reasonable proportion of files without
	// reallocation but not too large to waste memory.

	std::size_t buffer_size = 32;

	std::ifstream file;
	file.open(path, std::ios::binary);

	if (!file)
		throw std::runtime_error("failed to open '"s + path + "'");

	file.exceptions(std::ios_base::badbit | std::ios_base::failbit);

	std::string contents(buffer_size, '\0');
	std::size_t size = 0;

	file.exceptions(std::ios_base::badbit);

	while (file.read(&contents[size], buffer_size - size))
	{
		size = buffer_size;
		contents.resize(buffer_size *= 2);
	}

	contents.resize(size + file.gcount());
	return contents;
}

std::string wx::slurp(const char *path)
{
	std::string contents = read_file_as_string(path);
	wx::rtrim(contents);
	return contents;
}

void wx::spew(const char *path, std::string_view str)
{
	std::ofstream file;
	file.exceptions(std::ios_base::badbit | std::ios_base::failbit);
	file.open(path, std::ios::binary);
	file.write(str.data(), str.size());
}
