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

#include <vector>

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
     * This technique is adapted from one used in the C++ standard library
     * sources included with my compiler. By defining separate classes for
     * nodes with data and nodes without data, we gain the ability of having
     * a unified interface for accessing the head and tail of the list.
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
     *  Pointer to the first node in this linked list.
     *
     *  The pointer will be nullptr when the list is empty.
     */
    BaseNode m_head{};

  public:
    /**
     * A forward iterator over a linked list.
     *
     * This class attempts to implement a basic forward iterator [4,5].
     */
    struct iterator {
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
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;

        /*
         * Default constructor.
         *
         * All members are already given in-class member initializers, so we can
         * just use the compiler generated default constructor [C.45,C.80 in 9].
         */
        iterator() noexcept = default;

        // Construct an iterator from a base node pointer.
        explicit iterator(BaseNode* base) noexcept: m_iter_pos{base} {}

        /**
         * Helper function for computing the iterator for the next element in
         * the list.
         *
         * @return Iterator to next element.
         */
        iterator next() const noexcept
        {
            // If this iterator is not the end iterator (i.e., set to nullptr),
            // return an iterator to the node that follows the current node.
            return iterator{m_iter_pos ? m_iter_pos->m_next_ptr.get() : nullptr};
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
        bool operator==(iterator other) const noexcept { return m_iter_pos == other.m_iter_pos; }

        bool operator!=(iterator other) const noexcept { return !(*this == other); }

        // Post-increment overload.
        iterator& operator++() noexcept
        {
            m_iter_pos = m_iter_pos->m_next_ptr.get();
            return *this;
        }

        // Post-increment overload.
        iterator operator++(int) noexcept
        {
            auto temp = *this;
            ++(*this);
            return temp;
        }

    }; // end struct iterator

    /*
     * Type alias for containers [10].  We only provide those used in this
     * project.
     */
    using value_type = T;


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
        auto out_end = before_begin();
        while (it != end) {
            out_end = insert_after(out_end, *it);
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
     * Returns an iterator representing the first element in this list.
     *
     * @return First element iterator.
     */
    iterator begin() noexcept
    {
        return iterator{m_head.m_next_ptr.get()};
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
     * Removes the elements immediately following the given position.
     *
     * This function is analogous to std::forward_iterator::erase_after().
     *
     *  Runs in O(1) time.
     *
     * @param position Iterating preceding the element to be removed.
     */
    void remove_after(iterator position);


    void clear();

};

#include "linked_list.tpp"

#endif //EECE_2560_PROJECTS_LINKED_LIST_H
