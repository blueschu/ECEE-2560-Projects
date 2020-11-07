/**
 * A heap / heapsort implementation.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-11-02
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
 * Ensures the branch rooted at `current` satisfies the heap property, assuming
 * that all of the branches below `current` already satisfy the heap property.
 */
template<typename Iter, typename Compare>
void heapify_branch(Iter heap_start, Iter heap_end, Iter current, Compare comp)
{
    // Ensure the iterator supports random access.
    using category = typename std::iterator_traits<Iter>::iterator_category;
    static_assert(std::is_base_of_v<std::random_access_iterator_tag, category>);

    const auto index = std::distance(heap_start, current);

    auto largest = current;
    const auto left = heap_start + (2 * index);
    const auto right = left + 1;

    if (left < heap_end && comp(*largest, *left)) {
        largest = left;
    }
    if (right < heap_end && comp(*largest, *right)) {
        largest = right;
    }

    if (largest != current) {
        std::iter_swap(current, largest);
        heapify_branch(heap_start, heap_end, largest, comp);
    }

};
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

    auto mid = start + (end - start) / 2;

    while (mid != start) {
        details::heapify_branch(start, end, mid, comp);
        --mid;
    }
    details::heapify_branch(start, end, start, comp);
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
 * @tparam Container Random access container used to store heap elements
 * @tparam Compare
 */
template<typename Container, typename Compare = std::less<>>
class OwningHeap {
    // Ensure the Container type support random access iteration.
    static_assert(std::is_base_of_v<
        std::random_access_iterator_tag,
        typename std::iterator_traits<typename Container::iterator>::iterator_category
    >);

    // The entries of this heap.
    Container m_values;

    // The binary functor used to compare heap elements.
    Compare m_compare;

  public:
    // Creates an OwningHeap with the given entries using the specified comparison function.
    explicit OwningHeap(Container values, Compare comp = Compare()) : m_values(std::move(values))
    {
        heapify(std::begin(m_values), std::end(m_values), comp);
    }

    template<typename Iter>
    OwningHeap(Iter start, Iter end, Compare comp = Compare())
        : OwningHeap(std::vector(start, end), comp) {}

    void sort()
    {
        heap_sort_unstable(std::begin(m_values), std::end(m_values), m_compare);
    }
};

// Template argument deduction guide for range constructor.
template<typename Iter>
OwningHeap(Iter, Iter) -> OwningHeap<std::vector<typename std::iterator_traits<Iter>::value_type>>;

#endif //EECE_2560_PROJECTS_HEAP_H
