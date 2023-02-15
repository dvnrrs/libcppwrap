//
// libcppwrap - A collection of C++ wrappers for native APIs
// Copyright (C) 2021-2023 David A. Norris <danorris@gmail.com>
// Published under the MIT license - https://opensource.org/licenses/MIT
//

#pragma once

#include <iterator>

namespace w
{
	/**
	 * A forward iterator for linked lists.
	 *
	 * @tparam ListNode The type of the linked list node.
	 * @tparam Next A mutator function which, given a pointer to a linked list node, returns a
	 *         pointer to the next linked list node.
	 */
	template <typename ListNode, const ListNode * (*Next)(const ListNode *)>
	struct const_list_iterator
	{
		/**
		 * Constructs an end iterator.
		 */
		const_list_iterator() noexcept : _current(nullptr) { }

		/**
		 * Constructs an iterator pointing to the specified linked list node.
		 *
		 * @param current The linked list node to point to.
		 */
		const_list_iterator(const ListNode *current) noexcept : _current(current) { }

		/**
		 * Advances to the next linked list node. The behavior is undefined if this is an end
		 * iterator.
		 *
		 * @return A reference to this object.
		 */
		const_list_iterator& operator++() noexcept
		{
			_current = Next(_current);
			return *this;
		}

		/**
		 * Gets a reference to the linked list node pointed to by this iterator. The behavior is
		 * undefined if this is an end iterator.
		 *
		 * @return a reference to the linked list node pointed to by this iterator
		 */
		const ListNode& operator*() const noexcept { return *_current; }

		/**
		 * Gets a pointer to the linked list node pointed to by this iterator.
		 *
		 * @return a pointer to the linked list node pointed to by this iterator, or `nullptr` if
		 *         this is an end iterator.
		 */
		const ListNode *operator->() const noexcept { return _current; }

		/**
		 * Tests if this object and @p rhs point to the same linked list node.
		 *
		 * @param rhs The other iterator to compare.
		 * @return `true` if and only if this object and @p rhs point to the same linked list
		 *         node.
		 */
		bool operator==(const_list_iterator const& rhs) const noexcept { return _current == rhs._current; }

		/**
		 * Tests if this object and @p rhs point to the same linked list node.
		 *
		 * @param rhs The other iterator to compare.
		 * @return `true` if and only if this object and @p rhs point to different linked list
		 *         nodes.
		 */
		bool operator!=(const_list_iterator const& rhs) const noexcept { return _current != rhs._current; }

		private: const ListNode *_current;
	};
}

template <typename ListNode, const ListNode * (*Next)(const ListNode *)>
struct std::iterator_traits<w::const_list_iterator<ListNode, Next>>
{
    typedef std::ptrdiff_t				difference_type;
    typedef ListNode					value_type;
    typedef const ListNode *			pointer;
    typedef const ListNode&				reference;
    typedef std::forward_iterator_tag	iterator_category;
};
