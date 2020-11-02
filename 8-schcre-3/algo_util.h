/**
 * Algorithm utilities for project 3.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-10-22
 *
 * References
 * ==========
 *  [1] https://en.cppreference.com/w/cpp/algorithm/iter_swap
 *  [2] https://en.cppreference.com/w/cpp/algorithm/min_element
 */

#ifndef EECE_2560_PROJECTS_ALGO_UTIL_H
#define EECE_2560_PROJECTS_ALGO_UTIL_H

#include <algorithm>        // std::iter_swap
#include <functional>       // for std::less
#include <optional>         // for std::optional
#include <utility>          // for std::swap

namespace eece2560 {

namespace details {
/**
 * Returns the minimum contained in the range [it, end).
 *
 * This function is analogous to the function std::min_element from standard
 * library.
 *
 * @tparam Iter Forward iterator type.
 * @tparam Comp Callable type to compare range elements.
 * @param it,end Range to be examined.
 * @param compare Function object used to compare elements.
 * @return Minimum element in the range.
 */
template<typename Iter, typename Compare = std::less<>>
Iter min_elem(Iter it, Iter end, Compare compare = Compare())
{
    Iter curr_min = it;
    ++it;
    while (it != end) {
        if (compare(*it, *curr_min)) {
            curr_min = it;
        }
        ++it;
    }
    return curr_min;
}

/**
 * Partitions the given range such that all elements that appear before some
 * pivot compare less than the pivot.
 *
 * The pivot is taken to be the first element in the range.
 *
 * The relative ordering of equal elements that compare less than the pivot
 * is not necessarily maintained.
 *
 * @tparam Iter Forward iterator type.
 * @tparam Compare Binary functor type for comparing elements of the range.
 * @param it,end Range to be partitioned.
 * @param comp Binary functor that returns true if its first argument compares
 *             less than its second argument.
 * @return Iterator to the pivot position.
 */
template<typename Iter, typename Compare>
Iter partition_unstable(Iter it, Iter end, Compare comp)
{
    // Ensure that the iterator is a forward iterator, since this algorithm
    // requires that incrementing the iterator does not invalidate copies.
    using category = typename std::iterator_traits<Iter>::iterator_category;
    static_assert(std::is_base_of_v<std::forward_iterator_tag, category>);

    if (it == end) {
        return it;
    }

    Iter pivot = it;
    Iter boundary = it;

    while (it != end) {
        if (comp(*it, *pivot)) {
            ++boundary;
            std::iter_swap(boundary, it);
        }
        ++it;
    }
    // Note: this step places the element at the boundary position at the
    // start of the range. If this eleemnts compares equal to another element in
    // the range, then its relative ordering will be lost, making this algorithm
    // unstable. This could be fixed by either imposing a stricter iterator
    // requirement (e.g., requiring a random access iterator so that the pivot
    // can be the last element) or by using performing an addition swap at each
    // step so that the pivot moves with the boundary.
    std::iter_swap(pivot, boundary);

    return boundary;
}
} // end namespace details

/**
 * Sorts the given range using a selection sort implementation. This sorting
 * algorithm is stable.
 *
 * Runs in O(n^2) time and O(1) space.
 *
 * @tparam Iter Forward iterator type.
 * @param it,end Range to be sorted.
 */
template<typename Iter, typename Compare = std::less<>>
void selection_sort(Iter it, Iter end, Compare comp = Compare())
{
    while (it != end) {
        std::iter_swap(it, eece2560::details::min_elem(it, end, comp));
        ++it;
    }
}

/**
 * Returns an iterator to an element in [start, end) that is equal to needle
 * under the ordering imposed by comp.
 *
 * If multiple elements in the range [start, end) compare equal to the needle,
 * the element returned will be arbitrary.
 *
 * @tparam Iter Random access iterator type.
 * @tparam T Type of element being searched for.
 * @tparam Compare Callable type to compare elements.
 * @param start,end Range to be searched.
 * @param needle Value being searched for.
 * @param comp Binary function that returns true when its first argument
 *             compares less than its second.
 * @return Iterator to the matching element if a match is found.
 */
template<typename Iter, typename T, typename Compare = std::less<>>
std::optional<Iter> binary_search(Iter start, Iter end, const T& needle, Compare comp = Compare())
{
    using category = typename std::iterator_traits<Iter>::iterator_category;
    static_assert(std::is_base_of_v<std::random_access_iterator_tag, category>);

    if (start == end) {
        if (comp(*start, needle) || comp(needle, *start)) {
            return std::nullopt;
        } else {
            return start;
        }
    } else {
        Iter mid = start;
        std::advance(mid, (end - start) / 2);
        if (comp(*mid, needle)) {
            return eece2560::binary_search(++mid, end, needle, comp);
        } else {
            return eece2560::binary_search(start, mid, needle, comp);
        }
    }

}

/**
 * Sorts the given range using an implementation of the quicksort algorithm.
 *
 * This quicksort implementation is unstable since an unstable partition
 * algorithm is used. The relative ordering of elements that compare equal
 * under the given comparison function is not necessarily maintained.
 *
 * @tparam Iter Forward iterator type.
 * @tparam Compare Callable type to compare elements.
 * @param it,end Range to be sorted.
 * @param comp Binary functor that returns true when its first argument
 *             compares less than its second.
 */
template<typename Iter, typename Compare = std::less<>>
void quicksort_unstable(Iter it, Iter end, Compare comp = Compare())
{
    if (it != end) {
        Iter mid = eece2560::details::partition_unstable(it, end, comp);
        eece2560::quicksort_unstable(it, mid, comp);
        eece2560::quicksort_unstable(++mid, end, comp);
    }
}

} // end namespace eece2560
#endif //EECE_2560_PROJECTS_ALGO_UTIL_H
