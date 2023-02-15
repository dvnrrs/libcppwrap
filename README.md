[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![build](https://github.com/dvnrrs/libcppwrap/actions/workflows/build.yml/badge.svg)](https://github.com/dvnrrs/libcppwrap/actions/workflows/build.yml)
[![doxygen](https://github.com/dvnrrs/libcppwrap/actions/workflows/doxygen.yml/badge.svg)](https://github.com/dvnrrs/libcppwrap/actions/workflows/doxygen.yml)

# libcppwrap<br><small><sub>A collection of C++ wrappers for native APIs</sub></small>

`libcppwrap` is a collection of low-level C++ wrappers around native C APIs such as POSIX
functions and BSD sockets. It provides the following features on top of the native functions,
while duplicating the function signatures as closely as possible:

* Errors are thrown as `std::system_error` exceptions instead of returning error codes.
* Since return values are no longer used for error reporting, output parameters can be returned as
  values instead, wherever practical.
* The [RAII](https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization) pattern is
  applied to resources such as file handles, preventing leaks, using a sophisticated and flexible
  `w::handle` type.
* Functions such as `fcntl()` and `ioctl()`, which can take input and/or output parameters of
  arbitrary type, are implemented as type-safe templates.
* Derived POD structures for types such as `sockaddr_in` are provided with useful constructors for
  initialization. Use of the derived structures is always optional.

The philosophy of the library is to stay as faithful as possible to the native API, while ensuring
type- and exception-safety and resource management as robustly as possible. Some helper functions
which deviate from the native APIs or add missing functionality are implemented in a separate,
optional namespace, which can be disabled.

The library is currently very POSIX-centric, being developed for and tested on Linux. It could and
may eventually be extended to include native APIs for other platforms like Windows.

# Usage

`libcppwrap` exposes two namespaces, `w` and `wx`. The `w` namespace contains the core wrappers,
which follow the native APIs as closely as possible. The `wx` namespace contains useful extensions
which deviate from the native API but provide low-level functionality that would not necessarily
justify a separate library.

    #include <w/posix.hpp>

	auto fd = w::open("file.txt", "at");
	w::write(fd, "hello", 5);
	// fd is automatically closed when it goes out of scope

See the **[Doxygen API reference](https://dvnrrs.github.io/libcppwrap/)** for details.

# Building

`libcppwrap` has no dependencies, requiring only a C++ compiler that supports C++17.

It can be built easily with [CMake](https://cmake.org/):

    mkdir build
	cd build
	cmake ..
	make
	sudo make install

# License

`libcppwrap` is published under the [MIT License](https://opensource.org/license/mit/). See
[LICENSE.txt](LICENSE.txt) for the full license text.
