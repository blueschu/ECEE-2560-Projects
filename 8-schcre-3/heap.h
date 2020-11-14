/**
 * A heap / heapsort implementation.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-11-02
 *
 *  References
 * ===========
 *  [1] https://en.cppreference.com/w/cpp/named_req/Container
 */

#ifndef EECE_2560_PROJECTS_HEAP_H
#define EECE_2560_PROJECTS_HEAP_H

#include <algorithm>            // for std::iter_swap
#include <functional>           // for std::less
#include <iterator>             // for std::distance, std::iterator_traits
#include <type_traits>          // for std::is_base_of_v
#include <vector>               // for std::vector

namespace details {
/**
 * Returns the "left child" of the given position in the heap.
 *
 * If the left child does not exist, the end iterator of the range will be
 * returned. This function avoids undefined behavior from incrementing an
 * end iterator.
 */
template<typename Iter>
constexpr Iter heap_child_left(Iter start, Iter end, Iter pos)
{
    auto next_index = 2 * (std::distance(start, pos) + 1) - 1;
    // We cannot increment a random access iterator past its end value.
    if (next_index <= std::distance(start, end)) {
        return start + next_index;
    } else {
        return end;
    }
}

/**
 * Returns the "right child" of the given position in the heap.
 *
 * If the right child does not exist, the end iterator of the range will be
 * returned. This function avoids undefined behavior from incrementing an
 * end iterator.
 */
template<typename Iter>
constexpr Iter heap_child_right(Iter start, Iter end, Iter pos)
{
    auto sibling = details::heap_child_left(start, end, pos);
    return sibling == end ? sibling : sibling + 1;
}

/**
 * Ensures the branch rooted at `current` satisfies the heap property, assuming
 * that all of the branches below `current` already satisfy the heap property.
 */
template<typename Iter, typename Compare>
void heapify_branch(Iter heap_start, Iter heap_end, Iter current, Compare comp)
{
    // Ensure the iterator supports random access.
    using category = typename std::iterator_traits<Iter>::iterator_category;
    static_assert(std::is_base_of_v<std::random_access_iterator_tag, category>);

    auto largest = current;
    const auto left = details::heap_child_left(heap_start, heap_end, current);
    const auto right = details::heap_child_right(heap_start, heap_end, current);

    if (left < heap_end && comp(*largest, *left)) {
        largest = left;
    }
    if (right < heap_end && comp(*largest, *right)) {
        largest = right;
    }

    if (largest != current) {
        std::iter_swap(current, largest);
        // Expected tail call optimization.
        heapify_branch(heap_start, heap_end, largest, comp);
    }
}
} // end namespace details

/**
 * Produces a max-heap in the given range.
 *
 * @tparam Iter Random iterator type.
 * @tparam Compare Callable type to compare elements.
 * @param it,end Range to be turned into a max heap.
 * @param comp Binary functor that returns true when its first argument
 *             compares less than its second.
 */
template<typename Iter, typename Compare = std::less<>>
void heapify(Iter start, Iter end, Compare comp = Compare())
{
    // Ensure the iterator supports random access.
    using category = typename std::iterator_traits<Iter>::iterator_category;
    static_assert(std::is_base_of_v<std::random_access_iterator_tag, category>);

    // Index of the bottom-right move branch with children. The difference type
    // of the iterator will be a signed type, so we do not have to worry about
    // underflow.
    auto index = (end - start) / 2 - 1;

    // Iterate over [0, index) in reverse.
    while (index >= 0) {
        details::heapify_branch(start, end, start + index, comp);
        --index;
    }
}

/**
 * Sorts the given range using an implementation of the heapsort algorithm.
 *
 * This heapsort implementation is unstable. The relative ordering of elements
 * that compare equal under the given comparison function is not necessarily
 * maintained.
 *
 * @tparam Iter Random iterator type.
 * @tparam Compare Callable type to compare elements.
 * @param it,end Range to be sorted.
 * @param comp Binary functor that returns true when its first argument
 *             compares less than its second.
 */
template<typename Iter, typename Compare = std::less<>>
void heap_sort_unstable(Iter start, Iter end, Compare comp = Compare())
{
    heapify(start, end, comp);

    while (start != end) {
        --end;
        // Place the root of the heap at the end of the random access collection.
        std::iter_swap(start, end);
        // Re-heapify the collection to find the new root.
        details::heapify_branch(start, end, start, comp);
    }

}

/**
 * A heap that owns it entries.
 *
 * This class implements the required heap interface for project 3b.
 *
 * @tparam Container Random access container used to store heap elements.
 * @tparam Compare Callable type that imposes an ordering among elements.
 */
template<typename Container, typename Compare = std::less<>>
class OwningHeap {
    // Ensure the Container type supports random access iteration.
    static_assert(std::is_base_of_v<
        std::random_access_iterator_tag,
        typename std::iterator_traits<typename Container::iterator>::iterator_category
    >);

    // Type alias for C++ container [1].
    using const_iterator = typename Container::const_iterator;

    /// The entries of this heap.
    Container m_values;

    /// The binary functor used to compare heap elements.
    Compare m_compare;

  public:
    /// Creates an OwningHeap with the given entries using the specified comparison function.
    explicit OwningHeap(Container values, Compare comp = Compare()) : m_values(std::move(values))
    {
        heapify(std::begin(m_values), std::end(m_values), comp);
    }

    /// Creates an OwningHeap from the values in the given iterator range
    /// using the specified comparison function.
    template<typename Iter>
    OwningHeap(Iter start, Iter end, Compare comp = Compare())
        : OwningHeap(std::vector(start, end), comp) {}

    void sort()
    {
        heap_sort_unstable(std::begin(m_values), std::end(m_values), m_compare);
    }

    [[nodiscard]]
    /// Returns the "parent" of the given position in the heap.
    const_iterator parent(const_iterator pos) const
    {
        return std::begin(m_values) + (std::distance(std::begin(m_values), pos) / 2);
    }

    [[nodiscard]]
    /// Returns the "left child" of the given position in the heap.
    const_iterator left(const_iterator pos) const
    {
        return details::heap_child_left(std::begin(m_values), std::end(m_values), pos);
    }

    [[nodiscard]]
    /// Returns the "right child" of the given position in the heap.
    const_iterator right(const_iterator pos) const
    {
        return details::heap_child_right(std::begin(m_values), std::end(m_values), pos);
    }

    /// Returns an iterator to the first element of this heap's underlying storage.
    [[nodiscard]] const_iterator begin() const noexcept { return std::begin(m_values); }

    /// Returns the end iterator of this heap's underlying storage.
    [[nodiscard]] const_iterator end() const noexcept { return std::end(m_values); }
};

// Template argument deduction guide for range constructor.
template<typename Iter>
OwningHeap(Iter, Iter) -> OwningHeap<std::vector<typename std::iterator_traits<Iter>::value_type>>;

#endif //EECE_2560_PROJECTS_HEAP_H
