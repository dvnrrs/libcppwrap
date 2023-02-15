//
// libcppwrap - A collection of C++ wrappers for native APIs
// Copyright (C) 2021-2023 David A. Norris <danorris@gmail.com>
// Published under the MIT license - https://opensource.org/licenses/MIT
//

#pragma once

#include <type_traits>
#include <utility>

namespace w
{
	/**
	 * An RAII handle type which owns an opaque resource of a specified type, and calls a
	 * designated function to close the resource upon destruction. The type is moveable but not
	 * copyable, and provides semantics similar to std::unique_ptr. Most members are `noexcept`
	 * for underlying resource types which are nothrow-swappable and nothrow-copy-constructible.
	 *
	 * @tparam Resource The type of the resource to own. This type must be nothrow-comparable,
	 * 	       nothrow-copy-constructible and nothrow-swappable.
	 * @tparam Closed A special value which indicates a closed resource.
	 * @tparam CloseFunc A function which closes a resource. The function must not throw
	 *         exceptions, and preferably should be marked `nothrow`. The function must either
	 *         take no arguments or a single argument of type @p Resource. Any value returned by
	 *         the function is ignored.
	 */
	template <
		typename	Resource,
		Resource	Closed,
		auto		CloseFunc,
		std::enable_if_t<
			std::is_nothrow_copy_constructible_v<Resource> &&
			std::is_nothrow_swappable_v<Resource> &&
			std::is_function_v<std::remove_pointer_t<decltype(CloseFunc)>> && (
				std::is_invocable_v<decltype(CloseFunc), Resource> ||
				std::is_invocable_v<decltype(CloseFunc)>)> * = nullptr>

	class handle
	{
		public:

			/**
			 * Constructs a handle which does not own a resource.
			 */
			handle() noexcept : handle(Closed) { }

			/**
			 * Constructs a handle which owns the specified resource.
			 *
			 * @param resource The resource to own.
			 */
			handle(Resource resource) noexcept : _resource(resource) { }

			handle(const handle&) = delete;
			handle& operator=(const handle&) = delete;

			/**
			 * Constructs a handle which takes ownership of a resource (if any) from another
			 * handle.
			 *
			 * @param other The other handle whose resource (if any) to take ownership of.
			 */
			handle(handle&& other) noexcept : _resource(Closed)
			{
				std::swap(_resource, other._resource);
			}

			/**
			 * Releases the resource (if any) owned by this handle and takes ownership of a
			 * resource (if any) from another handle.
			 *
			 * @param other The other handle whose resource (if any) to take ownership of.
			 * @return A reference to this object.
			 */
			handle& operator=(handle&& other) noexcept
			{
				if (&other != this)
				{
					close();
					std::swap(_resource, other._resource);
				}

				return *this;
			}

			/**
			 * Closes the resource (if any) owned by this handle.
			 */
			~handle() noexcept { close(); }

			/**
			 * Returns the resource (if any) owned by this handle.
			 *
			 * @return The owned resource, or the special value @p Closed if this handle does not
			 *         own a resource.
			 */
			Resource get() noexcept { return _resource; }

			/**
			 * Returns the resource (if any) owned by this handle.
			 *
			 * @return The owned resource, or the special value @p Closed if this handle does not
			 *         own a resource.
			 */
			const Resource get() const noexcept { return _resource; }

			/**
			 * Returns the resource (if any) owned by this handle.
			 *
			 * @return The owned resource, or the special value @p Closed if this handle does not
			 *         own a resource.
			 */
			operator Resource() noexcept { return _resource; }

			/**
			 * Returns the resource (if any) owned by this handle.
			 *
			 * @return The owned resource, or the special value @p Closed if this handle does not
			 *         own a resource.
			 */
			operator const Resource() const noexcept { return _resource; }

			/**
			 * Tests whether this handle owns a resource.
			 *
			 * @return `true` if this handle owns a resource.
			 */
			operator bool() const noexcept { return Closed != _resource; }

			/**
			 * Closes the resource (if any) owned by this handle by calling @p CloseFunc. After
			 * the call, the handle no longer owns a resource. @p CloseFunc is only called if the
			 * handle owns a resource.
			 */
			void close() noexcept
			{
				Resource resource(Closed);
				std::swap(resource, _resource);
				if (Closed != resource)
				{
					if constexpr (std::is_invocable_v<decltype(CloseFunc), Resource>)
						CloseFunc(resource);
					else
						CloseFunc();
				}
			}

			/**
			 * Releases (without closing) and returns the underlying resource.
			 *
			 * @return The underlying resource.
			 */
			Resource release() noexcept
			{
				Resource resource = Closed;
				std::swap(resource, _resource);
				return resource;
			}

		private:

			Resource _resource;
	};
}
