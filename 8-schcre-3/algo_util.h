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
 */

#ifndef EECE_2560_PROJECTS_ALGO_UTIL_H
#define EECE_2560_PROJECTS_ALGO_UTIL_H

#include <algorithm>        // std::iter_swap
#include <functional>       // for std::less
#include <optional>         // for std::optional
#include <utility>          // for std::swap

namespace eece2560 {

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
template<typename Iter, typename Comp = std::less<>>
Iter min_elem(Iter it, Iter end, Comp compare = Comp())
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
 * Sorts the given range using a selection sort implementation. This sorting
 * algorithm is stable.
 *
 * Runs in O(n^2) time and O(1) space.
 *
 * @tparam Iter Forward iterator type.
 * @param it,end Range to be sorted.
 */
template<typename Iter>
void selection_sort(Iter it, Iter end)
{
    while (it != end) {
        std::iter_swap(it, min_elem(it, end));
        ++it;
    }
}

template<typename Iter, typename T, typename Compare = std::less<T>>
std::optional<Iter> binary_search(Iter start, Iter end, const T& needle, Compare comp = Compare())
{
    static_assert(std::is_base_of_v<
        std::random_access_iterator_tag,
        typename std::iterator_traits<Iter>::iterator_category
    >);

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

} // end namespace eece2560
#endif //EECE_2560_PROJECTS_ALGO_UTIL_H
