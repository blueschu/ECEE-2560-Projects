/**
 * EECE 2560 Project 2 - linked list declarations.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          David Cree      <cree.d@northeastern.edu>
 * Date:    2020-09-30
 *
 * References
 * ==========
 *
 *  [1] https://rust-unofficial.github.io/too-many-lists/index.html
 *  [2] https://en.cppreference.com/w/cpp/memory/unique_ptr
 *  [3] https://en.cppreference.com/w/cpp/container/forward_list
 *  [4] https://en.cppreference.com/w/cpp/named_req/ForwardIterator
 *  [5] https://stackoverflow.com/a/38103394
 *  [6] https://en.cppreference.com/w/cpp/iterator/iterator_traits
 *  [7] https://en.cppreference.com/w/cpp/language/rule_of_three
 *  [8] https://en.cppreference.com/w/cpp/utility/exchange
 *  [9] https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
 *  [10] https://en.cppreference.com/w/cpp/named_req/Container
 */

#ifndef EECE_2560_PROJECTS_LINKED_LIST_H
#define EECE_2560_PROJECTS_LINKED_LIST_H

#include "basic_unique.h"

#include <iterator>         // for iterator tag
#include <utility>          // for std::exchange (in move ctor)

/**
 * A singlely linked list.
 *
 * This implementation attempts to expose a similar interface to that of
 * `std::forward_list` from the C++ standard library.
 *
 * @tparam T The data type of elements stored in the list.
 */
template<typename T>
class LinkedList {

    /**
     * Helper class representing a linked list element with no data.
     *
     * This technique is adapted from one used in the implementation of the
     * C++ standard library included with my compiler. By defining separate
     * classes for nodes with data and nodes without data, we gain the ability
     * of having a unified interface for accessing the head and tail of the list.
     *
     * If we used only one Node class, the head pointer could not be treated as
     * a node, which would prevent the creation of iterators that reference the
     * location just before the list. This functionality is needed in order for
     * LinkedList<T>::insert_after to be able to manipulate the first location
     * in the list.
     */
    struct BaseNode {
        /// Owning pointer to the next node.
        BasicUnique<BaseNode> m_next_ptr;
    };

    /// Helper class representing a link in the linked list.
    struct Node : public BaseNode {
        /// The value contained in this node.
        T m_value;
    };

    /**
     *  Node containing a pointer to the first node in this linked list.
     *
     *  This pointer will be nullptr when the list is empty.
     */
    BaseNode m_head{};

  public:
    /**
     * A forward iterator over a linked list.
     *
     * This class attempts to implement a basic forward iterator [4,5].
     *
     * Note: this class needs to be a member class of Linked list since it needs
     * access to BaseNode/Node types that are not tainted by the qualifiers
     * that may be attached to Q. We cannot assume that, for example, that
     * BaseNode is the same as LinkedList<Q>::BaseNode since Q will
     * differ from T when this template is used to create a const iterator
     * (Q = const T).
     *
     * @tparam Q A potentially const-qualified version of the class template
     *           parameter T. This template parameter is required so that this
     *           iterator class can be used to instantiate both non-const and
     *           const iterator for LinkedList.
     */
    template<typename Q>
    struct LinkedListIterator {
        /**
         * The position of this iterator in the linked list.
         *
         * While declared as a BaseNode, this pointer MUST reference a Node
         * in order for dereferencing this iterator to be defined. This should
         * always be the case except in the events where this iterator is
         * represents a position directly before or after the list.
         */
        BaseNode* m_iter_pos{nullptr};

        /*
         * Standard aliases for iterator traits [6].
         */
        using value_type = Q;
        using pointer = Q*;
        using reference = Q&;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;

        /*
         * Default constructor.
         *
         * All members are already given in-class member initializers, so we can
         * just use the compiler generated default constructor [C.45,C.80 in 9].
         */
        LinkedListIterator() noexcept = default;

        // Construct an iterator from a base node pointer.
        explicit LinkedListIterator(BaseNode* base) noexcept: m_iter_pos{base} {}

        /**
         * Helper function for computing the iterator for the next element in
         * the list.
         *
         * @return Iterator to next element.
         */
        LinkedListIterator<Q> next() const noexcept
        {
            // If this iterator is not the end iterator (i.e., set to nullptr),
            // return an iterator to the node that follows the current node.
            return LinkedListIterator{m_iter_pos ? m_iter_pos->m_next_ptr.get() : nullptr};
        }

        /*
         * Dereference operator overload.
         *
         * We assume that this iterator points a valid Node (not a BaseNode),
         * which makes the static casts below safe.
         */
        reference operator*() const noexcept { return static_cast<Node*>(m_iter_pos)->m_value; }

        pointer operator->() const noexcept { return &static_cast<Node*>(m_iter_pos)->m_value; }

        /*
         * Comparison operators.
         */
        bool operator==(LinkedListIterator<Q> other) const noexcept { return m_iter_pos == other.m_iter_pos; }

        bool operator!=(LinkedListIterator<Q> other) const noexcept { return !(*this == other); }

        // Post-increment overload.
        LinkedListIterator<Q>& operator++() noexcept
        {
            m_iter_pos = m_iter_pos->m_next_ptr.get();
            return *this;
        }

        // Post-increment overload.
        LinkedListIterator<Q> operator++(int) noexcept
        {
            auto temp = *this;
            ++(*this);
            return temp;
        }

    }; // end struct iterator

    /*
     * Type aliases for containers [10].  We only provide those used in this
     * project.
     */
    using value_type = T;
    using iterator = LinkedListIterator<T>;
    using const_iterator = LinkedListIterator<const T>;
    using reference = T&;
    using const_reference = const T&;

    /*
     * Default constructor.
     *
     * All members are already given in-class member initializers, so we can
     * just use the compiler generated default constructor [C.45,C.80 in 9].
     */
    LinkedList() = default;

    /**
     * Range constructor.
     *
     * This constructor was added to simplify the code in the extra credit
     * portion of this lab.
     *
     * @tparam Iter Input iterator type.
     * @param it,end The range of elements to be inserted.
     */
    template<typename Iter>
    LinkedList(Iter it, Iter end)
    {
        auto back_it = before_begin();
        while (it != end) {
            back_it = insert_after(back_it, *it);
            ++it;
        }
    }

    /*
     * Copy constructor and assignment operator were not necessary for this
     * lab.
     *
     * To prevent accidents, we explicitly disallow the compiler from generating
     * them [C.21,C.81 in 9].
     */
    LinkedList(const LinkedList&) = delete;

    LinkedList& operator=(const LinkedList&) = delete;

    /*
     * Move constructor [7, C.66 in 9].
     *
     * Moves are required in our implementation of the extra credit portion
     * of this lab.
     */
    LinkedList(LinkedList&& other) noexcept
        : m_head{std::exchange(other.m_head, BaseNode{})} {}

    // Move assignment [7, C.66 in 9].
    LinkedList& operator=(LinkedList&& other) noexcept
    {
        // Allow a specialized swap to be found through argument-dependent
        // lookup if we later define one [C.165 in 9].
        using std::swap;
        swap(m_head, other.m_head);
        return *this;
    }

    /**
     * Returns the first element in this list.
     *
     * The behavior of this function is not defined if this list is empty.
     *
     * @return First element.
     */
    [[nodiscard]]
    reference front() { return *begin(); }

    /**
    * Returns the first element in this list.
    *
    * The behavior of this function is not defined if this list is empty.
    *
    * @return First element.
    */
    [[nodiscard]]
    const_reference front() const { return *begin(); }

    /**
     * Returns true if this list contains no elements.
     *
     * Runs in O(1) time.
     *
     * @return True if this list is empty.
     */
    [[nodiscard]]
    bool empty() const noexcept { return m_head.m_next_ptr.get() == nullptr; }

    /**
    * Inserts the given element into this linked last at the position
    * immediately following the provided iterator.
    *
    * This function is named after the analogous function in std::forward_list.
    *
    * Runs in O(1) time.
    *
    * @param position Iterator preceding the insertion position.
    * @param value Element to be inserted.
    * @return Iterator to the inserted element.
    */
    iterator insert_after(iterator position, const T& value);

    /**
     * Inserts the given element at the front of this list.
     *
     * This function is named after the analogous function in std::forward_list.
     *
     *  Runs in O(1) time.
     *
     * @param value Element to be inserted.
     */
    void push_front(const T& value);

    /**
     * Removes the element immediately following the given position.
     *
     * This function is analogous to std::forward_iterator::erase_after().
     *
     *  Runs in O(1) time.
     *
     * @param position Iterating preceding the element to be removed.
     */
    void remove_after(iterator position);

    /**
     * Removes the first element from this list.
     *
     * This function emulates the behavior of the function with the same name
     * in std::forward_list. If we instead wrote this linked list implementation
     * to be incompatible with std::forward_list, this member function could
     * return an optional value instead. We take this approach in Deck::deal.
     *
     *  Runs in O(1) time.
     */
    void pop_front() { remove_after(before_begin()); }

    /**
     * Removes all elements from this list.
     *
     * This function is named after the analogous function in std::forward_list.
     *
     * Runs in O(N) time.
     */
    void clear();

    /**
     * Returns an iterator that represents an entry just before the beginning
     * of the list.
     *
     * This technique was discovered by examining the implementation of
     * std::forward_list.
     *
     * @return Iterator before the beginning of this list.
     */
    iterator before_begin() noexcept
    {
        return iterator{&m_head};
    }

    /**
     * Returns an iterator that represents an entry just before the beginning
     * of the list.
     *
     * This technique was discovered by examining the implementation of
     * std::forward_list.
     *
     * @return Iterator before the beginning of this list.
     */
    const_iterator before_begin() const noexcept
    {
        return const_iterator{&m_head};
    }

    /**
     * Returns an iterator representing the first element in this list.
     *
     * @return First element iterator.
     */
    iterator begin() noexcept
    {
        return iterator{m_head.m_next_ptr.get()};
    }

    /**
    * Returns an iterator representing the first element in this list.
    *
    * @return First element iterator.
    */
    const_iterator begin() const noexcept
    {
        return const_iterator{m_head.m_next_ptr.get()};
    }

    /**
     * Returns an iterator representing the end of this list.
     *
     * @return End iterator.
     */
    iterator end() noexcept
    {
        return iterator{nullptr};
    }

    /**
     * Returns an iterator representing the end of this list.
     *
     * @return End iterator.
     */
    const_iterator end() const noexcept
    {
        return const_iterator{nullptr};
    }
}; // end class LinkedList

#include "linked_list.tpp"

#endif //EECE_2560_PROJECTS_LINKED_LIST_H
